#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

struct Motor {
  uint8_t inVolt;
  uint8_t inA;
  uint8_t inB;
};

struct Motor createMotor(uint8_t inVolt, uint8_t inA, uint8_t inB); 

void foward(struct Motor motor, uint8_t speed); 

void backward(struct Motor motor, uint8_t speed);

void stop(struct Motor motor); 

#endif