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
#include "libbonuspin/libbonuspin.h"

namespace hitagi {
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
    inline void setSPIAddress(byte value) noexcept {
        digitalWrite(SPIDecoderA0, value & 0b001 ? HIGH : LOW);
        digitalWrite(SPIDecoderA1, value & 0b010 ? HIGH : LOW);
        digitalWrite(SPIDecoderA2, value & 0b100 ? HIGH : LOW);
    }
    inline void selectSPIDevice() noexcept {
        digitalWrite(SPIDecoderEnable, LOW);
    }
    inline void deselectSPIDevice() noexcept {
        digitalWrite(SPIDecoderEnable, HIGH);
    }
    class GPIOExpander : public GenericMCP23S17<0b000> {
        public:
            // note that the interrupt lines are _NOT_ connected in REV2
            // hardware and probably will never be until I understand how to
            // wire up the interrupts correctly!
            using Self = GPIOExpander;
            static GPIOExpander& instance() noexcept;
        private:
            GPIOExpander() = default;
        public:
            Self& operator=(const Self&) = delete; 
            Self& operator=(Self&&) = delete; 
            GPIOExpander(const Self&) = delete;
            GPIOExpander(Self&&) = delete;
            ~GPIOExpander() override = default;
            void enableCS() noexcept override {
                setSPIAddress(MCP23S17Enable);
                digitalWrite(SPIDecoderEnable, LOW);
            }
            void disableCS() noexcept override {
                digitalWrite(SPIDecoderEnable, HIGH);
            }
    };

    class SRAM final {
        public:
            using Interface = bonuspin::sram::microchip::series_23lcxx::Device_23LC1024;
            static SRAM& instance() noexcept;
        private:
            SRAM() = default;

    }
    // All of the used pins
    // there are unused pins attached to the MCP23S17 and those will need
    // to be decoded as well
} // end namespace hitagi

#endif // end HITAGI_H__
