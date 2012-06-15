/**
 * Copyright 2012, Thierry Sallé
 *
 * This file is part of ArSeuf.
 *  
 *  ArSeuf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ArSeuf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ArSeuf.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "Motor.h"
#include "Servo.h"
#include "IO_Pin.h"
#include <Arduino.h>

boolean is_fwd = false;
boolean is_back = false;

/* Constructor */
Motor::Motor() { 
}

void Motor::attach() {
  pinMode(MOTOR_LEFT_DIR, OUTPUT);
  pinMode(MOTOR_LEFT_PWM, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR, OUTPUT);
  pinMode(MOTOR_RIGHT_PWM, OUTPUT);
}


/************ Motor related Functions ****************/


void Motor::goForward(int speed) {  
  digitalWrite(MOTOR_LEFT_DIR, 0);
  digitalWrite(MOTOR_RIGHT_DIR, 0);
  analogWrite(MOTOR_LEFT_PWM, speed);
  analogWrite(MOTOR_RIGHT_PWM, speed);
  is_fwd = true;
}

void Motor::goBack(int speed) {
  digitalWrite(MOTOR_LEFT_DIR, 1);
  digitalWrite(MOTOR_RIGHT_DIR, 1);
  analogWrite(MOTOR_LEFT_PWM, speed);
  analogWrite(MOTOR_RIGHT_PWM, speed);
  is_back = true;
}


void Motor::stopRobot() {
  analogWrite(MOTOR_LEFT_PWM, 0);
  analogWrite(MOTOR_RIGHT_PWM, 0);
  is_fwd = false;
  is_back = false;
}


void Motor::turnLeft(int speed) {
  if (is_fwd or is_back) {
    /* We keep all motor in the same direction, but stop right motor */
    analogWrite(MOTOR_LEFT_PWM, speed);
	analogWrite(MOTOR_RIGHT_PWM, 0);
  } else {
    /* We reverse motor directions and power all motors */
    digitalWrite(MOTOR_LEFT_DIR, 1);
	digitalWrite(MOTOR_RIGHT_DIR, 0);
	analogWrite(MOTOR_LEFT_PWM, speed);
	analogWrite(MOTOR_RIGHT_PWM, speed);
  }
}

void Motor::stopTurn(int speed) {
  if (is_fwd) {
    this->goForward(speed);
  } else if (is_back) {
    this->goBack(speed);
  } else {
    this->stopRobot();
  }
}

void Motor::turnRight(int speed) {
  if (is_fwd or is_back) {
    /* We keep all motor in the same direction, but stop right motor */
    analogWrite(MOTOR_LEFT_PWM, 0);
    analogWrite(MOTOR_RIGHT_PWM, speed);
  } else {
    digitalWrite(MOTOR_LEFT_DIR, 0);
    digitalWrite(MOTOR_RIGHT_DIR, 1);
    analogWrite(MOTOR_LEFT_PWM, speed);
    analogWrite(MOTOR_RIGHT_PWM, speed);
  }
}


  
