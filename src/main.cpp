#include <Arduino.h>
#include <DHT.h>

#define DHTPIN A7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int value = 0;
float time_val  = 0;
float distance = 0;
float temperature = 0;
float humidity = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  pinMode(D3, OUTPUT);
  pinMode(A0, INPUT);
  digitalWrite(D3, LOW);

  dht.begin(); // Initialize DHT11
}

void read_distance(){
  digitalWrite(D3, HIGH);
  delayMicroseconds(10);
  digitalWrite(D3, LOW);

  time_val = pulseIn(A0, HIGH);
  distance = (time_val * 0.0343) / 2;

  Serial.print("Distance: ");
  Serial.println(distance);
}

void read_temperature(){
  temperature = dht.readTemperature();
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  else{
    Serial.print("Temperature: ");
    Serial.println(temperature);
  }
}

void read_humidity(){
  humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  else{
    Serial.print("Humidity: ");
    Serial.println(humidity);
  }
}

void logic_distance(){
  if (distance < 10) {
    analogWrite(A1, 255);
    analogWrite(A2, 0);
  } else {
    analogWrite(A1, 0);
    analogWrite(A2, 255);
  }
}

void loop() {
  read_distance();
  read_temperature();
  read_humidity();
  logic_distance();
  delay(50);
}
