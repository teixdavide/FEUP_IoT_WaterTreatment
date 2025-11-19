#include <Arduino.h>

int value = 0;
float time_val  = 0;
float distance = 0;

void setup() {
  Serial.begin(9600);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A6, INPUT);

  pinMode(D3, OUTPUT);
  pinMode(A0, INPUT);
  digitalWrite(D3,LOW);
}

void loop() {
  digitalWrite(D3,HIGH);
  delayMicroseconds(10);
  digitalWrite(D3,LOW);

  time_val = pulseIn(A0, HIGH);
  distance = (time_val * .0343)/2;
  Serial.print("Distance: ");
  if (distance < 10){
    analogWrite(A1, 255);
    analogWrite(A2, 0);
  }
  else{
    analogWrite(A1, 0);
    analogWrite(A2, 255);
  }
  Serial.println(distance);
  delay(100);
}