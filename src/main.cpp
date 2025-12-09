#include <Arduino.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define DHTPIN A7
#define DHTTYPE DHT11

#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988

const char* ssid = "iPhone de Davide";
const char* pass = "jatedisse";
const char* brokerUser = "teensy";
const char* brokerPass = "";
const char* broker = "172.20.10.4";
const char* distanceTopic = "sensors/distance";
const char* temperatureTopic = "sensors/temperature";
const char* humidityTopic = "sensors/humidity";

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
int buzzers = 1;

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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if((char)payload[0] == '1' & buzzers == 1){
    analogWrite(A5, 150);
  }
  else{
    analogWrite(A5, 0);
  }
}

void setup() {
  Serial.begin(9600);
  setupWiFi();
  client.setServer(broker, 1883);
  client.setCallback(callback);
  
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, INPUT);

  pinMode(D3, OUTPUT);
  pinMode(A0, INPUT);
  digitalWrite(D3, LOW);

  pinMode(A5, OUTPUT);

  dht.begin(); // Initialize DHT11

  client.subscribe("something");
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
  if(buzzers==0){
    analogWrite(A1,0);
    analogWrite(A2,0);
  }
  else{
    if (distance < 10) {
    analogWrite(A1, 255);
    analogWrite(A2, 0);
  } else {
    analogWrite(A1, 0);
    analogWrite(A2, 255);
  }
  }
}

void loop() {
  digitalRead(A4);
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
  /*
  analogWrite(A5, 0);
  delay(2000);
  analogWrite(A5, 75);
  delay(2000);
  analogWrite(A5, 150);
  delay(2000);
  */
  //delay(50);
}
