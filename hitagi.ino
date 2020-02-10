#include <SPI.h>
#include "Adafruit_ILI9341.h"
#include "hitagi.h"

void setup() {
    Serial.begin(9600);
    Serial.println("Starting up....");
    hitagi::setup([]() {
                Serial.println("Could not initialize SD Card!");
                delay(1000);
                });
    hitagi::lcd.setRotation(1);
    hitagi::lcd.fillScreen(ILI9341_BLACK);
    Serial.println("Startup complete!");
}

void loop() {

}
