# GONDOR: LoRa Telemetry System
Simple fire detection system using two Arduino MKRWAN 1310s

## Overview
<p align="center">
  <img src="/assets/images/defensibleSpace.PNG">
</p>

This project was inspired by the idea of creating a digital defensible space in order to monitor environmental factors and detect early fire events. This build exposes the LoRa radio directly, and allows you to send data to any radios in range with the same radio parameters. All data is broadcasted and there is no addressing, which allows anyone to listen in.

## Hardware 

|     |     |  
| --- | --- | 
| _***Item***_ | _***Description***_ | 
| Arduino MKRWAN 1310 | The Arduino MKR WAN 1310 provides a practical and cost effective solution to add LoRa® connectivity to projects requiring low power.  | 
| Adafruit Neopixel Stick |  8 5050 (5mm x 5mm) smart RGB LEDs onto a PCB with mounting holes and a chainable design. | 
| [DHT22 Sensor](https://www.adafruit.com/product/385) | A capacitive humidity sensor and a thermistor to measure surrounding air |
| KY-026 Flame Sensor |  Detects infrared light emitted by fire, has both digital and analog outputs and a potentiometer to adjust the sensitivity |
| Antenna | xxx |
| 80x100 Solar Cell | xxx|
| Adafruit Universal USB / SOLAR / LIPO Charger | xxx |

## Schematics
<p align="center">
  <img src="/assets/fritzing/Fritzing_LoRaSender.PNG">
</p>
Breadboard Layout
<br>
<p align="center">
  <img src="/assets/fritzing/schematicFritzing.PNG">
</p>

## Code


## Housing
<p align="center">
  <img src="/assets/images/fusion360_boxEnclosure.PNG">
</p>
Using MakerCase as a template, I designed the final casing in Fusion360 and used Rhino7 to laser cut and assemble the boxes using 3mm plywood.


## Future Improvements

- Implementation of Low Power Library 
- Security layer adding localAddress and destination bytes
- Connect The Things Stack to IFTTT and create automated notification applets 
- Add more devices and build a community mesh network

## Resources
