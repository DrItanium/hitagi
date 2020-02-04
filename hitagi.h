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

namespace hitagi {
    // All of the used pins
    constexpr auto RED_PWM = 5;
    constexpr auto GREEN_PWM = 6;
    constexpr auto BLUE_PWM = 9;
    constexpr auto SPIDecoderA0 = A3;
    constexpr auto SPIDecoderA1 = A2;
    constexpr auto SPIDecoderA2 = A1;
    constexpr auto SPIDecoderEnable = A0;
    constexpr auto MCP23S17Enable = 0b111;
    constexpr auto SRAMEnable = 0b110;
    constexpr auto SPI0Enable = 0b000;
    constexpr auto SPI1Enable = 0b001;
    constexpr auto SPI2Enable = 0b010;
    // These are unused pins which are broken out into STEMMA connectors
    constexpr auto Digital0 = 4;
    constexpr auto Analog6 = A6;
    constexpr auto Digital7 = 7;
    constexpr auto Digital2 = 8;
    constexpr auto Analog8 = A8;
    constexpr auto PWM = 10;
    constexpr auto D10 = 10;
    // there are unused pins attached to the MCP23S17 and those will need
    // to be decoded as well
} // end namespace hitagi

#endif // end HITAGI_H__
