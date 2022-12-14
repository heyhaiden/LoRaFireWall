/*
  GONDOR: TRANSMITTER BEACON
  v1.0 
  8 December 2022
*/

#include <MKRWAN.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <LoRa.h>
#include <ArduinoLowPower.h>
#include <DHT.h>
#include <DHT_U.h>

LoRaModem modem;

#include "arduino_secrets.h"
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

#define DHTTYPE DHT22  // DHT 22  (AM2302), AM2321
#define PIN 2
#define NUMPIXELS 8

Adafruit_NeoPixel pixels(NUMPIXELS, PIN);

// Flame Sensor setup
int flame_sensor_pin = 4;  // initializing pin 2 as the sensor output pin
//int flame_pin;             // state of sensor

// DHT Sensor setup
uint8_t DHTPin = 5;        // on Pin 12 of the board
DHT dht(DHTPin, DHTTYPE);  // Initialize DHT sensor.

int counter = 0;
String message;

float Temperature;
float Humidity;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Initializing GONDOR Transmitter");

  // LoRa Radio setup
  if (!LoRa.begin(868E6)) {
    Serial.println("LoRa Radio connection failed!");
    while (1);
  }

  //Declare flame sensor pin
  pinMode(flame_sensor_pin, INPUT);

  //Initialise the DHT sensor
  pinMode(DHTPin, INPUT);
  dht.begin();

  //Initialise LED
  pixels.begin();

  delay(5000);
}

void loop() {

  // Reset Neopixels
  pixels.clear();

  uint16_t Temperature = dht.readTemperature();
  uint16_t Humidity = dht.readHumidity();
  uint16_t flame_pin = digitalRead(flame_sensor_pin);


  if ((Temperature <= 35) && (Humidity <= 50)) {

    for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...

      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(0, 20, 0));
    }

    pixels.show();  // Send the updated pixel colors to the hardware.

    Serial.println("**GOOD CONDITIONS**");

    delay(1000);

    Serial.print("Temp: ");
    Serial.println(Temperature);  // Gets the values of the temperature
    Serial.print("Hum: ");
    Serial.println(Humidity);  // Gets the values of the humidity

    delay(5000);

    //GoToSleep();
    
  }

  else {

    if (flame_pin == HIGH) {

      Serial.println("**FLAME DETECTED**");

      // Turn on LED Alarm
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(20, 0, 0));
      }
      pixels.show();  // Send the updated pixel colors to the hardware.

      // Initialize Packet
      Serial.print("Sending packet: ");
      Serial.println(counter);

      // Send packet
      LoRa.beginPacket();
      LoRa.print("| ***FLAME DETECTED** | Temp: ");
      LoRa.print(Temperature);
      LoRa.print(" C | Hum: ");
      LoRa.print(Humidity);
      LoRa.print(" % |");
      LoRa.endPacket();

      counter++;
      delay(10000);

    }

    else {
       for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(20, 20, 0));
      }
      pixels.show();  // Send the updated pixel colors to the hardware.

      Serial.println("**YELLOW FLAG WARNING**");

      Serial.print("Sending packet: ");
      Serial.println(counter);

      // Send packet
      LoRa.beginPacket();
      LoRa.print("| YELLOW FLAG WARNING | Temp: ");
      LoRa.print(Temperature);
      LoRa.print(" C | Hum: ");
      LoRa.print(Humidity);
      LoRa.print(" % |");
      LoRa.endPacket();

      counter++;
      delay(5000);

      //GoToSleep();

    }
  }

  //wakeUp();
}

// Deep Sleep Task
void GoToSleep() {
  Serial.println(F("GONDOR Sender - Entering Deep Sleep"));
  LowPower.deepSleep(10000);
}

void wakeUp() {
  Serial.begin(9600);
  Serial.println("Waking Up GONDOR Sender");

  // LoRa Radio setup
  if (!LoRa.begin(868E6)) {
    Serial.println("GONDOR LoRa Radio connection failed!");
    while (1)
      ;
  }
}
