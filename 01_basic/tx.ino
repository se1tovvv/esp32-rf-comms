#include <SPI.h>
#include <RF24.h>

RF24 radio (4,5);
const byte address [] = "00001";

void setup() {
 Serial.begin(115200);
 radio.begin();
 radio.openWritingPipe(address);
 radio.setPALevel(RF24_PA_LOW);
 radio.stopListening();

}

void loop() {
const char text [] = "Hello";
bool success = radio.write(&text, sizeof(text));
if(success) {
 Serial.println("Message sent")
} else{
 Serial.println("Message failed to send")
}
delay(1000);
}
