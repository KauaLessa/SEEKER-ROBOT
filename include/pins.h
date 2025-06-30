#ifndef PINS_H
#define PINS_H

#include <stdint.h>

// DC motors pinout
const uint8_t H_AB_ENABLE_PIN = 11;  // PWM
const uint8_t H_CD_ENABLE_PIN = 10;  // PWM
const uint8_t H_OUT_A_PIN = 13;
const uint8_t H_OUT_B_PIN = 12;
const uint8_t H_OUT_C_PIN = 8;
const uint8_t H_OUT_D_PIN = 7;

// HC-SR04 pinout
const uint8_t HC_TRIG_PIN = 4;
const uint8_t HC_ECHO_PIN = 2;

const uint8_t SEEK_LED = 6; // On when object is found
const uint8_t RETREAT_LED = 5; // On when robot is iin RETREAT state
const uint8_t BUTTON_PIN = 9; // When pressed robot activates RETREAT state

#endif