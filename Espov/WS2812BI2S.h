#include <FastLED.h>



//initializing variables
static const int pixelCount = 45 + 2;                // +2?
static const int samplesNeeded = (pixelCount * 24 * 4 + 31) / 32;
static const int stereoSamplesNeeded = (samplesNeeded + 1) / 2;
static const int bufferSize = 128;
static const int bufferCount = (stereoSamplesNeeded + bufferSize - 1) / bufferSize;
static const int allocatedSamples = bufferCount * bufferSize * 2;

//initializing arrays
static unsigned int bitLUT[256];
unsigned long pixels[allocatedSamples];
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


using namespace std;

//class for the LED strip
class WS2812B {  
  int pin_number;
  int number_of_leds;
  Adafruit_NeoPixel strip(45,27,NEO_GRB + NEO_KHZ800);
;
public:
  WS2812B(int pin_number, int number_of_leds) : pin_number(pin_number), number_of_leds(number_of_leds),  strip) {
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
        clock_prescale_set(clock_div_1);
    #endif
    strip.begin();
    strip.clear();
  }
}

//move to image file
void initPixels() 
{
  for(int i = 0; i < allocatedSamples; i++)
    pixels[i] = 0;
  for(int i = 0; i < 256; i++)
  {
    bitLUT[i] = 0;
    for(int bit = 7; bit >= 0; bit--)
      bitLUT[i] |= (((i >> bit) & 1)?0b1110 : 0b1000) << (bit * 4);
  }
}
