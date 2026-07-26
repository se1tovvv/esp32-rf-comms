//tx side of multisensot wireless radio transmission 
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include <DHT.h>

//DHT 11 settings
#define DHTPIN 26
#define DHTTYPE DHT11

//OLED settings
#define OLED_SCREEN_WIDTH 128
#define OLED_SCREEN_HEIGHT 64
#define OLED_SCREEN_RESET -1

//Pins
const int flamePin = 25;
const int lightPin = 35;
const int gasPin = 34;

//Communication address
const byte address[] = "00001";

unsigned long packetsCount = 0;
unsigned long failedPackets = 0;

//Creating objects
Adafruit_SSD1306 display(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, &Wire, OLED_SCREEN_RESET);
DHT dht(DHTPIN, DHTTYPE);
RF24 radio(4, 5);

//Data sent. You may add here as many sensors as you wish. Connect, define and include here.
struct ClimateData {
  int flame;
  int light;
  float temperature;
  float humidity;
  int gas;
};


void setup() {

  Serial.begin(115200);
  pinMode(flamePin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(gasPin, INPUT);

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

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  //Dht initialization
  dht.begin();

  //final message before loop
  display.clearDisplay();
  display.setCursor(20, 25);
  display.println("ALL SYSTEMS READY");
  display.display();
  delay(500);
}

void loop() {

  ClimateData climate;
  //Read all sensors
  climate.flame = analogRead(flamePin);
  climate.light = analogRead(lightPin);
  climate.temperature = dht.readTemperature();
  climate.humidity = dht.readHumidity();
  climate.gas = analogRead(gasPin);

  bool dhtError = isnan(climate.temperature) || isnan(climate.humidity);

  if (!dhtError) {
    bool success = radio.write(&climate, sizeof(climate));
    if (success) {
      packetsCount++;
      Serial.print("Number of packets sent:");
      Serial.println(packetsCount);
    } else {
      failedPackets++;
      Serial.print("Transmission failed. Total failures: ");
      Serial.println(failedPackets);
    }
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Temperature:");
    display.println(climate.temperature);
    display.print("Humidity:");
    display.print(climate.humidity);
    display.println("%");
    if (climate.flame < 50) {
      display.println("No flame danger");
    } else {
      display.println("Flame detected!");
    }
    if (climate.gas < 3000) {
      display.println("No gas leakage");
    } else {
      display.println("Gas leak!");
    }
    display.print("Value of light:");
    display.println(climate.light);
    display.display();
  } else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("DHT Reading Failed. Check Wiring!");
    display.display();
  }


  delay(1000);
}
