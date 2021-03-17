#include <HCSR04.h>
#include <Servo.h>

#define TRIG_PIN 35
#define ECHO_PIN 34
#define SERVO_PIN 9

Servo myservo;
HCSR04 hc(TRIG_PIN, ECHO_PIN);

int ultrasonicDistance;
int servoAngle = 84;

void ultrasonicSetup() {
  myservo.attach(SERVO_PIN);
  myservo.write(servoAngle);// move servos to center position -> 90°a
}

void uSensorMesureDistance() {
  // Serial.println("[ultrasonic_sensor] Measuring distance to object");
  ultrasonicDistance = hc.dist();
  if (ultrasonicDistance > 400) {
    ultrasonicDistance = 400;
  }
}

unsigned int getLastDistanceToObject() {
  return ultrasonicDistance;
}

int getUltrasonicAngle() {
  return servoAngle;
}

void moveUltrasonicLeft()
{
  moveUltrasonic(60);
}


void moveUltrasonicRight() {
  moveUltrasonic(120);
}

void moveUltrasonic(int angle) {
  myservo.write(angle);
}
