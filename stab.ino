#include <Adafruit_NeoPixel.h>

struct LEDColor {  
  uint8_t r, g, b;
};

// Do not change these values unless you extend/shrinken the arrays used below
constexpr size_t maxLEDStrips = 9;
constexpr size_t maxLEDLights = 10;

const int numLED[maxLEDStrips] = {10, 8, 7, 6, 5, 3, 2, 1, 1};
const int pinAddr[maxLEDStrips] = {2, 3, 4, 5, 6, 7, 8, 9, 10};

const uint8_t wingRootColor[3] = {0, 191, 255}; // Dark
const uint8_t wingEndColor[3] = {224, 247, 255}; // Bright

// Initialized in setup()
LEDColor cachedLEDColor[maxLEDLights];
float cachedLEDIntensity[maxLEDLights];

const float colorStep = 100.f / 9.f;

// Delta is the variation between the colors, please be careful to never let the end intensity reach > 100.f or we get undefined colors
const float wingIntesityDelta[maxLEDLights] = {0.f, 10.f, 10.f, 10.f, 10.f, 10.f, 10.f, 10.f, 10.f, 0.f};

Adafruit_NeoPixel strip[maxLEDStrips];

void setup() {
  Serial.begin(9600);

  for (auto i = 0; i < maxLEDLights; ++i) {
    cachedLEDIntensity[i] = (100.f - (i * 100.f / static_cast<float>(maxLEDLights))) * 0.01f;
    Serial.print("Intensity for LED ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(cachedLEDIntensity[i]);
  }

  for (auto i = 0; i < maxLEDLights; ++i) {
    cachedLEDColor[i] = computeBaseGradientColor(i);
  }

  for (auto i = 0; i < maxLEDStrips; ++i) {
    strip[i] = Adafruit_NeoPixel(numLED[i], pinAddr[i], NEO_GRB + NEO_KHZ800);

    strip[i].begin();
    strip[i].show();
  }

  testLEDs(250);
}

void loop() {
  // put your main code here, to run repeatedly:
  //cycleColors(20);
}

LEDColor computeBaseGradientColor(int led) {
  uint8_t colorDifference[3] = {abs(wingRootColor[0] - wingEndColor[0]), abs(wingRootColor[1] - wingEndColor[1]), abs(wingRootColor[2] - wingEndColor[2])};

  // This may not be 100% accurate without round()
  LEDColor ledColor;
  ledColor.r = wingRootColor[0] + cachedLEDIntensity[led] * colorDifference[0];
  ledColor.g = wingRootColor[1] + cachedLEDIntensity[led] * colorDifference[1];
  ledColor.b = wingRootColor[2] + cachedLEDIntensity[led] * colorDifference[2];

  return ledColor;
}

void testLEDs(int delayTime) {
  Serial.println("Testing LED strips, 0 to 8");

  for (auto i = 0; i < maxLEDStrips; ++i) {
    Serial.print("Testing LED strip ");
    Serial.println(i);

    for (uint16_t currentPixel = 0; currentPixel < strip[i].numPixels(); ++currentPixel) {
      Serial.print("Testing pixel (lamp) ");
      Serial.print(currentPixel);

      strip[i].setPixelColor(currentPixel, 255, 0, 0);
      strip[i].show();

      delay(delayTime);

      strip[i].clear();
      strip[i].show();
    }
  }

  Serial.println("Setting up base gradient");
  for (auto i = 0; i < maxLEDStrips; ++i) {
    for (auto n = 0; n < maxLEDLights; ++n) {
      if (n >= strip[i].numPixels()) {
        break;
      }

      strip[i].setPixelColor(n, cachedLEDColor[n].r, cachedLEDColor[n].g, cachedLEDColor[n].b);
    }

    strip[i].show();
  }
}

void cycleColors(int delayTime) {

}

