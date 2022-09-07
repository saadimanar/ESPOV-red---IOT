#include <FastLED.h>

#define DATA_PIN    27
#define NUM_LEDS    45
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER RGB

CRGB leds[NUM_LEDS];


void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {

  for(int i=0; i<NUM_LEDS;i++){
      leds[i] = CRGB( 50, 100, 150);
    }
  FastLED.show();
  delay(10);
}
