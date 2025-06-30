#include "robotact.h"
#include "ultrasnd.h"
#include "motor.h"

bool isInRange(int distance, int range) {
  return (distance && distance <= range);
}

bool seek(
  struct Motor motorA,
  struct Motor motorB,
  struct Ultrasnd eyes,
  uint8_t speed,
  unsigned int trashold,
  unsigned int range
) {
  if (isInRange(getDistance(eyes, trashold), range)) {
    return true; 
  }
  foward(motorA, speed); 
  backward(motorB, speed); 
  return false; 
}

void moveRobotFoward(struct Motor motorA, struct Motor motorB, uint8_t speed) {
  foward(motorA, speed);
  foward(motorB, speed);
}

void moveRobotBackward(struct Motor motorA, struct Motor motorB, uint8_t speed) {
  backward(motorA, speed); 
  backward(motorB, speed); 
}

void stopRobot(struct Motor motorA, struct Motor motorB) {
  stop(motorA);
  stop(motorB);
}

void retreatRobot(
  struct Motor motorA, 
  struct Motor motorB, 
  uint8_t speed, 
  unsigned long retreat_time 
) {
  backward(motorA, speed); 
  backward(motorB, speed); 
  delay(retreat_time);
  stopRobot(motorA, motorB); 
}