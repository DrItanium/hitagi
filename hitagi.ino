#include <SPI.h>
#include "Adafruit_ILI9341.h"
//#include "hitagi.h"

//hitagi::GPIOExpander& gpio = hitagi::GPIOExpander::instance();
//hitagi::SRAM& sram;
//hitagi::Screen& lcd = hitagi::Screen::instance();
Adafruit_ILI9341 tft = Adafruit_ILI9341(A0, A3, A2);
void setup() {
    Serial.begin(9600);
    Serial.println("donuts");
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);
#if 0
    SPI.begin();
    pinMode(hitagi::SRAMEnable, OUTPUT);
    digitalWrite(hitagi::SRAMEnable, HIGH);
    pinMode(hitagi::MCP23S17Enable, OUTPUT);
    digitalWrite(hitagi::MCP23S17Enable, HIGH);
    hitagi::lcd.begin(16, 2);
    hitagi::lcd.setBacklightColor(255,255,255);
    hitagi::lcd.print("Donuts");
    delay(2000);
#endif
}
uint32_t address = 0u;
uint8_t value = 0u;
uint32_t rgb = 0u;
constexpr int16_t
  bits        = 20,   // Fractional resolution
  pixelWidth  = 320,  // TFT dimensions
  pixelHeight = 240,
  iterations  = 128;  // Fractal iteration limit or 'dwell'
float
  centerReal  = -0.6, // Image center point in complex plane
  centerImag  =  0.0,
  rangeReal   =  3.0, // Image coverage in complex plane
  rangeImag   =  3.0;
void loop() {
#if 0
    hitagi::lcd.setBacklightColor(rgb);
    hitagi::lcd.home();
    hitagi::SRAM::instance().write(address, value);
    hitagi::Screen::instance().home();
    Serial.print("Writing Address: ");
    Serial.print(address, HEX);
    Serial.print(", value: ");
    Serial.println(value, HEX);
    auto result = hitagi::SRAM::instance().read(address);
    Serial.print("Reading Address: ");
    Serial.print(address, HEX);
    Serial.print(", result: ");
    Serial.println(result, HEX);
    if (result == value) {
        hitagi::Screen::instance().print("RESULT: OK  ");
    } else {
        hitagi::Screen::instance().print("RESULT: FAIL");
    }
    ++address;
    ++value;
    hitagi::lcd.print("test!");
    ++rgb;
    rgb &= 0x00FF'FFFF;
    delay(1000);
#endif
int64_t       n, a, b, a2, b2, posReal, posImag;
  uint32_t      startTime,elapsedTime;


  int32_t
    startReal   = (int64_t)((centerReal - rangeReal * 0.5)   * (float)(1 << bits)),
    startImag   = (int64_t)((centerImag + rangeImag * 0.5)   * (float)(1 << bits)),
    incReal     = (int64_t)((rangeReal / (float)pixelWidth)  * (float)(1 << bits)),
    incImag     = (int64_t)((rangeImag / (float)pixelHeight) * (float)(1 << bits));
  
  startTime = millis();
  posImag = startImag;
  for (int y = 0; y < pixelHeight; y++) {
    posReal = startReal;
    for (int x = 0; x < pixelWidth; x++) {
      a = posReal;
      b = posImag;
      for (n = iterations; n > 0 ; n--) {
        a2 = (a * a) >> bits;
        b2 = (b * b) >> bits;
        if ((a2 + b2) >= (4 << bits)) 
          break;
        b  = posImag + ((a * b) >> (bits - 1));
        a  = posReal + a2 - b2;
      }
      #if defined(USE_BUFFER)
        buffer[y * pixelWidth + x] = (n * 29)<<8 | (n * 67);
      #else
        tft.drawPixel(x, y, (n * 29)<<8 | (n * 67)); // takes 500ms with individual pixel writes
      #endif
      posReal += incReal;
    }
    posImag -= incImag;
  }
  #if defined(USE_BUFFER)
    tft.drawRGBBitmap(0, 0, buffer, pixelWidth, pixelHeight); // takes 169 ms
  #endif
  elapsedTime = millis()-startTime;
  Serial.print("Took "); Serial.print(elapsedTime); Serial.println(" ms");

  rangeReal *= 0.95;
  rangeImag *= 0.95;
}
