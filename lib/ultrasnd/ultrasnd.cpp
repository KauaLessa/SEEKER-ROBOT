#include "ultrasnd.h"
#include <Arduino.h>

struct Ultrasnd createUltrasndSensor(uint8_t trig_pin, uint8_t echo_pin) {
  struct Ultrasnd sensor;
  sensor.trig_pin = trig_pin;
  sensor.echo_pin = echo_pin;
  return sensor;
}

void trigger(struct Ultrasnd sensor, unsigned int trashold) {
  digitalWrite(sensor.trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor.trig_pin, HIGH);
  delayMicroseconds(trashold);
  digitalWrite(sensor.trig_pin, LOW);
}

unsigned long echo(struct Ultrasnd sensor, unsigned int trashold) {
  trigger(sensor, trashold);
  return pulseIn(sensor.echo_pin, HIGH);
}

int getDistance(struct Ultrasnd sensor, unsigned int trashold) {
  return echo(sensor, trashold) / 29 / 2;
}