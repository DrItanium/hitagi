The hitagi board is a platform designed around a 5v Sparkfun Pro Micro (or
compatible clone) which provides many expansion capabilities and features.

This includes:

- RGB Backlight Character LCD Display controlled via MCP23S17
  - R, G, and B are connected to separate PWM pins on the pro micro
- MCP23S17 used to provide some extra GPIOs and control the Character LCD (tied
		into a 74HC138)
- 1x 23LC1024 1MB SRAM (128 kbytes * 8) (tied into a 74HC138 
- 3x SPI breakout headers in the style of Adafruit Stemma connectors (just with
	more pins)
- 4x Adafruit STEMMA 4-pin connectors
- 4x Adafruit STEMMA/Neopixel 3-pin headers
  - Only one is actually a pwm header
- 9V battery holder (technically optional but it is wired into a toggle switch)


the original design of the board was meant for working with the stemma soil
sensor but it has evolved since then to have more features. The most important
of this being that you can drive up to four soil sensors at once

This project contains the hardware as well as the example library code to work
with the "platform."
