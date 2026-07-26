//tx side of single sensor project
#include <SPI.h>
#include <RF24.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>


RF24 radio(4,5);
const byte address [] = "00001";


Adafruit_SSD1306 display (128, 64 ,  &Wire , -1);


const int flamePin = 15;


void setup() {
 Serial.begin(115200);
  //pinMode
 pinMode(flamePin, INPUT);


 //radio
 radio.begin();
 radio.openWritingPipe(address);
 radio.setPALevel(RF24_PA_LOW);
 radio.stopListening();


 //display
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 display.setTextSize(1);
 display.setTextColor(SSD1306_WHITE);
}


void loop() {
 int flameValue = analogRead(flamePin);
 display.clearDisplay();
 display.setCursor(0,0);
 display.print("FlameValue:");
 display.println(flameValue);
 display.display();


 bool success = radio.write(flameValue, sizeof(flameValue));


 if(success) {
 Serial.println("Sent");
 } else{Serial.println("failed");}
 }
