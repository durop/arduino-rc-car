#include <arduino-timer.h>
#include "Wire.h"

Timer<5> timer;

int ledPin = LED_BUILTIN;
bool isAutonomousDrivingEnable = false;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  pinMode(ledPin, OUTPUT);

  //bluetoothSetup();
  irSensorsSetup();
  carshSensorsSetup();
  ultrasonicSetup();
  motorDriveControllerSetup();
  speedSensorsSetup();
  //gyroscopeSetup();
  wifiSetup();

  timer.every(250, checkForObstacles);
  timer.every(200, measureDistance);
  // timer.every(1000, gyroscopeUpdate);
  timer.every(50, wifiLoopCheck);
}

void loop() {
  timer.tick();
}

bool wifiLoopCheck() {
  wifiLoop();

  return true;
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
  bool wasImminentCrashDetected = false;
  bool hasFrontCollision = wasFrontCrushDetected();
  bool existsFrontObstacle = isFrontObstacleDetected();
  bool existsBackObstacle = isBackObstacleDetected();
  int distanceToFrontObstacle = getLastDistanceToObject();
  bool wasImminentFrontCrashDetected = isImminentFrontCrashDetected(distanceToFrontObstacle, existsFrontObstacle);

  if (getIsDrivingFowrard() && (wasImminentFrontCrashDetected || hasFrontCollision)) {
    Serial.println(F("[rc_auto] Automatic stop due to front object."));
    stopCar();
    wasImminentCrashDetected = true;
  } else if (getIsDrivingBackwards() && existsBackObstacle) {
    Serial.println(F("[rc_auto] Automatic stop due to back object."));
    stopCar();
    wasImminentCrashDetected = true;
  }

  if (isAutonomousDrivingEnable && wasImminentCrashDetected) {
    avoidObstacle(distanceToFrontObstacle);
  } else if (getIsDrivingFowrard() && distanceToFrontObstacle < 25) {
    enableReducedSpeed();
  } else if (isAutonomousDrivingEnable && !getIsDrivingFowrard() && !wasImminentFrontCrashDetected && !hasFrontCollision) {
    driveForward();
  }

  if (getIsDrivingFowrard() && getIsReducedSpeedEnabled() && !wasImminentCrashDetected && distanceToFrontObstacle > 25) {
    disableReducedSpeed();
  }

  if (wasImminentCrashDetected || distanceToFrontObstacle < 25) {
    Serial.print(F("[rc_auto] Is driving forward: "));
    Serial.println((getIsDrivingFowrard() ? "true" : "false"));
    Serial.print(F("[rc_auto] Is driving backwards: "));
    Serial.println((getIsDrivingBackwards() ? "true" : "false"));
    Serial.print(F("[rc_auto] Imminent crash detected: "));
    Serial.println((wasImminentCrashDetected == 1 ? "true" : "false"));
    Serial.print(F("[rc_auto] Front obstacle detected at "));
    Serial.println(distanceToFrontObstacle);
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
  bool isEngineDierctionForward = getIsEngineDierctionForward();

  if (distanceToObstacle > 5) {
    turnLeft();
    delay(500);
  } else if (isEngineDierctionForward && !isBackObstacleDetected()) {
    driveBackwards();
  } else if (!isEngineDierctionForward && !isFrontObstacleDetected() && !wasFrontCrushDetected() && getLastDistanceToObject() > 15) {
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
