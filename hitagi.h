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
#include "LiquidCrystal.h"
#include "Adafruit_ILI9341.h"
#include <SPI.h>

namespace hitagi {
    constexpr auto RED_PWM = 5;
    constexpr auto GREEN_PWM = 6;
    constexpr auto BLUE_PWM = 9;
    constexpr auto SRAMEnable = A0;
    constexpr auto MCP23S17Enable = A1;
    // These are unused pins which are broken out into STEMMA connectors
    constexpr auto Digital0 = 4;
    constexpr auto Analog6 = A6;
    constexpr auto Digital7 = 7;
    constexpr auto Digital2 = 8;
    //constexpr auto Analog8 = A8;
    constexpr auto PWM = 10;
    constexpr auto D10 = 10;
    class GPIOExpander : public bonuspin::GenericMCP23S17<0b000> {
        public:
            // note that the interrupt lines are _NOT_ connected in REV2
            // hardware and probably will never be until I understand how to
            // wire up the interrupts correctly!
            using Self = GPIOExpander;
            static GPIOExpander& instance() noexcept {
                static GPIOExpander _self;
                return _self;
            }
        private:
            GPIOExpander() = default;
        public:
            Self& operator=(const Self&) = delete; 
            Self& operator=(Self&&) = delete; 
            GPIOExpander(const Self&) = delete;
            GPIOExpander(Self&&) = delete;
            ~GPIOExpander() override = default;
            void enableCS() noexcept override {
                digitalWrite(MCP23S17Enable, LOW);
            }
            void disableCS() noexcept override {
                digitalWrite(MCP23S17Enable, HIGH);
            }
    };

    using SPIActivator = bonuspin::HoldPinLow<SRAMEnable>;

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
                SPIActivator activate;
                sendOpcode(Opcodes::READ);
                transferAddress(address);
                return SPI.transfer(0x00);
            }
            void write(Address address, uint8_t value) const noexcept {
                SPIActivator activate;
                sendOpcode(Opcodes::WRITE);
                transferAddress(address);
                SPI.transfer(value);
            }
        private:
            SRAM() = default;
    };

    class Screen final : public LiquidCrystal {
        public:
            static constexpr auto GPIO_RS = 9;
            static constexpr auto GPIO_RW = 10;
            static constexpr auto GPIO_EN = 11;
            static constexpr auto GPIO_A4 = 12;
            static constexpr auto GPIO_A5 = 13;
            static constexpr auto GPIO_A6 = 14;
            static constexpr auto GPIO_A7 = 15;
            static Screen& instance() noexcept {
                static Screen _self(GPIO_RS, GPIO_RW, GPIO_EN, GPIO_A4, GPIO_A5, GPIO_A6, GPIO_A7);
                return _self;
            }
            void setBacklightRedColor(int value) const noexcept;
            void setBacklightGreenColor(int value) const noexcept;
            void setBacklightBlueColor(int value) const noexcept;
            void setBacklightColor(int r, int g, int b) const noexcept;
            void setBacklightColor(uint32_t packedColor) const noexcept;
        protected:
            ~Screen() override = default;
            using LiquidCrystal::LiquidCrystal;
            void digitalWrite(int pin, int value) noexcept override {
                GPIOExpander::instance().digitalWrite(pin, value);
            }
            void pinMode(int pin, int mode) noexcept override {
                GPIOExpander::instance().pinMode(pin, mode);
            }
            // @todo add methods for setting pwm colors
    };
    inline void setupOutputPin(int pin) noexcept {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }
    template<typename ... Args>
    void setupOutputPins(Args&& ... pins) noexcept {
        (setupOutputPins(pins), ...);
    }
    extern SRAM& sram;
    extern GPIOExpander& gpio;
    extern Screen& lcd;
} // end namespace hitagi


#endif // end HITAGI_H__
