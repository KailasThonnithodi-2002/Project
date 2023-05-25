// Arduino is the the master
#include <Wire.h>
#include <BH1750.h>

#define motor_address 9
#define motor_pin 3

void setup() {
  pinMode(motor_pin, OUTPUT);
  digitalWrite(motor_pin, LOW);
  Wire.begin(motor_address);
  Serial.begin(9600);
}

uint8_t soil_percentage;
uint8_t motor_duration;
uint8_t soil_threshold;

void receiveEvent() {

  while (Wire.available()) {
    soil_percentage = Wire.read();
    motor_duration = Wire.read();
    soil_threshold = Wire.read();
  }

  // Seri.al.println(soil_percentage);
  // Serialprintln(motor_duration);
  // Serial.println(soil_threshold);
  // Serial.println();
}

int seconds(int second) {
  return second * 1000;
}

void loop() {

  Wire.onReceive(receiveEvent);                                                                     
  // if soil moisture less than the threshold, then excrete
  if (soil_percentage < soil_threshold) {
    digitalWrite(motor_pin, HIGH);
    delay(seconds(motor_duration));
    digitalWrite(motor_pin, LOW);
  }
}  


