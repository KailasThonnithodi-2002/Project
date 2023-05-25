#include <Wire.h>

#define motor_address 9
#define answer_size 5

const int warning = D2;
const int low_moisture = D3;

// this is the maximum soil moisture reading which can be read
const int soil_max = 4000;
// the limit at which the soil moisture will cause the motor to run (in this case 10%)
uint8_t soil_threshold = 10; // 10% (changeable)
int second_delay = 10; // changebale (every time the plant soil is read)
uint8_t motor_duration = 4; // changeab;e (how long would the motor run for if soil is low)

int soil_threshold_function(String new_value) {
    soil_threshold = new_value.toInt();
    return -1;
}

int second_delay_function(String new_value) {
    second_delay = new_value.toInt();
    return -1;
}

int motor_duration_function(String new_value) {
    motor_duration = new_value.toInt();
    return -1;
}

// initailising all the pins 
int SOIL_PIN = A0;

void setup() {
    Particle.function("soil_threshold_function", soil_threshold_function);
    Particle.function("second_delay_function", second_delay_function);
    Particle.function("motor_duration_function", motor_duration_function);
    pinMode(warning, OUTPUT);
    pinMode(low_moisture, OUTPUT);
    Wire.begin();
}

void loop() {
    
    int soil_value = analogRead(SOIL_PIN);
    uint8_t soil_percentage = map(soil_value, 0, soil_max, 0, 100); // which can be sent as full number
    
    Serial.println(soil_threshold);
    Serial.println(second_delay);
    Serial.println(motor_duration);
    
    // Sending data to the Ardunio 
    Wire.beginTransmission(motor_address);
    Wire.write(soil_percentage);
    Wire.write(motor_duration);
    Wire.write(soil_threshold);
    Wire.endTransmission();
    
    // Conditions
    if (soil_percentage <= soil_threshold) { // if the lower, keep checking up based on the motor duration 
        digitalWrite(warning, LOW);
        digitalWrite(low_moisture, HIGH);
        delay(seconds(motor_duration));
    } 
    else if (soil_percentage - 5 <= soil_threshold) { // for example if the threshold was 15% and the soil moisture was 20%
        digitalWrite(warning, HIGH);
        digitalWrite(low_moisture, LOW);
        delay(seconds(second_delay));
    }
    else { // if normal, the continue with normal speed
        digitalWrite(warning, LOW);
        digitalWrite(low_moisture, LOW);
        delay(seconds(second_delay));
    }
}

int seconds(int second) {
  return second * 1000;
}
