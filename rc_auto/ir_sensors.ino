
#define frontLeftDown 23
#define frontLeftUp 22
#define frontRightDown 3
#define frontRightUp 41
#define backLeft 40
#define backRight 39


// HIGH means no obstacle
int isFrontObstacle = HIGH;
int isBackObstacle = HIGH;


void irSensorsSetup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(frontRightDown, INPUT);
  pinMode(backRight, INPUT);
  attachInterrupt(digitalPinToInterrupt(frontRightDown), emergencyStop, RISING);

}

// This is only tirggered by the front right down sensor (only 1 interrupt pin available)
void emergencyStop() {
  Serial.println(F("[ir_sensor] Emergency stop triggered!!!"));
 // stopCar();
}

bool isFrontObstacleDetected() {
  isFrontObstacle = !digitalRead(frontLeftDown) || !digitalRead(frontRightDown) || !digitalRead(frontLeftUp) || !digitalRead(frontRightUp);
  setInbuiltLedLight();

  if (isFrontObstacle) {
    Serial.println("[ir_sensor] Front obstacle detected");
  }

  return isFrontObstacle;
}

bool isBackObstacleDetected() {
  isBackObstacle = !digitalRead(backLeft) || !digitalRead(backRight);
  setInbuiltLedLight();

  if (isBackObstacle) {
    Serial.println("[ir_sensor] Back obstacle detected");
  }

  return isBackObstacle;
}

void setInbuiltLedLight() {
  if (isFrontObstacle || isBackObstacle) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
