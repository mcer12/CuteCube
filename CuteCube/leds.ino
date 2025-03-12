
void handleOrientation(int side) {
  //int whichZero = random(0, 2);
  //int randR = random(0, 5535);
  //int randG = random(0, 5535);
  //int randB = random(0, 5535);

  if (!getLedsPower()) {
    setLedsPower(true);
    delay(5);
  }

  if (side == 0) {
    clearTo(bri + extraR, 0, 0);
  }
  if (side == 1) {
    clearTo(0, bri + extraG, 0);
  }
  if (side == 2) {
    clearTo(0, 0, bri + extraB);
  }
  if (side == 3) {
    clearTo(bri + extraR, bri + extraG, 0);
  }
  if (side == 4) {
    clearTo(0, bri + extraG, bri + extraB);
  }
  if (side == 5) {
    clearTo(bri + extraR, 0, bri + extraB);
  }
  strip.show();

}

void handleShuffle() {
#ifdef DEBUG_ENABLE
  Serial.println("shuffling!");
  Serial.println("shuffling!!");
  Serial.println("shuffling!!!");
  Serial.println("shuffling!!!!");
  Serial.println("shuffling!!!!!");
#endif

  if (!getLedsPower()) {
    setLedsPower(true);
    delay(5);
  }

  int whichActive = random(0, 2);
  bool secondActive = (random(0, 1) == 1) ? true : false;
  //int whichZero = random(0, 2);
  int randR = random(0, bri);
  int randG = random(0, bri);
  int randB = random(0, bri);

  int r, g, b;
  int steps = 20;
  int chunk = bri / steps;

  for (int repeat = 0; repeat < 20; repeat++) {
    whichActive = random(0, 2);
    secondActive = random(0, 1);
    randR = random(0, bri);
    randG = random(0, bri);
    randB = random(0, bri);

    for (int i = 0; i < steps; i++) {
      r = ((randR * i) / steps);
      g = ((randG * i) / steps);
      b = ((randB * i) / steps);
      if (whichActive == 0) {
        clearTo(i * chunk, secondActive ? g : 0, secondActive ? 0 : b);
      }
      if (whichActive == 1) {
        clearTo(secondActive ? r : 0, i * chunk, secondActive ? 0 : b);
      }
      if (whichActive == 2) {
        clearTo(secondActive ? r : 0, secondActive ? 0 : g, i * chunk);
      }
      strip.show();
      delay(10);
    }
    for (int i = steps; i >= 0; i--) {
      r = ((randR * i) / steps);
      g = ((randG * i) / steps);
      b = ((randB * i) / steps);
      if (whichActive == 0) {
        clearTo(i * chunk, secondActive ? g : 0, secondActive ? 0 : b);
      }
      if (whichActive == 1) {
        clearTo(secondActive ? r : 0, i * chunk, secondActive ? 0 : b);
      }
      if (whichActive == 2) {
        clearTo(secondActive ? r : 0, secondActive ? 0 : g, i * chunk);
      }
      strip.show();
      delay(10);
    }
  }

}

void clearTo(int R, int G, int B) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, R, G, B); // Full red
  }
}

bool getLedsPower() {
  return !digitalRead(LED_EN);
}

void setLedsPower(bool enable) {
  digitalWrite(LED_EN, !enable);
}
