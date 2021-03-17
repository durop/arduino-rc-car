#define enL  11
#define inL1 26
#define inL2 27
#define enR  10
#define inR1 28
#define inR2 29

extern Timer<5> timer;
bool _isDrivingDirectionForward = true;
bool isReducedSpeedEnabled = false;
long initialAngle = 0;
int pwmOutput = 0; // Map the potentiometer value from 0 to 255
int pwmOutputBeforeReducedSpeed = 0;

int getPwmOutput() {
  return pwmOutput;
}

void setPwmOutput(int pwm) {
  pwmOutput = pwm > 255 ? 255 : pwm;
  pwmOutput = pwm >= 0 && pwm <= 50 ? 0 : pwm;

  // If the car was moving then update the engines speed.
  if (getIsCarMoving() && pwmOutput != 0) {
    _isDrivingDirectionForward ? driveForward() : driveBackwards();
  }
}

// Useful to level the engines power. Ex: After left or right turn.
void levelEngines() {
  analogWrite(enL, pwmOutput);
  analogWrite(enR, pwmOutput);
}

bool getIsCarMoving() {
  return pwmOutput > 0;
}

bool getIsReducedSpeedEnabled() {
  return isReducedSpeedEnabled;
}

bool getIsDrivingDierctionForward() {
  return true;
}

bool getIsDrivingFowrard() {
  return _isDrivingDirectionForward && pwmOutput > 0;
}

bool getIsDrivingBackwards() {
  return !_isDrivingDirectionForward && pwmOutput > 0;
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
  _isDrivingDirectionForward = true;
  setLeftEngineForwards();
  setRightEngineForwards();

  delay(20);
}

void setLeftEngineForwards() {
  digitalWrite(inL1, HIGH);
  digitalWrite(inL2, LOW);
}

void setRightEngineForwards() {
  digitalWrite(inR1, LOW);
  digitalWrite(inR2, HIGH);

}

void setEngineBackwards() {
  _isDrivingDirectionForward = false;

  setLeftEngineBackwards();
  setRightEngineBackwards();

  delay(20);
}

void setLeftEngineBackwards() {
  digitalWrite(inL1, LOW);
  digitalWrite(inL2, HIGH);
}

void setRightEngineBackwards() {
  digitalWrite(inR1, HIGH);
  digitalWrite(inR2, LOW);
}

void driveForward() {
  if (isEngingeStop()) {
    setPwmOutput(255);
  } else {
    levelEngines();
  }

  setEngineForward();

  analogWrite(enL, pwmOutput); // Send PWM signal to L298N Enable pin
  analogWrite(enR, pwmOutput);
  delay(50);

  Serial.print(F("[motor_drive] driving forward at "));
  Serial.println(pwmOutput);
}

void driveBackwards() {
  if (isEngingeStop()) {
    setPwmOutput(255);
  } else {
    levelEngines();
  }

  setEngineBackwards();

  analogWrite(enL, pwmOutput);
  analogWrite(enR, pwmOutput);
  delay(50);

  Serial.println(F("[motor_drive] driving backwards at"));
  Serial.println(pwmOutput);
}

void stopCar() {
  Serial.println(F("[motor_drive] Stoping car"));

  noInterrupts();
  setPwmOutput(0);
  analogWrite(enL, 0);
  analogWrite(enR, 0);
  interrupts();
}

void enableReducedSpeed() {
  if (!isReducedSpeedEnabled) {
    isReducedSpeedEnabled = true;
    pwmOutputBeforeReducedSpeed = pwmOutput;
    int newSpeed =  pwmOutput - (pwmOutput / 3);
    newSpeed = newSpeed > 100 ? newSpeed : 100;
    setPwmOutput(newSpeed);

    Serial.print(F("[motor_drive] Reduced speed enabled. New Speed "));
    Serial.println(newSpeed);
  }
}

void disableReducedSpeed() {
  setPwmOutput(pwmOutputBeforeReducedSpeed);
  isReducedSpeedEnabled = false;
  // ToDo: If the speed gets changed during the reduced speed then it will set pwmOutputBeforeReducedSpeed as speed insted of the new speed.
  setPwmOutput(pwmOutputBeforeReducedSpeed);

  Serial.print(F("[motor_drive] Reduced speed disabled. New Speed "));
  Serial.println(pwmOutputBeforeReducedSpeed);
}

void turnLeft() {
  if (getIsCarMoving()) {
    if (_isDrivingDirectionForward) {
      Serial.println(F("[motor_drive] Left forward turn."));
      analogWrite(enL, pwmOutput);
      analogWrite(enR, (pwmOutput / 2));
    } else {
      Serial.println(F("[motor_drive] Left backwards turn."));
      analogWrite(enL, (pwmOutput / 2));
      analogWrite(enR, pwmOutput);
    }
  } else {
    if (!getIsCarMoving()) {
      setPwmOutput(255);
    }
    if (_isDrivingDirectionForward) {
      Serial.println(F("[motor_drive] Left turn in current position."));
      setRightEngineBackwards();
      analogWrite(enL, pwmOutput);
      analogWrite(enR, pwmOutput);
    } else {
      Serial.println(F("[motor_drive] Left turn in current position."));
      setLeftEngineForwards();
      analogWrite(enL, pwmOutput);
      analogWrite(enR, pwmOutput);
    }
  }
  delay(50);
}

void turnRight(bool makeStopTurn) {
  if (getIsCarMoving()) {
    if (_isDrivingDirectionForward) {
      Serial.println(F("[motor_drive] Right forward turn."));
      analogWrite(enL, (pwmOutput / 2));
      analogWrite(enR, pwmOutput);
    } else {
      Serial.println(F("[motor_drive] Right backwards turn."));
      analogWrite(enL, pwmOutput);
      analogWrite(enR, (pwmOutput / 2));
    }
  } else {
    if (!getIsCarMoving()) {
      setPwmOutput(255);
    }
    if (_isDrivingDirectionForward) {
      Serial.println(F("[motor_drive] Right turn in current position."));
      setLeftEngineBackwards();
      analogWrite(enL, pwmOutput);
      analogWrite(enR, pwmOutput);
    } else {
      Serial.println(F("[motor_drive] Right turn in current position."));
      setRightEngineForwards();
      analogWrite(enL, pwmOutput);
      analogWrite(enR, pwmOutput);
    }
  }
  delay(50);
}

void turnLeft(int turnDegrees) {
  initialAngle = getPositionalAngle();
  timer.every(500, checkTurnAngle, turnDegrees);
  Serial.print(F("[motor_drive] Turn left "));
  Serial.print(turnDegrees);
  Serial.print(F(" Intial angle: "));
  Serial.println(initialAngle);

  if (_isDrivingDirectionForward) {
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
