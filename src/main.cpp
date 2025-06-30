#include <Arduino.h>
#include <stdint.h>
#include "motor.h"
#include "ultrasnd.h"
#include "robotact.h"
#include "pins.h"

void setup() {
  pinMode(H_OUT_A_PIN, OUTPUT);
  pinMode(H_OUT_B_PIN, OUTPUT);
  pinMode(H_OUT_C_PIN, OUTPUT);
  pinMode(H_OUT_D_PIN, OUTPUT);
  pinMode(HC_TRIG_PIN, OUTPUT);
  pinMode(HC_ECHO_PIN, INPUT);
  pinMode(SEEK_LED, OUTPUT);
  pinMode(RETREAT_LED, OUTPUT); 
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  Serial.begin(9600); 
}

bool detectBorder() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    return true; 
  } 
  return false; 
}

State current_state = SEEK; 

void loop() {
  const unsigned int range = 50; // The robot will detect objects within this range
  const uint8_t robot_speed = 150;
  const unsigned int trashold = 10; 
  struct Motor motorA = createMotor(H_AB_ENABLE_PIN, H_OUT_A_PIN, H_OUT_B_PIN);
  struct Motor motorB = createMotor(H_CD_ENABLE_PIN, H_OUT_C_PIN, H_OUT_D_PIN);
  struct Ultrasnd eyes = createUltrasndSensor(HC_TRIG_PIN, HC_ECHO_PIN);

  switch (current_state) {
    case SEEK: {
      Serial.println("seek"); 
      bool object_found = seek(motorA, motorB, eyes, robot_speed, trashold, range);
      digitalWrite(SEEK_LED, object_found ? HIGH : LOW);
      current_state = object_found ? FOWARD : SEEK; 
      break;
    }
    case FOWARD: {
      Serial.println("foward"); 
      if (detectBorder()) {
        current_state = RETREAT; 
      } else if (isInRange(getDistance(eyes, trashold), range)) {
        moveRobotFoward(motorA, motorB, robot_speed); 
        current_state = FOWARD; 
      } else {
        current_state = SEEK; 
      }
      break; 
    }
    case RETREAT: {
      Serial.print("retreat");
      digitalWrite(RETREAT_LED, HIGH); 
      retreatRobot(motorA, motorB, robot_speed, 450); 
      digitalWrite(RETREAT_LED, LOW); 
      current_state = SEEK;
      break; 
    }
    default: {
      Serial.println("stop");
      current_state = STOP;
      stopRobot(motorA, motorB);
      break;  
    }
  }
}