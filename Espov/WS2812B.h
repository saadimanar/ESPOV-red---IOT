#include <FastLED.h>
#include <Adafruit_NeoPixel.h>


//initializing variables
static const int pixelCount = 45;                // +2?
static const int samplesNeeded = (pixelCount * 24 * 4 + 31) / 32;
static const int stereoSamplesNeeded = (samplesNeeded + 1) / 2;
static const int bufferSize = 40;
static const int bufferCount = (stereoSamplesNeeded + bufferSize - 1) / bufferSize;
static const int allocatedSamples = bufferCount * bufferSize * 2;

//initializing arrays
static unsigned int bitLUT[256];
unsigned long pixels[allocatedSamples];

using namespace std;

//class for the LED strip
#define num_of_leds 45
#define pin_num 27
 Adafruit_NeoPixel Strip(num_of_leds, pin_num, NEO_GRB + NEO_KHZ800);
 
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
