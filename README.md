# orsala_hack
Alternative firmware for IKEA ORSALA lamp (https://www.ikea.com/de/de/p/orsala-arbeitsleuchte-led-dimmbar-weiss-00482904/)

## but why?
The ORSALA lamp from IKEA is a nice desktop lamp but has a very strange user experience. It features one rotary encoeder with button to control the brightness, color temperature and an on/off switch.

### On the original firmware the following functions are implemented:

If lamp is turned off: 
 * Tap the encoder to turn on the lamp.

If lamp is turned on:
 * Press the encoder for about 750ms to turn off the lamp.
 * Tap the rotary encoder to switch between brightness and temperature control.
 * Turn the rotary encoder in to change brightness or color temperature according to the current mode.

Also the lamp does not save the brightness or color temperature on power loss.

### This firmware fixes the user experience in the following way:

If lamp is turned off: 
 * Tap the rotary encoder to turn on the Lamp
 * Turn the rotary encoder clockwise to turn on the lamp
 
If lamp is turned on:
 * Tap the rotary encoder to turn off the lamp
 * Turn rotary encoder to change the brightness
 * Turn the rotary encoder below minimum brightness to turn off the lamp
 * Press and turn the rotary encoder to change color temperature
 * Internal LED flashes if minimum/maximum temperature/brightness is reached

The current color and brightness is saved in EEPROM on power off the lamp.

## Information
The IKEA ORSALA lamp contains a STM8S003F3P6 (https://www.st.com/resource/en/datasheet/stm8s003f3.pdf) cpu and two TTP932 led drivers for the warm and cold white leds.
The SWIM Interface is lead out to pads with 2mm spacing:

![SWIM Interface](https://datenpir.at/images/orsala_hack/SWIM_Interface.jpg)
![CPU](https://datenpir.at/images/orsala_hack/CPU.jpg)

```
+    VSS     (VCC)      3.3V on STLINKv2
R    NRST    (Reset)    RST  on STLINKv2
S    SWIM    (SWIM)     SWIM on STLINKv2
-    VDD     (GND)      GND  on STLINKv2
```

## Pinout

```
PD4   internal led
PA3   warm white leds (via TTP932)
PD3   cold white leds (via TTP932)
PA1   encoder pin 1
PA2   encoder pin 2
PB5   encoder button
```

## Requirements
### Build
Sduino Arduino API Ports to STM8S https://tenbaht.github.io/sduino/

### Flash
stm8flash (https://github.com/tenbaht/stm8flash/)

STLINKv2 (https://www.st.com/en/development-tools/st-link-v2.html)

## Flash firmware

* Connect your STLINKv2 to the pads on the pcb (VCC is not needed if lamp is powered)
* Flash firmware:
```bash
./stm8flash -c stlinkv2 -p stm8s003f3 -w orsala_hack.hex
```
