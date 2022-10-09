
const int buttonPin = 5;     
const int ledPin =  27;       

int buttonState = 0;          

void setup() {
   Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);
  } else {
      Serial.println("heeereee1");
      Serial.println("heeereee2");
      Serial.println("heeereee3");
    digitalWrite(ledPin, LOW);
  }
}
