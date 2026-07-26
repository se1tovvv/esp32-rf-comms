//rx
#include <SPI.h>
#include <RF24.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>


RF24 radio(4, 5);
const byte address[] = "00001";


Adafruit_SSD1306 display(128, 64, &Wire, -1);


void setup() {
  Serial.begin(115200);


  //radio
  radio.begin();
  radio.openReadingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();


  //display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}


void loop() {
  if (radio.available()) {
    int flameValue;
    radio.read(flameValue, sizeof(flameValue));
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(text);
    display.display();
    delay(1000);
  }
}
