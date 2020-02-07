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

namespace hitagi {
    void Screen::setBacklightRedColor(int value) const noexcept {
        analogWrite(RED_PWM, value);
    }
    void Screen::setBacklightGreenColor(int value) const noexcept {
        analogWrite(GREEN_PWM, value);
    }
    void Screen::setBacklightBlueColor(int value) const noexcept {
        analogWrite(BLUE_PWM, value);
    }
    void Screen::setBacklightColor(int r, int g, int b) const noexcept {
        setBacklightRedColor(r);
        setBacklightGreenColor(g);
        setBacklightBlueColor(b);
    }
    void Screen::setBacklightColor(uint32_t packedColor) const noexcept {
        // format is 0rgb with r being bits 23-16
        setBacklightColor((0x00FF0000 & packedColor) >> 16,
                          (0x0000FF00 & packedColor) >> 8,
                          (0x000000FF & packedColor));
    }
    void setupOutputPin(int pin) noexcept {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }
    template<typename ... Args>
    void setupOutputPins(Args&& ... pins) noexcept {
        (setupOutputPins(pins), ...);
    }
    void setup() noexcept {
        SPI.begin();
        lcd.begin(16, 2);
        setupOutputPins(SPIDecoderA0, SPIDecoderA1, SPIDecoderA2, SPIDecoderEnable);
    }
    GPIOExpander& gpio = GPIOExpander::instance();
    SRAM& sram = SRAM::instance();
    Screen& lcd = Screen::instance();

} // end namespace hitagi

