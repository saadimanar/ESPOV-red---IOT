#include "driver/i2s.h"
#include <FastLED.h>
static const int pixelCount = 45 + 2;
static const int samplesNeeded = (pixelCount * 24 * 4 + 31) / 32;
static const int stereoSamplesNeeded = (samplesNeeded + 1) / 2;
static const int bufferSize = 128;
static const int bufferCount = (stereoSamplesNeeded + bufferSize - 1) / bufferSize;
static const int allocatedSamples = bufferCount * bufferSize * 2;

CRGB leds[NUM_LEDS];

static unsigned int bitLUT[256];
unsigned long pixels[allocatedSamples];

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

void loopPixels() 
{
  static int rgb = 0;
  rgb++;
  int sample = 0;
  for(int i = 0; i < pixelCount; i++)
  {
    pixels[sample++] = bitLUT[(rgb >> 8) & 255];
    pixels[sample++] = bitLUT[rgb & 255];
    pixels[sample++] = bitLUT[(rgb >> 16) & 255];
  }   
   for(int i =0; i<pixelCount-2;i++){
        LED[i] = pixels[i];
    }
  FastLED.show();
}
