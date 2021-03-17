#include <arduino-timer.h>
#include "Wire.h"

Timer<5> timer;

int ledPin = LED_BUILTIN;
bool isAutonomousDrivingEnable = true;


void setup()
{
  Serial.begin(19200);
  Wire.begin();

  pinMode(ledPin, OUTPUT);

  irSensorsSetup();
  carshSensorsSetup();
  ultrasonicSetup();
  motorDriveControllerSetup();
  speedSensorsSetup();
  gyroscopeSetup();
  bluetoothSetup();
  timer.every(250, checkForObstacles);
  timer.every(200, measureDistance);
  timer.every(1000, gyroscopeUpdate);
}

void loop() {
  timer.tick();
}

bool measureDistance() {
  uSensorMesureDistance();

  return true;
}

bool gyroscopeUpdate() {
  gyroscopeUpdateMeasures();

  return true;
}

bool checkForObstacles() {
  bool wasObstacleDetected = false;
  bool wasImminentCrashDetected = false;
  bool hasFrontCollision = wasFrontCrushDetected();
  bool existsFrontObstacle = isFrontObstacleDetected();
  bool existsBackObstacle = isBackObstacleDetected();
  bool isDrivingDirectionFowrard = getIsDrivingDierctionForward();
  int distanceToObstacle = getLastDistanceToObject();
  bool wasImminentFrontCrashDetected = isImminentFrontCrashDetected(distanceToObstacle, existsFrontObstacle);

  if ((isDrivingDirectionFowrard && (wasImminentFrontCrashDetected || hasFrontCollision)) || (!isDrivingDirectionFowrard && existsBackObstacle)) {
    wasImminentCrashDetected = true;
  } else if (distanceToObstacle < 25) {
    wasObstacleDetected = true;
  }

  if (wasImminentCrashDetected) {
    stopCar();
    if (isAutonomousDrivingEnable) {
      avoidObstacle(distanceToObstacle);
    }
  } else if (wasObstacleDetected) {
    enableReducedSpeed();
  } else if (isAutonomousDrivingEnable && !getIsDrivingFowrard() && !wasImminentFrontCrashDetected && !hasFrontCollision) {
    driveForward();
  }

  if (getIsDrivingFowrard() && getIsReducedSpeedEnabled() && !wasImminentCrashDetected && !wasObstacleDetected) {
    disableReducedSpeed();
  }

  if (wasImminentCrashDetected || wasObstacleDetected) {
    Serial.print(F("[rc_auto] Is driving forward: "));
    Serial.println((isDrivingDirectionFowrard == 1 ? "true" : "false"));
    Serial.print(F("[rc_auto] Imminent crash detected: "));
    Serial.println((wasImminentCrashDetected == 1 ? "true" : "false"));
    Serial.print(F("[rc_auto] Obstacle detected by ultrasonic sensor: "));
    Serial.println((wasObstacleDetected == 1 ? "true" : "false"));
    Serial.print(F("[rc_auto] Obstacle detected at "));
    Serial.println(distanceToObstacle);
    Serial.print(F("[rc_auto] Front obstacle detected: "));
    Serial.println((existsFrontObstacle == 1 ? "true" : "false"));
    Serial.print(F("[rc_auto] Back obstacle detected: "));
    Serial.println((existsBackObstacle == 1 ? "true" : "false"));
    Serial.println(F("**************************************************"));
  }

  return true;
}

bool isImminentFrontCrashDetected(unsigned int distanceToObstacle, bool existsFrontObstacle) {
  if ((distanceToObstacle < 10 || existsFrontObstacle)) {
    return true;
  }

  return false;
}

void avoidObstacle(int distanceToObstacle) {
  bool isDrivingFowrard = getIsDrivingDierctionForward();

  if (distanceToObstacle > 5) {
    turnLeft();
    delay(500);
  } else if (isDrivingFowrard && !isBackObstacleDetected()) {
    driveBackwards();
  } else if (!isDrivingFowrard && !isFrontObstacleDetected() && !wasFrontCrushDetected() && getLastDistanceToObject() > 15) {
    driveForward();
  }
}

void enableAutonomousDriving() {
  Serial.println(F("[rc_auto] Autonomous driving enabled"));
  isAutonomousDrivingEnable = true;
  driveForward();
}

void disableAutonomousDriving() {
  Serial.println(F("[rc_auto] Autonomous driving disabled"));
  isAutonomousDrivingEnable = false;
  stopCar();
}
