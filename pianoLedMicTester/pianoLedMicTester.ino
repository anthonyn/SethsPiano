/*
  Copyright (c) 2014-2016 NicoHood
  See the readme for credit to other people.

  MSGEQ7 FastLED example
  Output via Led strip and FastLED library

  Reads MSGEQ7 IC with 7 different frequencies from range 0-255
  63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
*/

// FastLED
#include "FastLED.h"


#define LED_PINS 4
#define COLOR_ORDER GRB
#define CHIPSET     WS2812 // WS2811 LPD8806
#define NUM_LEDS 120

#define BRIGHTNESS  255  // reduce power consumption
#define LED_DITHER  255  // try 0 to disable flickering
#define CORRECTION  TypicalLEDStrip

CRGB leds[NUM_LEDS]; // Define the array of leds

// MSGEQ7
#include "MSGEQ7.h"
#define pinAnalog A7
#define pinReset 5
#define pinStrobe 2
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH false

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalog> MSGEQ7;

void setup() {
  // FastLED setup
  FastLED.addLeds<CHIPSET, 4, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(CORRECTION);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.setDither(LED_DITHER);
  FastLED.show(); // needed to reset leds to zero

  // This will set the IC ready for reading
  MSGEQ7.begin();

  Serial.begin(115200);
}

void loop() {
  // Analyze without delay
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Led strip output
  if (newReading) {
    // visualize the average bass of both channels
    uint8_t val = MSGEQ7.get(MSGEQ7_BASS);
    uint8_t valLow = MSGEQ7.get(MSGEQ7_LOW);
    uint8_t valMid = MSGEQ7.get(MSGEQ7_MID);
    uint8_t valHigh = MSGEQ7.get(MSGEQ7_HIGH);

    // Reduce noise
    val = mapNoise(val);
    valLow = mapNoise(valLow);
    valMid = mapNoise(valMid);
    valHigh = mapNoise(valHigh);

    Serial.print("Low is: ");
    Serial.print(valLow);
    Serial.print(" Mid is: ");
    Serial.print(valMid);
    Serial.print(" High is: ");
    Serial.println(valHigh);

    // Visualize leds to the beat
    CRGB color = CRGB::White;
    color.nscale8_video(val);
    //fill_solid(leds, NUM_LEDS, color);
    fill_solid(leds, NUM_LEDS, color);
//
//    // Visualize leds to the beat
//    CRGB colorRed = CRGB::Red;
//    colorRed.nscale8_video(valLow);
//    //leds[0] = colorRed;
//    fill_solid(leds, NUM_LEDS, colorRed);
//
//    CRGB colorBlue = CRGB::Blue;
//    colorBlue.nscale8_video(valMid);
//    leds[1] = colorBlue;
//    fill_solid(leds, NUM_LEDS - 40, colorBlue);
//
//    CRGB colorGreen = CRGB::Green;
//    colorGreen.nscale8_video(valHigh);
//    leds[2] = colorGreen;
//    fill_solid(leds, NUM_LEDS - 80, colorGreen);

    

    // Update Leds
    FastLED.show();
  }
}
