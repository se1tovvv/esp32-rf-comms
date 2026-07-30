//tx side of multisensot wireless radio transmission 
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <SPI.h>
#include <RF24.h>

//OLED settings
#define OLED_SCREEN_WIDTH 128
#define OLED_SCREEN_HEIGHT 64
#define OLED_SCREEN_RESET -1

//Communication addresses
const byte addresses[][6] = {"00001", "00002"};

//Creating objects
Adafruit_SSD1306 display(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, &Wire, OLED_SCREEN_RESET);
RF24 radio(4, 5);

void setup() {

  Serial.begin(115200);

  //Debugging for display initialization. If you see fail message, check your wiring.
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.print("OLED Initialization failed. Check wiring!");
    while (true)
      ;
  } else {
    Serial.println("OLED Initialization successfull");
    display.clearDisplay();
    display.setCursor(25, 25);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println("OLED STATUS: OK");
    display.display();
    delay(500);
  }

  if (!radio.begin()) {
    Serial.println("NRF Initialization failed. Check Wiring!");
    display.clearDisplay();
    display.setCursor(25, 25);
    display.println("NRF24 ERROR");
    display.println("Check wiring");
    display.display();
    while (true)
      ;
  } else {
    Serial.print("NRF Initialization successfull");
    display.clearDisplay();
    display.setCursor(25, 25);
    display.println("NRF STATUS: OK");
    display.display();
    delay(500);
  }

  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  //final message before loop
  display.clearDisplay();
  display.setCursor(20, 25);
  display.println("ALL SYSTEMS READY");
  display.display();
  delay(500);
}

void loop() {
const char text [] = "1";
bool success = radio.write(&text, sizeof(text));
if (success) {
  Serial.print("Initial message 1 sent");
    display.clearDisplay();
    display.setCursor(25, 25);
    display.println("Message 1 sent");
    display.display();
} else {
  Serial.print("failed to send 1st message");
}
  radio.startListening();
  if(radio.available()) {
    char newtext [32] = "";
    radio.read(&newtext, sizeof(newtext));
    Serial.print("ACK RECEIVED:");
    Serial.print(newtext);
} else {
  Serial.print("rx failed");
}
delay(1000);

}
