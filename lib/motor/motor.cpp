#include <Arduino.h>
#include "motor.h"

struct Motor createMotor(uint8_t inVolt, uint8_t inA, uint8_t inB) {
  Motor motor;
  motor.inVolt = inVolt;
  motor.inA = inA;
  motor.inB = inB;
  return motor;
}

void foward(struct Motor motor, uint8_t speed) {
  analogWrite(motor.inVolt, speed);
  digitalWrite(motor.inA, HIGH);
  digitalWrite(motor.inB, LOW);
}

void backward(struct Motor motor, uint8_t speed) {
  analogWrite(motor.inVolt, speed);
  digitalWrite(motor.inA, LOW);
  digitalWrite(motor.inB, HIGH);
}

void stop(Motor motor) {
  digitalWrite(motor.inA, LOW);
  digitalWrite(motor.inB, LOW);
}