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
    checkCommand(input);
  }
}

void checkCommand(char command[200]) {
  if (jsonCommand.containsKey("x") && jsonCommand.containsKey("y")) {

    x = jsonCommand["x"];
    y = jsonCommand["y"];
    sw = jsonCommand["sw"];

    if (x <= 400) {
      turnRightInPosition();
    } else if (x >= 600) {
      turnLeftInPosition();
    } else if (y <= 450) {
      Serial.println("Drive back command");
      driveBackwards();
    } else if (y >= 550) {
       Serial.println("Drive forward command");
      driveForward();
    } else {
      stopCar();
    }
  } else if (command == "x") {
    //enableAutonomousDriving();
  } else if (command == "X" || command == "OK+CONN") {
    //disableAutonomousDriving();
  }  else {
    Serial.print("Mensaje No conocido ");
    Serial.println(command);
  }
}
