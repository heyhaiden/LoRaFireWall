/*
  FireAware: RECEIVER BEACON
  v2.0 
  3 January 2023
*/

#include <MKRWAN.h>
#include <LoRa.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoLowPower.h>

// Set up the MKR WAN 1310
LoRaModem modem;

#define PIXEL_PIN 3
#define NUM_PIXELS 8

Adafruit_NeoPixel pixels(NUM_PIXELS, PIXEL_PIN);

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  Serial.println("Initializing GONDOR Receiver");

  if (!LoRa.begin(868E6)) {
    Serial.println("LoRa Radio connection failed!");
    while (1);
  }

  Serial.println("Waiting for packets...");

  pixels.begin();

  delay(5000);
}

void loop() {

  parsePacket();
  
}

void parsePacket() {

  // Check if a packet has been received
  int packetSize = modem.parsePacket();

  if (packetSize) {
    // Read the data from the packet
    String receivedData;
    while (modem.available()) {
      receivedData += (char)modem.read();
    }

    // Check if the received data contains the string "fire"
    if (strstr(receivedData.c_str(), "FLAME")) {

      // Turn on the NeoPixel strip
      for (int i = 0; i < NUM_PIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 20, 0));
      }
      pixels.show();

      delay(5000);
    }
  } 
  
  else {
    // Put the device in deep sleep until a packet is received
    LowPower.sleep();
  }
}

void wakeUp() {
  // The device has been woken up by the interrupt
}