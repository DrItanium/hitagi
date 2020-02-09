/*
 * Hitagi
 * Copyright (c) 2020, Joshua Scoggins
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "hitagi.h"
#include <SPI.h>

namespace hitagi {
    SRAM& sram = SRAM::instance();
    Adafruit_ILI9341 lcd(LCD_CS, LCD_DC, LCD_RESET);
    Adafruit_seesaw soil0;
    SetupResult setup() {
        SetupResult ret;
        pinMode(SDCS, OUTPUT);
        digitalWrite(SDCS, HIGH);
        pinMode(SRAMEnable, OUTPUT);
        digitalWrite(SRAMEnable, HIGH);
        pinMode(LCD_CS, OUTPUT);
        digitalWrite(LCD_CS, HIGH);

        SPI.begin();
        if (!SD.begin(SDCS)) {
            ret.markSDInitFailed();
        }
        // setup the lcd as well
        lcd.begin();
        // then try and setup the soil sensor as well
        if (!soil0.begin(0x36)) {
            ret.markSoil0Failed();
        }

        // try doing a round trip test with the SRAM
        auto value = 0x51;
        auto address = 0x4FE;
        sram.write(address, value);
        if (auto result = sram.read(address); result != value) {
            ret.markSRAMFailed();
        }
        // now pulse the led
        for (int i = 0; i < 0x100; ++i) {
            analogWrite(LED0, i);
            delay(10);
        }
        for (int i = 0; i < 0x100; ++i) {
            analogWrite(LED0, 0xFF - i);
        }
        
        return ret;
    }
} // end namespace hitagi

