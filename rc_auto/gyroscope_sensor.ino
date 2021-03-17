#include <MPU6050_light.h>

MPU6050 mpu(Wire);

unsigned long timerGyro = 0;

void gyroscopeSetup() {
  // Gyro and Accelerometer sensibility. Range 1-3.
  byte status = mpu.begin(1, 2);
  Serial.print(F("[gyroscope] MPU6050 status: "));
  Serial.println(status);
  while (status != 0) { } // stop everything if could not connect to MPU6050

  Serial.println(F("[gyroscope] Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true, true); // gyro and accelero
  // 0 Accelerometer only 1 Gyroscope only. Recommended 0.98
  mpu.setFilterGyroCoef(0.98);
  Serial.println("[gyroscope] Done!\n");

}

float getHorizontalInclination() {
  return mpu.getAngleY();
}

float getVerticalInclination() {
  return mpu.getAngleX();
}

float getPositionalAngle() {
  return mpu.getAngleZ();
}

void gyroscopeUpdateMeasures() {
  mpu.update();

  if (millis() - timerGyro > 15000) { // print data every second
    Serial.print(F("[gyroscope] TEMPERATURE: ")); Serial.println(mpu.getTemp());
    Serial.print(F("[gyroscope] ACCELERO  X: ")); Serial.print(mpu.getAccX());
    Serial.print("[gyroscope] \tY: "); Serial.print(mpu.getAccY());
    Serial.print("[gyroscope] \tZ: "); Serial.println(mpu.getAccZ());

    Serial.print(F("[gyroscope] GYRO      X: ")); Serial.print(mpu.getGyroX());
    Serial.print("[gyroscope] \tY: "); Serial.print(mpu.getGyroY());
    Serial.print("[gyroscope] \tZ: "); Serial.println(mpu.getGyroZ());

    Serial.print(F("[gyroscope] ACC ANGLE X: ")); Serial.print(mpu.getAccAngleX());
    Serial.print("[gyroscope] \tY: "); Serial.println(mpu.getAccAngleY());

    Serial.print(F("[gyroscope] ANGLE     X: ")); Serial.print(mpu.getAngleX());
    Serial.print("[gyroscope] \tY: "); Serial.print(mpu.getAngleY());
    Serial.print("[gyroscope] \tZ: "); Serial.println(mpu.getAngleZ());
    Serial.println(F("=====================================================\n"));
    timerGyro = millis();
  }

}
