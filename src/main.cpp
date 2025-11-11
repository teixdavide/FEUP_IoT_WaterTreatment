#include <Arduino.h>
int value = 0;
int pressure = 0;
void setup() {
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A6, INPUT);
}

void loop() {
  if(value % 3 == 0){
    analogWrite(A1, 255);
    analogWrite(A2, 0);
    analogWrite(A3, 0);
    value++;
  }
  else if(value % 3 == 1){
    analogWrite(A1, 0);
    analogWrite(A2, 255);
    analogWrite(A3, 0);
    value++;
  }
  else if(value % 3 == 2){
    analogWrite(A1, 0);
    analogWrite(A2, 0);
    analogWrite(A3, 255);
    value = 0;
  }
  pressure = analogRead(A6);
  Serial.println(pressure);  
  delay(1000);
}