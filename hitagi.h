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
#include "Adafruit_seesaw.h"
#include <SPI.h>
#include <SD.h>

namespace hitagi {
    constexpr auto LED0 = 10; 
    constexpr auto D4 = 4;
    constexpr auto PWM0 = 6;
    constexpr auto PWM1 = 5;
    constexpr auto LCD_DC = 7;
    constexpr auto LCD_RESET = 8;
    constexpr auto BRIGHTNESS = 9;
    constexpr auto LCD_CS = A0;
    constexpr auto SDCS = A1;
    constexpr auto SRAMEnable = A2;
    constexpr auto SPI0Enable = A3;
    using SRAMActivator = bonuspin::HoldPinLow<SRAMEnable>;
    using SPI0Activator = bonuspin::HoldPinLow<SPI0Enable>;

    class SRAM final {
        private:
            enum class Opcodes : uint8_t {
                RDSR = 0x05,
                RDMR = RDSR,
                WRSR = 0x01,
                WRMR = WRSR,
                READ = 0x03,
                WRITE = 0x02,
                EDIO = 0x3B,
                EQIO = 0x38,
                RSTIO = 0xFF,
            };
        public:
            using Address = uint32_t;
            static SRAM& instance() noexcept {
                static SRAM _self;
                return _self;
            }
        private:
            void sendOpcode(Opcodes op) const noexcept {
                SPI.transfer(uint8_t(op));
            }
            void transferAddress(Address address) const noexcept {
                SPI.transfer(static_cast<uint8_t>(address >> 16));
                SPI.transfer(static_cast<uint8_t>(address >> 8));
                SPI.transfer(static_cast<uint8_t>(address));
            }
        public:
            uint8_t read(Address address) const noexcept {
                SRAMActivator activate;
                sendOpcode(Opcodes::READ);
                transferAddress(address);
                return SPI.transfer(0x00);
            }
            void write(Address address, uint8_t value) const noexcept {
                SRAMActivator activate;
                sendOpcode(Opcodes::WRITE);
                transferAddress(address);
                SPI.transfer(value);
            }
        private:
            SRAM() = default;
    };

    class SetupResult final {
        public:
            constexpr SetupResult() = default;
            void markSDInitFailed() noexcept { _sdFailed = true; }
            constexpr auto sdInitFailed() const noexcept { return _sdFailed; }
            void markSoil0Failed() noexcept { _soil0Failed = true; }
            constexpr auto soil0Failed() const noexcept { return _soil0Failed; }
            void markSRAMFailed() noexcept { _sramFailed = true; }
            constexpr auto sramInitFailed() const noexcept { return _sramFailed; }
        private:
            bool _sdFailed = false;
            bool _soil0Failed = false;
            bool _sramFailed = false;
    };

    SetupResult setup() noexcept;

    extern SRAM& sram;
    extern Adafruit_ILI9341 lcd;
    extern Adafruit_seesaw soil0;
} // end namespace hitagi


#endif // end HITAGI_H__
