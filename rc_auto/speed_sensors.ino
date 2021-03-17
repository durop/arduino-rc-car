#define inSpeedSensor  2

volatile unsigned int counter = 0;

extern Timer<5> timer;

float radius_of_wheel = 0.033;
int currentRPS = 0;

void docount()  // counts from the speed sensor
{
  counter++;  // increase +1 the counter value
}

void timerIsr()
{
  int rotation = (counter / 20);  // divide by number of holes in Disc
  if (rotation != currentRPS) {
    currentRPS = rotation;
    Serial.println("+++++++++++++++++++++++++++++++++++++");
    Serial.print("Motor Speed: ");
    Serial.print(rotation, DEC);
    Serial.print(" Rotation per seconds. Speed: ");
    counter = 0; //  reset counter to zero

    // auto v = radius_of_wheel * rpm * 0.104; //0.033 is the radius of the wheel in meter
    Serial.println("+++++++++++++++++++++++++++++++++++++");
    //  https://circuitdigest.com/microcontroller-projects/speed-distance-and-angle-measurement-for-mobile-robots-using-arduino-and-lm393-h206
    // auto distance = (2*3.141*radius_of_wheel) * (left_intr/40);
    // int angle_left = (left_intr % 360) * (90/80) ;
    // int angle_right = (right_intr % 360) * (90/80) ;
    // angle = angle_right - angle_left;
  }


  return true;
}

void speedSensorsSetup()
{
  attachInterrupt(0, docount, RISING);  // increase counter when speed sensor pin goes High
  timer.every(1000, timerIsr);
}
