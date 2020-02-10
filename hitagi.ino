#include <SPI.h>
#include "Adafruit_ILI9341.h"
#include "Adafruit_seesaw.h"
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
#ifdef ARDUINO_AVR_ATmega1284
// read diagnostics (optional but can help debug problems)
  uint8_t x = hitagi::lcd.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = hitagi::lcd.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = hitagi::lcd.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = hitagi::lcd.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = hitagi::lcd.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
  
  Serial.println(F("Benchmark                Time (microseconds)"));
  delay(10);
  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen());
  delay(500);

  Serial.print(F("Text                     "));
  Serial.println(testText());
  delay(3000);

  Serial.print(F("Lines                    "));
  Serial.println(testLines(ILI9341_CYAN));
  delay(500);

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines(ILI9341_RED, ILI9341_BLUE));
  delay(500);

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(ILI9341_GREEN));
  delay(500);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(ILI9341_YELLOW, ILI9341_MAGENTA));
  delay(500);

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles(10, ILI9341_MAGENTA));

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles(10, ILI9341_WHITE));
  delay(500);

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles());
  delay(500);

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles());
  delay(500);

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects());
  delay(500);

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects());
  delay(500);
#endif 
}

void loop() {
#ifdef ARDUINO_AVR_ATmega1284
  for(uint8_t rotation=0; rotation<4; rotation++) {
    hitagi::lcd.setRotation(rotation);
    testText();
    delay(1000);
  }
#endif
}
#ifdef ARDUINO_AVR_ATmega1284
unsigned long testFillScreen() {
  unsigned long start = micros();
  hitagi::lcd.fillScreen(ILI9341_BLACK);
  yield();
  hitagi::lcd.fillScreen(ILI9341_RED);
  yield();
  hitagi::lcd.fillScreen(ILI9341_GREEN);
  yield();
  hitagi::lcd.fillScreen(ILI9341_BLUE);
  yield();
  hitagi::lcd.fillScreen(ILI9341_BLACK);
  yield();
  return micros() - start;
}

unsigned long testText() {
  hitagi::lcd.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  hitagi::lcd.setCursor(0, 0);
  hitagi::lcd.setTextColor(ILI9341_WHITE);  hitagi::lcd.setTextSize(1);
  hitagi::lcd.println("Hello World!");
  hitagi::lcd.setTextColor(ILI9341_YELLOW); hitagi::lcd.setTextSize(2);
  hitagi::lcd.println(1234.56);
  hitagi::lcd.setTextColor(ILI9341_RED);    hitagi::lcd.setTextSize(3);
  hitagi::lcd.println(0xDEADBEEF, HEX);
  hitagi::lcd.println();
  hitagi::lcd.setTextColor(ILI9341_GREEN);
  hitagi::lcd.setTextSize(5);
  hitagi::lcd.println("Groop");
  hitagi::lcd.setTextSize(2);
  hitagi::lcd.println("I implore thee,");
  hitagi::lcd.setTextSize(1);
  hitagi::lcd.println("my foonting turlingdromes.");
  hitagi::lcd.println("And hooptiously drangle me");
  hitagi::lcd.println("with crinkly bindlewurdles,");
  hitagi::lcd.println("Or I will rend thee");
  hitagi::lcd.println("in the gobberwarts");
  hitagi::lcd.println("with my blurglecruncheon,");
  hitagi::lcd.println("see if I don't!");
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = hitagi::lcd.width(),
                h = hitagi::lcd.height();

  hitagi::lcd.fillScreen(ILI9341_BLACK);
  yield();
  
  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) hitagi::lcd.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) hitagi::lcd.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  yield();
  hitagi::lcd.fillScreen(ILI9341_BLACK);
  yield();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) hitagi::lcd.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) hitagi::lcd.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  hitagi::lcd.fillScreen(ILI9341_BLACK);
  yield();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) hitagi::lcd.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) hitagi::lcd.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  hitagi::lcd.fillScreen(ILI9341_BLACK);
  yield();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) hitagi::lcd.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) hitagi::lcd.drawLine(x1, y1, x2, y2, color);

  yield();
  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = hitagi::lcd.width(), h = hitagi::lcd.height();

  hitagi::lcd.fillScreen(ILI9341_BLACK);
  start = micros();
  for(y=0; y<h; y+=5) hitagi::lcd.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) hitagi::lcd.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = hitagi::lcd.width()  / 2,
                cy = hitagi::lcd.height() / 2;

  hitagi::lcd.fillScreen(ILI9341_BLACK);
  n     = min(hitagi::lcd.width(), hitagi::lcd.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    hitagi::lcd.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = hitagi::lcd.width()  / 2 - 1,
                cy = hitagi::lcd.height() / 2 - 1;

  hitagi::lcd.fillScreen(ILI9341_BLACK);
  n = min(hitagi::lcd.width(), hitagi::lcd.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    hitagi::lcd.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    hitagi::lcd.drawRect(cx-i2, cy-i2, i, i, color2);
    yield();
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = hitagi::lcd.width(), h = hitagi::lcd.height(), r2 = radius * 2;

  hitagi::lcd.fillScreen(ILI9341_BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      hitagi::lcd.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = hitagi::lcd.width()  + radius,
                h = hitagi::lcd.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      hitagi::lcd.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = hitagi::lcd.width()  / 2 - 1,
                      cy = hitagi::lcd.height() / 2 - 1;

  hitagi::lcd.fillScreen(ILI9341_BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    hitagi::lcd.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      hitagi::lcd.color565(i, i, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = hitagi::lcd.width()  / 2 - 1,
                   cy = hitagi::lcd.height() / 2 - 1;

  hitagi::lcd.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    hitagi::lcd.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      hitagi::lcd.color565(0, i*10, i*10));
    t += micros() - start;
    hitagi::lcd.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      hitagi::lcd.color565(i*10, i*10, 0));
    yield();
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = hitagi::lcd.width()  / 2 - 1,
                cy = hitagi::lcd.height() / 2 - 1;

  hitagi::lcd.fillScreen(ILI9341_BLACK);
  w     = min(hitagi::lcd.width(), hitagi::lcd.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    hitagi::lcd.drawRoundRect(cx-i2, cy-i2, i, i, i/8, hitagi::lcd.color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = hitagi::lcd.width()  / 2 - 1,
                cy = hitagi::lcd.height() / 2 - 1;

  hitagi::lcd.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(hitagi::lcd.width(), hitagi::lcd.height()); i>20; i-=6) {
    i2 = i / 2;
    hitagi::lcd.fillRoundRect(cx-i2, cy-i2, i, i, i/8, hitagi::lcd.color565(0, i, 0));
    yield();
  }

  return micros() - start;
}
#endif
