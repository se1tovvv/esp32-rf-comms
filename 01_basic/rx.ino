//rx 
#include <SPI.h>
#include <RF24.h>


RF24 radio (4,5);
const byte adress [] = "00001";

void setup() {
 Serial.begin(115200);
 radio.begin();
 radio.openReadingPipe(adress);
 radio.setPALevel(RF24_PA_LOW);
 radio.startListening();

}

void loop() {
if (radio.available()){
 char text [32] = "";
 radio.read(text, sizeof(text));
 Serial.print("Received: ")
 Serial.print(text);
}
}
