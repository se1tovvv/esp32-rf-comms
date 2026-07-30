//rx side of bydirectional
#include <SPI.h>
#include <RF24.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

//change the settings depending on your dimesnions and pins
#define OLED_WIDTH 128
#define OLED_HEIGHT 32
#define OLED_RESET -1
#define SDA_PIN 4
#define SCL_PIN 5
#define CE_PIN 2
#define CSN_PIN 15

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

RF24 radio(CE_PIN, CSN_PIN);
const byte addresses[][6] = {"00001", "00002"};


unsigned long packetsCount = 0;
unsigned long lastPacketTime = 0;

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Oled display initialization failed.");
    while (true) { delay(100); }
  }

  Serial.println("Oled initialization successfull.");
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("OLED initialized");
  display.display();
  delay(500);


  if (!radio.begin()) {
    Serial.println("Radio initialization failed.");
    while (true) {
      delay(1000);
    }
  }
  radio.openReadingPipe(1, addresses[0]);
  radio.openWritingPipe(addresses[1]);

  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
  Serial.print("RX Initialized successfully");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("NRF RX initialized");
  display.display();
  delay(500);
}


void loop() {

  if (radio.available()) {
    char text [32] ="";
    radio.read(&text, sizeof(text));
    Serial.print(text);
    delay(1000);
    radio.stopListening();
    const char newtext [] = "2";
    radio.write(&newtext, sizeof(newtext));
    radio.startListening();
  }

 
  
}
