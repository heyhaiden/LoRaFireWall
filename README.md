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
| DHT22 Sensor | A capacitive humidity sensor and a thermistor to measure surrounding air |
| KY-026 Flame Sensor |  Detects infrared light emitted by fire, has both digital and analog outputs and a potentiometer to adjust the sensitivity |
| Antenna | Whip Omnidirectional Telemetry Antenna with SMA Connector |
| 80x100 Solar Cell | peak output, it produces 0.935W power with a peak voltage of 5.5V and a current of 170mA |
| Adafruit Universal USB / SOLAR / LIPO Charger | Solar Charger Battery Charger 3.7V, 4.2V 1.5A |

## Schematics
### Breadboard 
<p align="center">
  <img src="/assets/fritzing/Fritzing_LoRaSender.PNG">
</p>

### Circuit 

<p align="center">
  <img src="/assets/fritzing/schematicFritzing.PNG">
</p>

## Code
This is a two part system consisting of a transmitter, or sender, and a receiver. Communication can either go one direction or two ways, depending on where LoRa packets are included. 
### Libraries
|     |     |  
| --- | --- | 
| _***Item***_ | _***Description***_ | 
| MKRWAN.h | Library for MKRWAN boards  | 
| Adafruit_Neopixel.h | xxx | 
| SPI.h | xxx |
| LoRa.h |  xxx |
| LowPower.h | xxx |
| DHT.h | Library for DHT22 sensor |
| DHT_U.h | xxx |

### Sender
In setup, initialize the LoRa radio using this code snippet:
```
Serial.println("Initializing GONDOR Sender");

  // LoRa Radio setup
  if (!LoRa.begin(868E6)) {
    Serial.println("LoRa Radio connection failed!");
    while (1);
  }
  ```
Utilize the serial console to print current conditions of the DHT22 and determine if environmental threshholds are crossed, which then triggers a "yellow flag alert," activating the flame sensor to take time based snapshots.

```
if ((Temperature <= 35) && (Humidity <= 50)) {
  Serial.println("**GOOD CONDITIONS**");
  ...
}
else {
  if (flame_pin == HIGH) {
    Serial.println("**FLAME DETECTED**");
    ...
    }
    else {
    Serial.println("**YELLOW FLAG WARNING**");
    ...
    }
}
```
In order to send an alert over LoRa, initialize a packet using uint16_t variables and strings.

```
// Send packet

LoRa.beginPacket();
LoRa.print("| ***FLAME DETECTED** | Temp: ");
LoRa.print(Temperature);
LoRa.print(" C | Hum: ");
LoRa.print(Humidity);
LoRa.print(" % |");
LoRa.endPacket();
```
#### Serial Monitor
The monitor will show current readings and whether an alert has been triggered. It will then send a numbered packet over LoRa.

<p align="center">
  <img src="/assets/images/Sender_Console.PNG">
</p>

### Receiver



#### Serial Monitor
<p align="center">
  <img src="/assets/images/Receiver_Console.PNG">
</p>



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
