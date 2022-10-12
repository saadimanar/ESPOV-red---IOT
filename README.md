# ESPOV-red---IOT
Our project : Lightsaber
Our project is based on the aspect of "persisitance of vision".
Our lightsaber recieves images from mobile application via bluetooth or via a web app and it starts projecting the images once the lightsaber is moved, the images are changed when the button is pressed. 
the main hardware we used is: esp32, Led strip, motion sensor and button, when we move the motion sensor it calculates the current axis and according to it we calculate the angle of the motion, according to the angle we determine what pixels to show.

By : Manar Saadi , Adan Assli , Hadeel Arshed .

The git contains the following files :
   1.espov : it contains the arduino code.
   2.unit-tests : it contains the tests for each component in the hardware.
   3.application: it contains the code of the application in flutter.
   
The libraris that we use with esp32 :
   1.FastLED , Version 3.5.0
   2.vector , Version 
   3.BluetoothSerial , Version 
   4.esp_spiffs , Version
   5.Wire , Version 
