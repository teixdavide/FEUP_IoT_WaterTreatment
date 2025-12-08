#include <Arduino.h>

int value = 0;
int height = 0;
int distance = 0;

void setup() {
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A6, INPUT);
  pinMode(D3, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  digitalWrite(D3,HIGH);
  delay(10);
  digitalWrite(D3,LOW);

  height = analogRead(A0);
  distance = (height*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);
}