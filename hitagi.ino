#include <SPI.h>
#include "hitagi.h"

//hitagi::GPIOExpander& gpio = hitagi::GPIOExpander::instance();
//hitagi::SRAM& sram;
//hitagi::Screen& lcd = hitagi::Screen::instance();
void setup() {
    Serial.begin(9600);
    Serial.println("donuts");
    SPI.begin();
    hitagi::GPIOExpander::instance();
    hitagi::SRAM::instance();
    //hitagi::Screen::instance().begin(16,2);
    //hitagi::Screen::instance().setBacklightColor(255, 255, 255);
    //constexpr auto RED_PWM = 5;
    //constexpr auto GREEN_PWM = 6;
    //constexpr auto BLUE_PWM = 9;
}
uint32_t address = 0u;
uint8_t value = 0u;
uint32_t rgb = 0u;
void loop() {
    analogWrite(hitagi::RED_PWM, rgb & 0xFF);
    analogWrite(hitagi::GREEN_PWM, (rgb & 0xFF00) >> 8);
    analogWrite(hitagi::BLUE_PWM, (rgb & 0xFF0000) >> 16);
    hitagi::SRAM::instance().write(address, value);
    Serial.print("Writing Address: ");
    Serial.print(address, HEX);
    Serial.print(", value: ");
    Serial.println(value, HEX);
    auto result = hitagi::SRAM::instance().read(address);
    Serial.print("Reading Address: ");
    Serial.print(address, HEX);
    Serial.print(", result: ");
    Serial.println(result, HEX);
    ++address;
    ++value;
    ++rgb;
    rgb &= 0x00FF'FFFF;
    delay(10);
}
