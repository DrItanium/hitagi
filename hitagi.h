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

#ifndef HITAGI_H__
#define HITAGI_H__

#include "Arduino.h"
#include "libbonuspin.h"
#include "Adafruit_ILI9341.h"
#include <SPI.h>
#include <SD.h>

namespace hitagi {
    constexpr auto LED0 = 10; 
    constexpr auto LED1 = 9; 
    constexpr auto D4 = 4;
    constexpr auto PWM0 = 6;
    constexpr auto PWM1 = 5;
    constexpr auto LCD_DC = 7;
    constexpr auto LCD_RESET = 8;
    constexpr auto LCD_CS = A0;
    constexpr auto SDCS = A1;
    constexpr auto SPI0Enable = A3;
    constexpr auto SPI1Enable = A2;
    template<uint8_t pin>
    using SPIActivator = bonuspin::HoldPinLow<pin>;

    extern Adafruit_ILI9341 lcd;
    template<uint8_t pin>
    inline void setLEDIntensity(uint8_t intensity, int delayAmount = 0) noexcept {
        bonuspin::emitIntensity<pin>(intensity, bonuspin::AnalogWrite_t{});
        if (delayAmount > 0) {
            delay(delayAmount);
        }
    }

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

    template<typename SDFailFn>
    bool setup(SDFailFn onSD) noexcept {
        bool outcome = true;
        SPI.begin();
        if (!SD.begin(SDCS)) {
            outcome = false;
            onSD();
        }
        // setup the lcd as well
        lcd.begin();

        // now pulse the led
        cycleLED<LED0>();
        cycleLED<LED1>();
        return outcome;
    }


} // end namespace hitagi


#endif // end HITAGI_H__
