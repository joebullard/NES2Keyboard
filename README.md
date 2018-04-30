# NES2Keyboard

Arduino Micro code for turning an NES controller into a USB keyboard

I had found tutorials on this which relied on Processing library/tool, but I found that kind of unwieldy, so I took advantage of the Arduino Micro's default keyboard library to turn each button on the controller into a keypress (e.g. pressing arrows on the controller sends arrow key presses to the computer). Emulators use keyboard input by default so this works quite nicely.
