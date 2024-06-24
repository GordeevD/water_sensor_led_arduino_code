/*
 * This Arduino Nano code was developed by newbiely.com
 *
 * This Arduino Nano code is made available for public use without any restriction
 *
 * For comprehensive instructions and wiring diagrams, please visit:
 * https://newbiely.com/tutorials/arduino-nano/arduino-nano-water-sensor
 */
// LED Control
#include "FastLED.h"
#define NUM_LEDS 8
#define DATA_PIN 5
#define BRIGHTNESS 20

// Water Sensor Control
#define POWER_PIN  A1  // The Arduino Nano pin connected to the power pin of water sensor
#define SIGNAL_PIN A0 // The Arduino Nano pin connected to the signal pin of water sensor

int waterLevelValue = 0; // variable to store the water sensor value
int sensorConnected = 0;
int waterLevelLeds[NUM_LEDS] = {300, 400, 500, 530, 550, 560, 570, 580};
// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  pinMode(POWER_PIN, OUTPUT);   // Configure D7 pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW); // turn the sensor OFF

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  checkWater();
  progressLED();
  showResultLED();
}

void showResultLED() { 
  if (!sensorConnected) {
    return;
  }
  int x = 0; // Number of confirmation blinks
  if (!waterLevelValue) {
    FastLED.setBrightness(1); // Dimm for green
    x = 2; // One blink 
  }    
  for (x; x < 3; x++) {
    for (int i = 0; i < NUM_LEDS ; i++) {
      if (waterLevelValue) {
        if (x > 0) { // last two blinks show level of water
          if (waterLevelValue > waterLevelLeds[i] || i == 0)
          {
            leds[i] = CRGB::Red;
          }
        } else {
          leds[i] = CRGB::Red; // first blink all leds
        }
      } else {
        leds[i] = CRGB::SeaGreen;
      }    
    }
    FastLED.show();
    delay(1000);
      for (int i = 0; i < NUM_LEDS ; i++) {
        leds[i] = CRGB::Black;
      }    
    FastLED.show();
    delay(200);
  }
  FastLED.setBrightness(BRIGHTNESS); // Return to set brightness
}

void progressLED() {
  // ### Led Show ####
  showLED();
  showLEDreverse();
  // Now turn the LED off, then pause
  //for (int i = 0; i < NUM_LEDS ; i++) {
  //  leds[i] = CRGB::Black;
  //}
  //FastLED.show();
  //delay(500);
}

void showLED() {
  int i = 0;
  for (i; i < NUM_LEDS ; i++) {
    leds[i] = CRGB::Blue;
    delay(60);
    FastLED.show();
    // Turn off previous led
    if (i > 0) {
      offLED(i);
    }
  // Turn off last led that left after a cycle
  //  if (i == NUM_LEDS - 1) {
  //    delay(60);
  //    offLED(NUM_LEDS);  
  //  }
  }
}

void showLEDreverse() {
  int i = NUM_LEDS - 1;
  for (i; i >= 0 ; i--) {  
    leds[i] = CRGB::Blue;
    delay(60);
    FastLED.show();
    // Turn off previos led
    if (i < NUM_LEDS - 1) {
      offLED(i + 2); // math
    }
    if(i == 0) {
      delay(60);
      offLED(1);
    }  
  }
  delay(150);
}
  
void offLED(int i) {
    leds[i-1] = CRGB::Black;
    FastLED.show();
}

void checkWater() {
  digitalWrite(POWER_PIN, HIGH);  // turn the water sensor ON
  delay(10);                      // wait 10 milliseconds
  
  waterLevelValue = analogRead(SIGNAL_PIN); // read the analog value from sensor
  waterLevelValue = waterLevelValue > 5 ? waterLevelValue : 0; // Filter ghost readings below 5
  sensorConnected = analogRead(POWER_PIN) < 1023; // read the analog value of voltage from sensor to determine if it connected
  digitalWrite(POWER_PIN, LOW);   // turn the water sensor OFF

  Serial.print("Water sensor connected: ");
  Serial.println(sensorConnected);
  
  Serial.print("Water sensor value: ");
  Serial.println(waterLevelValue);

  delay(500);
}
