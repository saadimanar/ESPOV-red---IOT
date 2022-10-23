// ======================== INCLUDE=========================================

#include "Gyro.h"
#include "File.h"
#include <vector>
#include <FastLED.h>
#include "bluetooth.h"

// ===================== DEFINE =============================================
#define DATA_PIN    27
#define NUM_LEDS    66
#define LED_TYPE    WS2812B
#define COLOR_ORDER RGB
#define BRIGHTNESS 128

// ===================== Globals =============================================
static const int pixelCount = 66 + 2;
static const int buttonPin = 5;
static const int ledpin = 27;
static const int ledCount = 66;
static const int speed = 3;
Gyro gyro(0, 1);
uint8_t image[55*55][3];
uint8_t image_rec[55*55*3];
int imageRes[] = {55, 55};
int currentImage = 0;
int pressed = 0;
CRGB leds[66];
extern const uint8_t gamma8[];  
bool on = false;
int visibleLeds = 0;
using namespace std;



int curr_index;
// ===================== Functions =============================================


bool loadCurrentImage()
{
  char filename[32];
  sprintf(filename, "/spiffs/image%d.bin", currentImage);
  if(!readFromFile(filename, image[0], 55 * 55 * 3))
  {
    for(int y = 0; y < 55; y++)
      for(int x = 0; x < 55; x++)
      {
        image[y * 55 + x][0] = 255;
        image[y * 55 + x][1] = 0;
        image[y * 55 + x][2] = 0;
      }    
    return false;
  } 
  return true; 
}

void turnOn()
{
  on = true;
  visibleLeds = 0;
  gyro.wakeUp();
}

void turnOff()
{
  on = false;
  visibleLeds = ledCount * speed + 100;
}

vector<float> return_angle(int dt)
{ 
  static float angle = 0;
  gyro.poll();
    float td = sqrt(gyro.rotationV[0] * gyro.rotationV[0] + gyro.rotationV[1] * gyro.rotationV[1] + gyro.rotationV[2] * gyro.rotationV[2]);
  float d = gyro.rotationV[2] * dt * 0.001;
  angle += d; 
  if(td < 5){
    float l = sqrt(gyro.positionA[0] * gyro.positionA[0] + gyro.positionA[1] * gyro.positionA[1] + gyro.positionA[2] * gyro.positionA[2]);
    float rl = 1 / ((l == 0)? 1 : l);
    angle = angle * 0.9 + acos(rl * gyro.positionA[0]) * 180 / M_PI * 0.1;
            }
  float sx = -cos(angle * M_PI / 180); 
  float sy = -sin(angle * M_PI / 180); 
  vector<float> result;
  result.push_back(sx);
  result.push_back(sy);
  return result;
}

void loopSaber(int dt)
{
  uint32_t res[66];
  vector<float> result = return_angle(dt);
  float sx = result[0];
  float sy = result[1];
  int sample = 0;
  for(int i = 0; i < pixelCount; i++)
  {
    int x = 30 + (int)(-sx * (i + 20)); 
    int y = 70 + (int)(sy * (i + 20));
    if(i * speed < visibleLeds)
    {
      int a = 0;
      if(x >= 0 && y >= 0 && x < imageRes[0] && y < imageRes[1]) 
        {
            a = imageRes[0] * y + x;
            leds[i] =CRGB(pgm_read_byte(&gamma8[image[a][1]]),pgm_read_byte(&gamma8[image[a][0]]),pgm_read_byte(&gamma8[image[a][2]]));
        }
     }
      else
     {
      //====default color======
     leds[i] = CRGB(pgm_read_byte(&gamma8[146]),pgm_read_byte(&gamma8[103]),pgm_read_byte(&gamma8[103]));
     } 
  }
   FastLED.show();
}

void update_image_byte_by_byte(uint8_t byte) {
  int color = curr_index / (55*55);
  //assert(color == 0 || color == 1 || color == 2);
  int i = (curr_index % (55*55)) / 55;
  int j = (curr_index % (55*55)) % 55;
  if (color == 0) {
    image[i*55+j][0] = byte;
  } else if (color == 1) {
    image[i*55+j][1] = byte;
  } else {
   image[i*55+j][2] = byte;
  }
  curr_index++;
  if (curr_index == (55*55*3)) {
    clear_pending_image();
  }
}

void clear_pending_image() {
  Serial.println("Called clear_pending_image.");
  curr_index = 0;
}


void setup()
{
  Serial.begin(115200);
  bluetooth::begin();
   //setup the LEDs
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  // setup the button,gyro and filesystem 
  pinMode(buttonPin, INPUT);
  gyro.calculateCorrection();    
  initFileSystem();
  currentImage = 0 ;
}




// ===================== main =============================================


void loop() {

if (!bluetooth::available()){
  static int time = 0;
  int t = millis();
  int dt = t - time;
  time = t;
  if(digitalRead(buttonPin) == LOW && !on){
    turnOn();
    delay(1000);  
    }
 else{
  if(digitalRead(buttonPin) == LOW && on)
  {
    clear_pending_image();
    turnOff();
    delay(1000);
  }
    }
   loopSaber(dt);
  if(on)
     visibleLeds+=dt;
  else
     visibleLeds-=dt;
    
  }
  
  if (bluetooth::available()) {
    while (bluetooth::available()) {
      auto image_bytes = bluetooth::read();
      update_image_byte_by_byte((uint8_t)image_bytes);
    }
   
  } 
 
  
}


//============ FOR COLOR CORRECTION ======================
const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
