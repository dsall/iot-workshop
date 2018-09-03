#include <IRremote.h>

IRsend irsend;

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(10000);
  //Power//
  Serial.println("Power");
  delay(10000);
  //Volume Up//
  for (int i = 0; i < 10; i++) {
    irsend.sendNEC(0xB5E9B811, 32); //Volume Up Code
    Serial.println("Volume Up");
    delay(250);
  }

}
