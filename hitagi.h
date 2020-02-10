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
    constexpr auto SRAMEnable = A2;
    constexpr auto SPI0Enable = A3;
    template<uint8_t pin>
    using SPIActivator = bonuspin::HoldPinLow<pin>;

    template<uint8_t pin>
    class SRAM final {
        public:
            using Activator = SPIActivator<pin>;
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
            SRAM() {
                pinMode(pin, OUTPUT);
                digitalWrite(pin, HIGH);
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
            [[nodiscard]] uint8_t perform(Opcodes opcode, Address address, uint8_t value) const noexcept {
                Activator activate;
                sendOpcode(opcode);
                transferAddress(address);
                return SPI.transfer(value);
            }
        public:
            uint8_t read(Address address) const noexcept {
                return perform(Opcodes::READ, address, 0x00);
            }
            void write(Address address, uint8_t value) const noexcept {
                (void)perform(Opcodes::WRITE, address, value);
            }
    };

    class SetupResult final {
        public:
            constexpr SetupResult() = default;
            void markSDInitFailed() noexcept { _flags |= 0b0000'0001; }
            constexpr bool sdInitFailed() const noexcept { return _flags & 0b0000'0001; }
            void markSRAMFailed() noexcept { _flags |= 0b0000'0010; }
            constexpr bool sramInitFailed() const noexcept { return _flags & 0b0000'0010; }
        private:
            uint8_t _flags = 0u;
    };

    SetupResult setup() noexcept;

    template<uint8_t pin>
    inline void setLEDIntensity(uint8_t intensity, int delayAmount = 0) noexcept {
        bonuspin::emitIntensity<pin>(intensity, bonuspin::AnalogWrite_t{});
        if (delayAmount > 0) {
            delay(delayAmount);
        }
    }

    extern SRAM<SRAMEnable> sram;
    extern Adafruit_ILI9341 lcd;
} // end namespace hitagi


#endif // end HITAGI_H__
