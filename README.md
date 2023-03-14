# FireWall: LoRa Telemetry System
Building a digital defensible space system using two Arduino MKRWAN 1310s

<p align="center">
  <img src="/assets/images/finishedDevices.jpg">
</p>

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

Calibrate the flame sensor using ```flameNeopixelAlert.ino```. The digital interface will send a HIGH signal when fire is detected by the sensor. Turn the potentiometer clock-wise to increase the detection threshold and counter-clockwise to decrease it. Keep in mind this sensor can be triggered by direct sunlight or a phone flashlight from close distances. If the flame is in-line with the sensor, it can detect from about 1.5m - 2m away.

This reading will then trigger a state change on the LED strip until environmental conditions change again.

<p align="center">
  <img src="/assets/images/alertStates.PNG">
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
| Adafruit_Neopixel.h | Controls neopixel strip | 
| LoRa.h |  Send messages over LoRa radio |
| ArduinoLowPower.h | Control idle and sleep settings for powering the board |
| DHT.h | Library for DHT22 sensor |
| DHT_U.h | Add'l library for DHT22 sensor |

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
Use an ```if``` statement to determine when environmental threshholds measured by the DHT22 sensor are crossed. Utilize the NeoPixel strip to visualize these alerts, and trigger a LoRa packet to send to your receiver device when red flag conditions are present and active flame detected. 

```
if ((temperature >= 35) && (humidity <= 15)) {

    // Change color on LED strip
    for (int i = 0; i < NUM_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(20, 20, 0));
    }
    pixels.show();

    // Display alert in serial monitor
    Serial.println("**YELLOW FLAG WARNING**");
    Serial.print("Sending packet: ");
    Serial.println(counter);

    // Send packet
    LoRa.beginPacket();
    LoRa.print("| YELLOW FLAG WARNING | Temp: ");
    LoRa.print(temperature);
    LoRa.print(" C | Hum: ");
    LoRa.print(humidity);
    LoRa.print(" % |");
    LoRa.endPacket();

    counter++;
      
```

#### Serial Monitor
The monitor will show current readings and whether an alert has been triggered. It will then send a numbered packet over LoRa.

<p align="center">
  <img src="/assets/images/Sender_Console.PNG">
</p>

### Receiver
In setup, initialize the LoRa radio using this code snippet:
```
Serial.println("Initializing GONDOR Sender");

  if (!LoRa.begin(868E6)) {
    Serial.println("LoRa Radio connection failed!");
    while (1);
  }
  ```
In order to receive a LoRa packet, use the following code:

```
void parsePacket() {

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Received packet '");

    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
```

You can also include a while statement to determine if a desired string is included in a packet. In this case, if ```"FLAME"``` is found, the device will change the color of the NeoPixel strip.

```
String receivedData;
    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }
    
    if (strstr(receivedData.c_str(), "FLAME")) {
    
      for (int i = 0; i < NUM_PIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 20, 0));
      }
      pixels.show();
    }  
```

#### Serial Monitor
Received Signal Strength Indication (RSSI) is the received signal power in milliwatts and is measured in dBm.

Values are expressed as a negative number. If RSSI=-30dBm: signal is strong. If RSSI=-120dBm: signal is weak.

<p align="center">
  <img src="/assets/images/Receiver_Console.PNG">
</p>


## Housing
<p align="center">
  <img src="/assets/images/fusion360_boxEnclosure.PNG">
</p>
Using MakerCase as a template, I designed the final casing in Fusion360 and used Rhino7 to laser cut and assemble the boxes using 3mm plywood.


## Future Improvements

- Security layer adding localAddress and destination bytes
- Connect The Things Stack to IFTTT and create automated notification applets 
- Add more devices and build a community mesh network

## Resources

- https://docs.arduino.cc/learn/communication/lorawan-101#arduino-libraries-for-lora-connectivity
- https://docs.arduino.cc/tutorials/mkr-wan-1310/the-things-network
- https://docs.arduino.cc/tutorials/mkr-wan-1310/lora-send-and-receive
- [The Limits of LoRaWAN in Event-Triggered Wireless Networked Control Systems](https://arxiv.org/pdf/2002.01472.pdf) (2020) Ivana Tomic, Laksh Bhatia, Michael J. Breza and Julie A. McCann
