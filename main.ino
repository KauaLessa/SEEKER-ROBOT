#include <Arduino.h>
#include <stdint.h>

// DC motors pinout
const uint8_t H_OUT_A_PIN = 9;
const uint8_t H_OUT_B_PIN = 10;
const uint8_t H_OUT_C_PIN = 11;
const uint8_t H_OUT_D_PIN = 12;

// HC-SR04 pinout
const uint8_t HC_TRIG_PIN = 4;
const uint8_t HC_ECHO_PIN = 2;

// Infrared sensors
const uint8_t IR_A = 7; 
const uint8_t IR_B = 8; 

const uint8_t SEEK_LED = 6; // On when object is found
const uint8_t RETREAT_LED = 5; // On when robot is in RETREAT state
// DEBUG const uint8_t BUTTON_PIN = 9; // When pressed robot activates RETREAT state

enum State {SEEK, FOWARD, STOP, RETREAT};

struct Ultrasnd {
  uint8_t trig_pin;
  uint8_t echo_pin;
};

struct Motor {
  uint8_t inA;
  uint8_t inB;
};

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

struct Motor createMotor(uint8_t inA, uint8_t inB) {
  struct Motor motor; 
  motor.inA = inA;
  motor.inB = inB;
  return motor;
}

void foward(struct Motor motor) {
  digitalWrite(motor.inA, HIGH);
  digitalWrite(motor.inB, LOW);
}

void backward(struct Motor motor) {
  digitalWrite(motor.inA, LOW);
  digitalWrite(motor.inB, HIGH);
}

void stop(Motor motor) {
  digitalWrite(motor.inA, LOW);
  digitalWrite(motor.inB, LOW);
}

bool isInRange(int distance, int range) {
  return (distance && distance <= range);
}

bool seek(
  struct Motor motorA,
  struct Motor motorB,
  struct Ultrasnd eyes,
  unsigned int trashold,
  unsigned int range
) {
  if (isInRange(getDistance(eyes, trashold), range)) {
    return true; 
  }
  foward(motorA); 
  backward(motorB); 
  return false; 
}

void moveRobotFoward(struct Motor motorA, struct Motor motorB) {
  foward(motorA);
  foward(motorB);
}

void moveRobotBackward(struct Motor motorA, struct Motor motorB) {
  backward(motorA); 
  backward(motorB); 
}

void stopRobot(struct Motor motorA, struct Motor motorB) {
  stop(motorA);
  stop(motorB);
}

void retreatRobot(
  struct Motor motorA, 
  struct Motor motorB,  
  unsigned long retreat_time 
) {
  backward(motorA); 
  backward(motorB); 
  delay(retreat_time);
  stopRobot(motorA, motorB); 
}

void setup() {
  pinMode(H_OUT_A_PIN, OUTPUT);
  pinMode(H_OUT_B_PIN, OUTPUT);
  pinMode(H_OUT_C_PIN, OUTPUT);
  pinMode(H_OUT_D_PIN, OUTPUT);
  pinMode(HC_TRIG_PIN, OUTPUT);
  pinMode(HC_ECHO_PIN, INPUT);
  pinMode(SEEK_LED, OUTPUT);
  pinMode(RETREAT_LED, OUTPUT); 
  Serial.begin(9600); 
}

bool detectBorder() {
  if (digitalRead(IR_A) == LOW || digitalRead(IR_B) == LOW) {
    return true; 
  }
  return false;
}

State current_state = SEEK; 

void loop() {
  const unsigned int range = 50; // The robot will detect objects within this range
  const unsigned int trashold = 10; 
  struct Motor motorA = createMotor(H_OUT_A_PIN, H_OUT_B_PIN);
  struct Motor motorB = createMotor(H_OUT_C_PIN, H_OUT_D_PIN);
  struct Ultrasnd eyes = createUltrasndSensor(HC_TRIG_PIN, HC_ECHO_PIN);

  switch (current_state) {
    case SEEK: {
      Serial.println("seek"); 

      bool object_found = seek(motorA, motorB, eyes, trashold, range);
      bool border = detectBorder(); 

      if (border) {
        current_state = RETREAT;
      } else if (object_found) {
        digitalWrite(SEEK_LED, HIGH); 
        current_state = FOWARD; 
      } else {
        digitalWrite(SEEK_LED, LOW); 
        current_state = SEEK;
      }
      break;
    }
    case FOWARD: {
      Serial.println("foward"); 

      if (detectBorder()) {
        current_state = RETREAT; 
      } else if (isInRange(getDistance(eyes, trashold), range)) {
        moveRobotFoward(motorA, motorB); 
        current_state = FOWARD; 
      } else {
        current_state = SEEK; 
      }
      break; 
    }
    case RETREAT: {
      Serial.print("retreat");
      digitalWrite(RETREAT_LED, HIGH); 
      retreatRobot(motorA, motorB, 450); 
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