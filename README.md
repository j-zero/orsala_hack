# orsala_hack
Alternative firmware for IKEA ORSALA lamp (https://www.ikea.com/de/de/p/orsala-arbeitsleuchte-led-dimmbar-weiss-00482904/)

# why
The ORSALA lamp from IKEA is a nice desktop lamp but has a very bad user experience. It features one rotary encoeder with button to control the brightness, color temperature and an on/off switch.
On the original firmware the following functions are implemented: 

## If lamp is turned off: 
  * Short press the encoder to turn on the lamp.
## If lamp is turned on:
 ** Press the encoder for about 750ms to turn off the lamp.
 ** Press the encoder to switch between brightness and temperature control.
 ** Turn the encoder in to change brightness or color temperature according to the current mode.


# information
The IKEA ORSALA lamp contains a STM8S003F3P6 cpu and two TTP932 led drivers for the warm and cold white leds.

# pinout

```
PD4   internal led
PA3   warm white leds
PD3   cold white leds
PA1   encoder pin 1
PA2   encoder pin 2
PB5   encoder button
```

# requirements
https://tenbaht.github.io/sduino/
