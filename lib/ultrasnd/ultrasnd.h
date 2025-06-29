#ifndef ULTRASND_H
#define ULTRASND_H

#include <Arduino.h>
#include <stdint.h>

struct Ultrasnd {
  uint8_t trig_pin;
  uint8_t echo_pin;
};

struct Ultrasnd createUltrasndSensor(uint8_t trig_pin, uint8_t echo_pin);

void trigger(struct Ultrasnd sensor, unsigned int trashold);

/* returns 0 if signal takes too long */
unsigned long echo(struct Ultrasnd sensor, unsigned int trashold);

/* Returns distance in cm */
int getDistance(struct Ultrasnd sensor, unsigned int trashold);

#endif