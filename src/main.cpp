#include <Arduino.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define DHTPIN A7
#define DHTTYPE DHT11

const char* ssid = "iPhone de Davide";
const char* pass = "jatedisse";
const char* brokerUser = "teensy";
const char* brokerPass = "";
const char* broker = "172.20.10.4";
const char* distanceTopic = "/sonar/distance";
const char* temperatureTopic = "/dht11/temperature";
const char* humidityTopic = "/dht11/humidity";

WiFiClient espClient;
PubSubClient  client(espClient);
long currentTime, lastTime; 
char messages[50];

DHT dht(DHTPIN, DHTTYPE);

int value = 0;
float time_val  = 0;
float distance = 0;
float temperature = 0;
float humidity = 0;

void setupWiFi(){
  delay(100);
  Serial.print("\nConnecting to");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print("-");
  }

  Serial.print("\nConnected to ");
  Serial.println(ssid);
}

void reconnect(){
  while (!client.connected()){
    Serial.print("\nConnecting to ");
    Serial.println(broker);
    if (client.connect("172.20.10.4", brokerUser, brokerPass)){
        Serial.print("\nConnected");
    }else{
      Serial.print("\nConnecting");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  setupWiFi();
  client.setServer(broker, 1883);
  
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
  if (!client.connected()){
    reconnect();
  }
  client.loop();
  currentTime = millis();
  if (currentTime - lastTime > 2000){
    snprintf(messages, 75, "%f", distance);
    client.publish(distanceTopic, messages);

    snprintf(messages, 75, "%f", temperature);
    client.publish(temperatureTopic, messages);

    snprintf(messages, 75, "%f", humidity);
    client.publish(humidityTopic, messages);

    lastTime = millis();
  }
  delay(50);
}
