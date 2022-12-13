#include <MKRWAN.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 8

Adafruit_NeoPixel pixels(NUMPIXELS, PIN);

int flame_sensor_pin = 2;  // initializing pin 2 as the sensor output pin
int flame_pin;             // state of sensor

void setup() {
  pinMode(flame_sensor_pin, INPUT);  // declaring sensor pin as input pin for Arduino

  Serial.begin(9600);  // setting baud rate at 9600
  pixels.begin();
}

void loop() {
  pixels.clear();
  // Set all pixel colors to 'off'

  flame_pin = digitalRead(flame_sensor_pin);  // reading from the sensor

  if (flame_pin == HIGH)  // applying condition
  {
    Serial.println("FIRE DETECTED!");

    for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...

      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(20, 0, 0));
    }
    pixels.show();  // Send the updated pixel colors to the hardware.

    delay(5000);
  }

  else {
    Serial.println("No Fire");

    for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...

      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(0, 20, 0));
    }
    pixels.show();  // Send the updated pixel colors to the hardware.
  }

  delay(1000);
}