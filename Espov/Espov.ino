#include "Gyro.h"
#include "WS2812B.h"
#include "File.h"
#include <vector>

static const int buttonPin = 5;
static const int ledpin = 27;
static const int ledCount = 45;
static const int speed = 3; 

//initialize a motion sensor with ascale = 0, gscale = 1
Gyro gyro(0, 1);


//image size is 128*128 and each pixel has 3 brightnesses, RGB
unsigned char image[128*128][3];

//image dimensions
int imageRes[] = {128, 128};

//starting from index 0 
int currentImage = 0;


bool loadCurrentImage()
{
  char filename[32];
  sprintf(filename, "/spiffs/image%d.bin", currentImage); //loads image to filename
  //if couldn't load image --> set zeros (default color)
  if(!readFromFile(filename, image[0], 128 * 128 * 3))
  {
    for(int y = 0; y < 128; y++)
      for(int x = 0; x < 128; x++)
      {
        image[y * 128 + x][0] = 0;//x * 2;
        image[y * 128 + x][1] = 0;//y * 2;
        image[y * 128 + x][2] = 0;//254 - x * 2;
      }    
    return false;
  } 
  return true;
  
}

vector<int> return_angle(int dt){
  
  static float angle = 0;
  gyro.poll();
    float td = sqrt(gyro.rotationV[0] * gyro.rotationV[0] + gyro.rotationV[1] * gyro.rotationV[1] + gyro.rotationV[2] * gyro.rotationV[2]);
  float d = gyro.rotationV[2] * dt * 0.001;
  angle += d; 
  if(td < 5)
  {
    float l = sqrt(gyro.positionA[0] * gyro.positionA[0] + gyro.positionA[1] * gyro.positionA[1] + gyro.positionA[2] * gyro.positionA[2]);
    float rl = 1 / ((l == 0)? 1 : l);
    angle = angle * 0.9 + acos(rl * gyro.positionA[0]) * 180 / M_PI * 0.1;
  }
  float sx = -cos(angle * M_PI / 180);
  float sy = -sin(angle * M_PI / 180);
  vector<int> result;
  result.push_back(sx);
  result.push_back(sy);
  return result;

  }
  
void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Strip.begin();
  Strip.clear();
  initPixels();
  pinMode(buttonPin, INPUT);
  gyro.calculateCorrection();   //calculate initial position 
  initFileSystem();
  
  //check if image is not being loaded
  bool loaded = loadCurrentImage();
  if(!loaded){
    Serial.println("image not loaded");
    }
}


bool on = false; // the strip is turned on/not
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



//update image according to current angle
void loopSaber(int dt)
{
   uint32_t res[45];
  vector<int> result = return_angle(dt);
  int sx = result[0];
  int sy = result[1];
  //-----------------------fill the pixels-----------------------------
  int sample = 0;
  for(int i = 0; i < pixelCount; i++)
  {
    int x = 64 + (int)(sx * (i + 20));
    int y = 45 + (int)(sy * (i + 20));
    if(i * speed < visibleLeds)
    {
      int a = 0;
      if(x >= 0 && y >= 0 && x < imageRes[0] && y < imageRes[1]) 
        {
            a = imageRes[0] * y + x;
            res[i] = Strip.Color(image[a][0],image[a][1],image[a][2]);  
        }
      
    }
    else
    {
      res[i] = Strip.Color(0,0,255);   
    } 
    Strip.setPixelColor(i,res[i]);
  }
 
  Strip.show();
}


void loop()
{
  // int res[45];
  static int time = 0;
  int t = millis();
  int dt = t - time;
  time = t;
  uint32_t res[45];
  if(digitalRead(buttonPin) == LOW && !on){
    turnOn();
   // loopSaber(dt);

      for(int x = 0; x < 45; x++)
      {
        Serial.println(image[x][0]);
        Serial.println(image[x][1]);
        Serial.println(image[x][2]);
        //res[x] = Strip.Color((int)image[x][0],(int)image[x][1],(int)image[x][2]);
        res[x] = Strip.Color(50,255,0);
        // Serial.println("the color is");
       // Serial.println(res[x]);
      // res[x] = Strip.Color(bitLUT[((int)image[x][1] * image[x][1]) >> 8],bitLUT[((int)image[x][0] * image[x][0]) >> 8],bitLUT[((int)image[x][2] * image[x][2]) >> 8]);
        Strip.setPixelColor(x,res[x]); 
      }  
    
  
    
    Strip.show();
    delay(10000);
    
    }
    if(digitalRead(buttonPin) == LOW && on){
           turnOff();
            for(int i=0;i<ledCount;i++ ){
              res[i] = Strip.Color(0,0,255);  
              Strip.setPixelColor(i,res[i]);  
           } 
           Strip.show();
           delay(10000);
            
      }
      
    if(on)
     visibleLeds += dt;
    else
     visibleLeds -= dt;
}
















  
