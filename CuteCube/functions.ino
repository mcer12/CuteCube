void initSensor() {
#ifdef USE_BMI088
  int status;
  status = accel.begin();
  if (status < 0) {
    Serial.println("Accel Initialization Error");
    Serial.println(status);
    while (1) {}
  }
  status = gyro.begin();
  if (status < 0) {
    Serial.println("Gyro Initialization Error");
    Serial.println(status);
    while (1) {}
  }
  //accel.setMode(false);
  //accel.setPower(false);
  gyro.writeRegister(0x11, 0b10000000);
#else
  Wire.begin();
  Wire.setClock(400000); //400khz clock
  delay(300);
  int err = IMU.init(calib, ACCEL_ADDRESS);
  if (err != 0) {
    Serial.print("Error initializing IMU: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }

  err = IMU.setGyroRange(2000);      //USE THESE TO SET THE RANGE, IF AN INVALID RANGE IS SET IT WILL RETURN -1
  err = IMU.setAccelRange(16);       //THESE TWO SET THE GYRO RANGE TO ±500 DPS AND THE ACCELEROMETER RANGE TO ±2g

  writeByteI2C(Wire, GYRO_ADDRESS, BMI055_GYR_LPM1, 0b10000000); // suspend mode for gyro
  //writeByteI2C(Wire, GYRO_ADDRESS, BMI055_GYR_LPM1, 0b00000000); // disable suspend mode for gyro



  if (err != 0) {
    Serial.print("Error Setting range: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }
#endif
}

void readData() {
#ifdef USE_BMI088
  accel.readSensor();
  gyro.readSensor();
  accelX = accel.getAccelX_mss();
  accelY = accel.getAccelY_mss();
  accelZ = accel.getAccelZ_mss();
  gyroX = gyro.getGyroX_rads();
  gyroY = gyro.getGyroY_rads();
  gyroZ = gyro.getGyroZ_rads();
  isCharging = digitalRead(CHRG_PIN);
  //temp = accel.getTemperature_C();
#ifdef DEBUG_ENABLE
  Serial.print("BMI088, ");
#endif
#else
  IMU.update();
  IMU.getAccel(&accelData);
  IMU.getGyro(&gyroData);
  float constant = 0.61;
  accelX = accelData.accelX * constant;
  accelY = accelData.accelY * constant * -1.0;
  accelZ = accelData.accelZ * constant * -1.0;
  gyroX = gyroData.gyroX;
  gyroY = gyroData.gyroY;
  gyroZ = gyroData.gyroZ;
  //temp = IMU.getTemp();
#ifdef DEBUG_ENABLE
  Serial.print("BMI055, ");
#endif
#endif
  voltage = (movingAverageRead(ADC_PIN, 5) / 4096.0) * 2.0 * 3.3;
#ifdef DEBUG_ENABLE
  Serial.print(accelX);
  Serial.print(" = ax, ");
  Serial.print(accelY);
  Serial.print(" = ay, ");
  Serial.print(accelZ);
  Serial.print(" = az, ");
  Serial.print(gyroX);
  Serial.print(" = gx, ");
  Serial.print(gyroY);
  Serial.print(" = gy, ");
  Serial.print(gyroZ);
  Serial.print(" = gz, ");
  Serial.print(voltage);
  Serial.print(" = voltage, ");
  Serial.print(isCharging);
  Serial.print(" = chrg, ");

  //Serial.print(temp);
  Serial.print("\n");
#endif
}

void handleLowBattery() {
  clearTo(bri, 0, 0);
  strip.show();
  delay(500);
  clearTo(0, 0, 0);
  strip.show();
  delay(500);
  clearTo(bri, 0, 0);
  strip.show();
  delay(500);
  clearTo(0, 0, 0);
  strip.show();
  delay(500);
  clearTo(bri, 0, 0);
  strip.show();
  delay(500);
  clearTo(0, 0, 0);
  strip.show();
  delay(500);
}

void triggerActivity() {
  activity = true;
  wentToSleep = false;
  sleepTimer = millis();
}

int movingAverageRead(int analogPin, int numReadings) {
  int total = 0;
  for (int i = 0; i < numReadings; i++) {
    total += analogRead(analogPin);
  }
  return total / numReadings;
}
