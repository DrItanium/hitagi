#include "hitagi.h"

void setup() {
	hitagi::setup();
	Serial.begin(9600);

}

void loop() {
	Serial.println("Donuts");
}
