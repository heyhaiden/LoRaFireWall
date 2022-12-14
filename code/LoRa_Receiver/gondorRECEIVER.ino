/*
  GONDOR: RECEIVER BEACON
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

#define PIN 3
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
  while (!Serial)
    ;

  Serial.println("Initializing GONDOR Receiver");

  if (!LoRa.begin(868E6)) {
    Serial.println("LoRa Radio connection failed!");
    while (1)
      ;
  }

  Serial.println("Waiting for packets...");

  //Declare flame sensor pin
  //pinMode(flame_sensor_pin, INPUT);

  //Initialise the DHT sensor
  pinMode(DHTPin, INPUT);
  dht.begin();

  //Initialise LED
  pixels.begin();

  delay(5000);
}

void loop() {

  parsePacket();
  readDHT();

}

void parsePacket() {

  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}

void readDHT() {

  uint16_t Temperature = dht.readTemperature();
  uint16_t Humidity = dht.readHumidity();

  if ((Temperature <= 35) && (Humidity <= 50)) {

    for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...
      pixels.setPixelColor(i, pixels.Color(0, 20, 0));
    }

    pixels.show();  // Send the updated pixel colors to the hardware.

  }

  else {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(20, 20, 0));
    }
    pixels.show();  // Send the updated pixel colors to the hardware.

    //Serial.println("**Yellow Flag Warning**");
  }
}