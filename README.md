The hitagi board is a platform designed around an atmega1284p which provides
many expansion capabilities and features.

This includes:

- An adafruit 240x320 tft lcd with sd card socket
- 9V battery holder (technically optional but it is wired into a toggle switch)
- four LEDs hooked up to pwm pins


It has changed quite a bit since this repo started as the first boards I made
were too complex. I keep coming back to the 1284p because it has so much flash
and ram to make it trivial to implement as much functionality as I want. The
pro micro was originally the target but the library code was so extensive that
I was using at least 20kb of the 28 kb available when the soil sensor code was
included. Plus the need to reset the pro micro as it can't do it automatically
turned out to be a real problem.

