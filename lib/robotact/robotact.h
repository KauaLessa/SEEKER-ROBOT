#ifndef ROBOT_H
#define ROBOT_H

#include <stdint.h>
#include "motor.h"
#include "ultrasnd.h"

enum State {SEEK, FOWARD, STOP}; 

bool seek(
    struct Motor motorA,
    struct Motor motorB, 
    struct Ultrasnd eyes,
    uint8_t speed,
    unsigned int trashold,
    unsigned int range
);

bool isInRange(int distance, int range); 

void moveRobotFoward(struct Motor motorA, struct Motor motorB, uint8_t speed);

void stopRobot(struct Motor motorA, struct Motor motorB); 

#endif