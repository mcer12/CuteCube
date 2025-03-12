//#define USE_BMI088
#define DEBUG_ENABLE
#define ACCEL_ADDRESS 0x18
#define GYRO_ADDRESS 0x68
#define LED_PIN PB3
#define LED_EN PA15
#define NUM_LEDS 8
#define SLEEP_TIMEOUT_SECONDS 20
#define ADC_PIN PA4
#define CHRG_PIN PA5

#include <Wire.h>
#include "WS2816_Driver.h"
#include "STM32LowPower.h"

#ifdef USE_BMI088
#include "BMI088.h"
#else
#include "FastIMU.h"
#endif

#ifdef USE_BMI088
Bmi088Accel accel(Wire, ACCEL_ADDRESS);
Bmi088Gyro gyro(Wire, GYRO_ADDRESS);
#else
BMI055 IMU;
calData calib = { 0 };  //Calibration data
AccelData accelData;    //Sensor data
GyroData gyroData;
#endif

float accelX, accelY, accelZ, gyroX, gyroY, gyroZ, temp, voltage;
bool isCharging = false;
uint8_t orientation = 0;
unsigned long sleepTimer, shuffleTimer, shortShuffleTimer;
uint8_t shuffleCounter, shuffleCounterSuccess, shortShuffleCounter, shortShuffleCounterSuccess;
int bri = 40000;
int extraR = 0;
int extraG = 0;
int extraB = 0;
bool activity, wentToSleep = false;

WS2816_Driver strip(NUM_LEDS, LED_PIN);

void setup() {
  Serial.begin(115200);

  analogReadResolution(12);
  pinMode(ADC_PIN, INPUT);
  pinMode(CHRG_PIN, INPUT);
  pinMode(LED_EN, OUTPUT);
  setLedsPower(true);
  randomSeed(analogRead(ADC_PIN));
  delay(5);

  strip.begin();
  strip.clear();
  strip.show();

  delay(100);

  initSensor();

  LowPower.begin();
  sleepTimer = shuffleTimer = millis();

}


void loop() {
  readData();

  if (voltage < 3.4 && !wentToSleep) {
    handleLowBattery();
    setLedsPower(false);
#ifdef DEBUG_ENABLE
    Serial.println("low battery, going to sleep!");
    delay(20);
#endif
    LowPower.deepSleep(3000);
  }

  if (millis() - shuffleTimer > 100) {
    shuffleTimer = millis();
    shuffleCounter++;
    if (accelX > 20.0 || accelY > 20.0 || accelZ > 20.0) {
      shuffleCounterSuccess++;
    }
    if (shuffleCounter >= 10) {
      if (shuffleCounterSuccess >= 3) {
        handleShuffle();
        triggerActivity();
        orientation = -1;
      }
      shuffleCounter = shuffleCounterSuccess = 0;
    }
  }

  /*
    if (millis() - shortShuffleTimer > 50) {
      shortShuffleTimer = millis();
      shortShuffleCounter++;
      if (accelX > 15.0 || accelY > 15.0 || accelZ > 15.0) {
        shortShuffleCounterSuccess++;
      }
      if (shortShuffleCounter >= 30) {
        if (shortShuffleCounterSuccess >= 10) {
          handleShortShuffle();
        }
        shortShuffleCounter = shortShuffleCounterSuccess = 0;
      }
    }
  */

  if (accelX < -9.6 && orientation != 0) {
    orientation = 0;
    handleOrientation(orientation);
    triggerActivity();
    //strip.setPixelColor(0, 100, 0, 0);
    //strip.setPixelColor(1, 100, 0, 0);
    //strip.setPixelColor(2, 100, 0, 0);
    //strip.setPixelColor(3, 100, 0, 0);
  }
  if (accelX > 9.6 && orientation != 1) {
    orientation = 1;
    handleOrientation(orientation);
    triggerActivity();
    //strip.setPixelColor(4, 100, 0, 0);
    //strip.setPixelColor(5, 100, 0, 0);
    //strip.setPixelColor(6, 100, 0, 0);
    //strip.setPixelColor(7, 100, 0, 0);
  }
  if (accelY > 9.6 && orientation != 2) {
    orientation = 2;
    handleOrientation(orientation);
    triggerActivity();
    //strip.setPixelColor(0, 100, 0, 0);
    //strip.setPixelColor(1, 100, 0, 0);
    //strip.setPixelColor(6, 100, 0, 0);
    //strip.setPixelColor(7, 100, 0, 0);
  }
  if (accelY < -9.6 && orientation != 3) {
    orientation = 3;
    handleOrientation(orientation);
    triggerActivity();
    //strip.setPixelColor(2, 100, 0, 0);
    //strip.setPixelColor(3, 100, 0, 0);
    //strip.setPixelColor(4, 100, 0, 0);
    //strip.setPixelColor(5, 100, 0, 0);
  }
  if (accelZ > 9.6 && orientation != 4) {
    orientation = 4;
    handleOrientation(orientation);
    triggerActivity();
    //strip.setPixelColor(0, 100, 0, 0);
    //strip.setPixelColor(2, 100, 0, 0);
    //strip.setPixelColor(4, 100, 0, 0);
    //strip.setPixelColor(6, 100, 0, 0);
  }
  if (accelZ < -9.6 && orientation != 5) {
    orientation = 5;
    handleOrientation(orientation);
    triggerActivity();
    //strip.setPixelColor(1, 100, 0, 0);
    //strip.setPixelColor(3, 100, 0, 0);
    //strip.setPixelColor(5, 100, 0, 0);
    //strip.setPixelColor(7, 100, 0, 0);
  }


  if (millis() - sleepTimer > 30000) {
    //sleepTimer = millis();
    wentToSleep = true;
    setLedsPower(false);
#ifdef DEBUG_ENABLE
    Serial.println("no action, going to sleep!");
    delay(20);
#endif
    LowPower.deepSleep(3000);
  }

#ifdef DEBUG_ENABLE
  delay(10);
#else
  LowPower.deepSleep(20);
#endif

}
