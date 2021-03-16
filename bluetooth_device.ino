extern Timer<5> timer;

void bluetoothSetup() {
  Serial2.begin(9600);
  Serial2.setTimeout(50);
  Serial2.write("Hola amigo");
}

void serialEvent2() {
  if (Serial2.available()) {
    Serial.print("[bluetooth] Command: ");
    auto command = Serial2.readString();
    Serial.println(command);
    char lastCharOfCommand = command.charAt(command.length() - 1);

    if (isDigit(lastCharOfCommand) || lastCharOfCommand == 'q') {
      char newSpeed = mapInputSpeed(lastCharOfCommand);
      setPwmOutput(newSpeed);
    } else if (command == "S") {
      stopCar();
    } else if (command == "F") {
      driveForward();
    } else if (command == "FS") {
      driveForward();
      delay(200);
      stopCar();
    } else if (command == "B") {
      driveBackwards();
    } else if (command == "BS") {
      driveBackwards();
      delay(200);
      stopCar();
    } else if (command == "L") {
      turnLeft();
    } else if (command == "LS") {
      turnLeft();
      delay(250);
      stopCar();
    } else if (command == "R") {
      turnRight();
    } else if (command == "RS") {
      turnRight();
      delay(250);
      stopCar();
    } else if (command == "x") {
      enableAutonomousDriving();
    } else if (command == "X") {
      disableAutonomousDriving();
    } else if (command.indexOf("OK+LOST") >= 0) {
      stopCar();
    }  else {
      Serial.print("Mensaje No conocido ");
      Serial.println(command);
    }
  }
}

int mapInputSpeed(char input) {
  int newSpeed = 0;

  if (input == 'q') {
    newSpeed = 255;
  } else if (input == '1') {
    newSpeed = 0;
  } else if (input == '2') {
    newSpeed = 120;
  } else if (input == '3') {
    newSpeed = 140;
  } else if (input == '4') {
    newSpeed = 160;
  } else if (input == '5') {
    newSpeed = 170;
  } else if (input == '6') {
    newSpeed = 180;
  } else if (input == '7') {
    newSpeed = 190;
  } else if (input == '8') {
    newSpeed = 200;
  } else if (input == '9') {
    newSpeed = 220;
  }

  return newSpeed;
}

void bluetoothloop() {
  // read from port 0, send to port 1:
  if (Serial.available()) {
    Serial.println("Serial.available()");
    int inByte = Serial.read();
    Serial2.print(inByte, DEC);
  }

  // Serial.print("Tick");
}
