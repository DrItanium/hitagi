#include <SPI.h>
#include "Adafruit_ILI9341.h"
#include "hitagi.h"

void setup() {
    Serial.begin(9600);
    Serial.println("Starting up....");
    auto result = hitagi::setup();
    if (result.sdInitFailed()) {
        Serial.println("Could not initialize SD Card!");
        delay(1000);
    } 
    if (result.soil0Failed()) {
        Serial.println("Could not initialize stemma soil sensor!");
        delay(1000);
    }
    if (result.sramInitFailed()) {
        Serial.println("SRAM initialization failed");
        Serial.println("Bad read/write test!");
        delay(1000);
    }
    hitagi::lcd.setRotation(1);
    hitagi::lcd.fillScreen(ILI9341_BLACK);
    Serial.println("Startup complete!");
}

void loop() {

}
