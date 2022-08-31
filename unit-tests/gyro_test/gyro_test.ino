#include "Gyro.h"

    Gyro gyro(0,1);


void setup()
{
    Serial.begin(115200);
}


void  loop(){
  gyro.calculateCorrection();
  gyro.poll();
}
