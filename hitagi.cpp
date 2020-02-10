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
    SRAM<SRAMEnable> sram;
    Adafruit_ILI9341 lcd(LCD_CS, LCD_DC, LCD_RESET);
    template<uint8_t pin>
    void cycleLED(int delay = 10) {
        for (int i = 0; i < 0x200; ++i) {
            if (i < 0x100) {
                setLEDIntensity<pin>(i, delay);
            } else {
                setLEDIntensity<pin>(0x1FF - i, delay);
            }
        }
    }
    inline void setupSPICSPin(int pin) noexcept {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }
    SetupResult setup() {
        SetupResult ret;

        SPI.begin();
        if (!SD.begin(SDCS)) {
            ret.markSDInitFailed();
        }
        // setup the lcd as well
        lcd.begin();

        // try doing a round trip test with the SRAM
        auto value = 0x51;
        auto address = 0x4FE;
        sram.write(address, value);
        if (auto result = sram.read(address); result != value) {
            ret.markSRAMFailed();
        }
        // now pulse the led
        cycleLED<LED0>();
        cycleLED<LED1>();
        return ret;
    }
} // end namespace hitagi

