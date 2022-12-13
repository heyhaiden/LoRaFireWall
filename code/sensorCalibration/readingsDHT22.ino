#include <DHT.h>
#include <DHT_U.h>
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// DHT Sensor setup
uint8_t DHTPin = 12;        // on Pin 12 of the board
DHT dht(DHTPin, DHTTYPE);   // Initialize DHT sensor.              

void setup() {
  // open serial connection
  Serial.begin(115200);
  delay(100);

  // start DHT sensor
  pinMode(DHTPin, INPUT);
  dht.begin();
}

void loop() {
  Serial.print("Temp: ");
  Serial.println(dht.readTemperature()); // Gets the values of the temperature
  Serial.print("Hum: ");
  Serial.println(dht.readHumidity()); // Gets the values of the humidity
  delay(10000);
}