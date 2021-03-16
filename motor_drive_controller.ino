#define enL  11
#define inL1 26
#define inL2 27
#define enR  10
#define inR1 28
#define inR2 29

extern Timer<5> timer;
bool isDrivingFowrard = false;
long initialAngle = 0;
int pwmOutput = 255; // Map the potentiometer value from 0 to 255

int getPwmOutput() {
  return pwmOutput;
}

void setPwmOutput(int pwm) {
  pwmOutput = pwm > 255 ? 255 : pwm;

  if (isDrivingFowrard) {
    driveForward();
  } else {
    driveBackwards();
  }
}


bool getIsDrivingFowrard() {
  return isDrivingFowrard;
}

bool isEngingeStop() {
  return pwmOutput <= 50;
}

void motorDriveControllerSetup() {
  pinMode(enL, OUTPUT);
  pinMode(inL1, OUTPUT);
  pinMode(inL2, OUTPUT);
  pinMode(inR1, OUTPUT);
  pinMode(inR2, OUTPUT);
}

void setEngineForward() {
  isDrivingFowrard = true;

  digitalWrite(inL1, HIGH);
  digitalWrite(inL2, LOW);
  digitalWrite(inR1, LOW);
  digitalWrite(inR2, HIGH);

  delay(20);
}

void setEngineBackwards() {
  isDrivingFowrard = false;

  digitalWrite(inL1, LOW);
  digitalWrite(inL2, HIGH);
  digitalWrite(inR1, HIGH);
  digitalWrite(inR2, LOW);

  delay(20);
}

void driveForward() {
  Serial.println(F("[motor_drive] driving forward"));
  setEngineForward();
  analogWrite(enL, pwmOutput); // Send PWM signal to L298N Enable pin
  analogWrite(enR, pwmOutput);
  delay(100);
}

void driveBackwards() {
  Serial.println(F("[motor_drive] driving backwards"));
  setEngineBackwards();
  analogWrite(enL, pwmOutput); // Send PWM signal to L298N Enable pin
  analogWrite(enR, pwmOutput);
  delay(100);
}

void stopCar() {
  Serial.println(F("[motor_drive] Stoping car"));

  noInterrupts();
  analogWrite(enL, 0);
  analogWrite(enR, 0);
  interrupts();

  delay(100);
}

void slowdownCar() {
  Serial.println(F("[motor_drive] Slowing down car"));
  analogWrite(enL, 160);
  analogWrite(enR, 160);
  delay(20);
}

void turnLeft() {
  Serial.print(F("[motor_drive] Turn left "));
  if (isDrivingFowrard) {
    analogWrite(enL, 0);
    analogWrite(enR, pwmOutput);
  } else {
    analogWrite(enL, pwmOutput);
    analogWrite(enR, 0);
  }
}

void turnRight() {
  Serial.print(F("[motor_drive] Turn right "));
  if (isDrivingFowrard) {
    analogWrite(enL, pwmOutput);
    analogWrite(enR, 0);
  } else {
    analogWrite(enL, 0);
    analogWrite(enR, pwmOutput);
  }
}

void turnLeft(int turnDegrees) {
  initialAngle = getPositionalAngle();
  timer.every(500, checkTurnAngle, turnDegrees);
  Serial.print(F("[motor_drive] Turn left "));
  Serial.print(turnDegrees);
  Serial.print(F(" Intial angle: "));
  Serial.println(initialAngle);

  if (isDrivingFowrard) {
    analogWrite(enL, 0);
    analogWrite(enR, pwmOutput);
  } else {
    analogWrite(enL, pwmOutput);
    analogWrite(enR, 0);
  }
}

void turnRight(int angle = 90) {
  if (isDrivingFowrard) {
    analogWrite(enL, pwmOutput);
    analogWrite(enR, 0);
  } else {
    analogWrite(enL, 0);
    analogWrite(enR, pwmOutput);
  }
}

void checkTurnAngle(long turnDegrees) {
  Serial.println("checking angle");
  bool isTurnDegreesReached = false;
  long newAngle = getPositionalAngle();

  if (newAngle >= (initialAngle + turnDegrees)) {
    Serial.print(F("[motor_drive] Finished turn. Current angle:"));
    Serial.println(newAngle);
    stopCar();

    isTurnDegreesReached = true;
  } else {
    Serial.print(F("[motor_drive] Turning... Current angle: "));
    Serial.println(newAngle);
  }

  return !isTurnDegreesReached; // Stop timer if turn degrees reached.
}
