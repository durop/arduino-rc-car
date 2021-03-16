
//#define frontLeft 40
#define frontRight 45

int isLeftCrushTrigger = HIGH;
int isRightCrushTrigger = HIGH;

void carshSensorsSetup() {
  pinMode(frontRight, INPUT);
}

bool wasFrontCrushDetected() {
  bool hasCrushed = !digitalRead(frontRight);

  if (hasCrushed) {
    Serial.println(F("[crush_sensor] Front collision detected"));
  }

  return hasCrushed;
}
