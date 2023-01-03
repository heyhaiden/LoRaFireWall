/*
  FireAware: SENDER BEACON
  v2.0 
  3 January 2023
*/

#include <MKRWAN.h>
#include <LoRa.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoLowPower.h>
#include <DHT.h>
#include <DHT_U.h>

// Set up the MKR WAN 1310
LoRaModem modem;

#define DHT_TYPE DHT22
#define DHT_PIN 5
#define FLAME_PIN 4
#define PIXEL_PIN 2
#define NUM_PIXELS 8

Adafruit_NeoPixel pixels(NUM_PIXELS, PIXEL_PIN);


DHT dht(DHT_PIN, DHT_TYPE);

int counter = 0;

// Wake device up when DHT22 conditions change
//LowPower.attachInterruptWakeup(DHT_PIN, wakeUp, CHANGE);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Initializing GONDOR Sender");

  // LoRa Radio setup
  if (!LoRa.begin(868E6)) {
    Serial.println("LoRa Radio connection failed!");
    while (1);
  }

  pinMode(FLAME_PIN, INPUT);
  dht.begin();
  pixels.begin();

  // Set up the interrupt pin
  //pinMode(INTERRUPT_PIN, INPUT);

}

void loop() {

  // Turn on LED strip
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 20, 0));
  }
  pixels.show();

  // Take a reading from the DHT22 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

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
      delay(5000);

    // Take a reading from the flame sensor
    int flame = digitalRead(FLAME_PIN);

    if (flame == HIGH) {

      // Change color on LED strip
      for (int i = 0; i < NUM_PIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(20, 0, 0));
      }
      pixels.show();

      // Display alert in serial monitor
      Serial.println("**FLAME DETECTED**");
      Serial.print("Sending packet: ");
      Serial.println(counter);

      // Build and send the packet
      LoRa.beginPacket();
      LoRa.print("| ***FLAME DETECTED** | Temp: ");
      LoRa.print(temperature);
      LoRa.print(" C | Hum: ");
      LoRa.print(humidity);
      LoRa.print(" % |");
      LoRa.endPacket();

      counter++;
      delay(10000);
    }
  }

  // Put the device in deep sleep for 1 hour
  LowPower.sleep(3600);
}

void wakeUp() {
  // The device has been woken up by the interrupt
}