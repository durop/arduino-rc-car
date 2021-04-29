#include <ArduinoJson.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(31, 32); // CE, CSN

const byte address[6] = "00001";

StaticJsonDocument<200> jsonCommand;


unsigned int sw = 0;
volatile unsigned int x = 500;
volatile unsigned int y = 500;

void wifiSetup() {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  Serial.println("WiFi setup completed");
}

void wifiLoop() {
  if (radio.available()) {
    char input[200] = "";
    radio.read(&input, sizeof(input));
    Serial.print("[wifi] input: ");
    Serial.println(input);

    deserializeJson(jsonCommand, input);
    checkJoystickCommand();
    checkButtonCommand();
  }
}

void checkJoystickCommand() {
  if (jsonCommand.containsKey("x")) {
    x = jsonCommand["x"];
    y = jsonCommand["y"];
    sw = jsonCommand["sw"];

    if (x <= 400) {
      turnRightInPosition();
    } else if (x >= 600) {
      turnLeftInPosition();
    } else if (y <= 450 && !isBackObstacleDetected()) {
      Serial.println("Drive back command");
      driveBackwards();
    } else if (y >= 550 && !isFrontObstacleDetected()) {
      Serial.println("Drive forward command");
      driveForward();
    } else {
      stopCar();
    }
  }
}

void checkButtonCommand() {
  if (jsonCommand.containsKey("green") || jsonCommand.containsKey("red")) {
    if (jsonCommand["green"]) {
      Serial.println("Autonomous driving enabled");
      enableAutonomousDriving();
    } else if (!jsonCommand["green"]) {
      Serial.println("Autonomous driving disabled");
      disableAutonomousDriving();
    }
  }
}
