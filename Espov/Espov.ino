// ======================== INCLUDE=========================================

#include "Gyro.h"
#include "File.h"
#include <vector>
#include <FastLED.h>

//===================ARGS FOR LEDS===========================================

static const int pixelCount = 66 + 2;            
static const int samplesNeeded = (pixelCount * 24 * 4 + 31) / 32;
static const int stereoSamplesNeeded = (samplesNeeded + 1) / 2;
static const int bufferSize = 55;
static const int bufferCount = (stereoSamplesNeeded + bufferSize - 1) / bufferSize;
static const int allocatedSamples = bufferCount * bufferSize * 2;

// ==================== BLUETOOTH ===========================================

#include "BluetoothSerial.h"
//  check if Bluetooth is properly enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial ESP_BT;
int incoming;
// ===================== DEFINE =============================================
#define DATA_PIN    27
#define NUM_LEDS    66
#define LED_TYPE    WS2812B
#define COLOR_ORDER RGB
#define BRIGHTNESS 128
#define MAX_IMG 3
//====================== VARIABLES ===========================================
static const int buttonPin = 5;
static const int ledpin = 27;
static const int ledCount = 66;
static const int speed = 3; 
Gyro gyro(0, 1);
unsigned char image[55*55][3];
int imageRes[] = {55, 55};
int currentImage = 0;
int pressed = 0;
CRGB leds[66];
extern const uint8_t gamma8[];

using namespace std;
int counter = 0;


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

  
void setup()
{
  Serial.begin(115200);
  while(!Serial);
  ESP_BT.begin("ESP32_RED"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(buttonPin, INPUT);
  gyro.calculateCorrection();    
  initFileSystem();
  currentImage = 0 ; 
  bool loaded = loadCurrentImage();
  if(!loaded){
    Serial.println("image not loaded");
   }
}


bool on = false;
int visibleLeds = 0;

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


void loop()
{ 
  if (ESP_BT.available()) {
    Serial.println("paired");
    Serial.write(ESP_BT.read());
  }
  delay(20);
  
  //image is changed when button is pressed.
  
  static int time = 0;
  int t = millis();
  int dt = t - time;
  time = t;
  if(digitalRead(buttonPin) == LOW && !on){
    delay(1000); 
    turnOn();
    counter = -1;
 
    }
 else{
  if(digitalRead(buttonPin) == LOW && on && counter == MAX_IMG)
  { 
    counter = -1;
    delay(1000);
    turnOff();
  }
    }
    pressed += dt;
   loopSaber(dt);
   if(digitalRead(buttonPin) == LOW && on && counter <=2){
        delay(1000); 
     currentImage = (counter + 1) % 4;
     bool loaded = loadCurrentImage();  
     if(!loaded){
      Serial.println(currentImage);
      }
     pressed = 0;
     counter += 1;
    
  } 
  if(on)
     visibleLeds+=dt;
  else
     visibleLeds-=dt;

//image is changed every 10 sec.

  /*static int time = 0;
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
    turnOff();
    delay(1000);
  }
    }
    pressed += dt;
   loopSaber(dt);
   if(pressed > 10000 && on){
     currentImage = (currentImage + 1) % 4;
     bool loaded = loadCurrentImage();  
     if(!loaded){
      Serial.println(currentImage);
      }
     pressed = 0;
  } 
  if(on)
     visibleLeds+=dt;
  else
     visibleLeds-=dt;*/
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


  
