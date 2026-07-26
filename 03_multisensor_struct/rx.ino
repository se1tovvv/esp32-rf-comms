//rx side of multisensot wireless radio transmission
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
const byte address[] = "00001";

//keep the struct the same with tx if you wish to add another sensor
struct ClimateData {
  int flame;
  int light;
  float temperature;
  float humidity;
  int gas;
};

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
  radio.openReadingPipe(0, address);
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
    ClimateData climate;
    radio.read(&climate, sizeof(climate));
    packetsCount++;
    lastPacketTime = millis();

    //Serial debug
    Serial.print("Number of packets received:");
    Serial.println(packetsCount);

    Serial.print("Temperature: ");
    Serial.println(climate.temperature);

    Serial.print("Humidity: ");
    Serial.println(climate.humidity);

    Serial.print("Flame: ");
    Serial.println(climate.flame);

    Serial.print("Gas: ");
    Serial.println(climate.gas);

    Serial.print("Light: ");
    Serial.println(climate.light);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("T:");
    display.print(climate.temperature);
    display.print("H:");
    display.print(climate.humidity);
    display.println("%");

    if (climate.flame < 50) {
      display.println("No flame ");
    } else {
      display.println("Flame !");
    }

    if (climate.gas < 3000) {
      display.println("No Gas Leak");
    } else {
      display.println("Gas Leak!");
    }

    display.print("L:");
    display.print(climate.light);

    display.display();
  }
  delay(1000);

  //if you see error message here, make sure the TX is correctly wired and turned on.
  if (millis() - lastPacketTime > 3000) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("NO DATA FROM TX");
    display.println("Check TX / NRF24");
    display.display();
  }
}
