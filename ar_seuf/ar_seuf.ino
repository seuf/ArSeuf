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

#include <SPI.h>
#include <WiFly.h>
#include <Servo.h>
#include "Credentials.h"
#include "IO_Pin.h"
#include "Melody.h"
#include "Motor.h"
#include "Ping.h"


#define MAX_COMMAND_LENGTH 10 //Max command Length
#define DEFAULT_SPEED 200
#define SAFE_DISTANCE 50

const char delimiters[] = "/"; //Delimiter for url
int servopos = 90;


WiFlyServer server(80);
Servo servo;
Motor motor;
Melody melody;
Ping ping;

void setup() {
  analogWrite(RED_LED_PIN, 0);
  analogWrite(GREEN_LED_PIN, 0);
  analogWrite(BLUE_LED_PIN, 255);
  
  Serial.begin(9600);
  
  servo.attach(SERVO_PIN);
  servo.write(90);
  
  motor.attach();
  melody.attach();
   
  /* Initializing WiFly */
  WiFly.setUart(&Serial);
  WiFly.begin(use_adhoc);
    
  if (use_adhoc) {
    /* Creating Adhoc Network */
    if(!WiFly.createAdHocNetwork(ssid_adhoc)) {
      analogWrite(RED_LED_PIN, 255);
      analogWrite(GREEN_LED_PIN, 0);
      analogWrite(BLUE_LED_PIN, 0);
      Serial.println("Failed to create network...");
      while(1) {
      
      }
    }
  } else {
    /* Joining wifi */
    if (!WiFly.join(ssid, passphrase, is_wpa)) {
      analogWrite(RED_LED_PIN, 255);
      analogWrite(GREEN_LED_PIN, 0);
      analogWrite(BLUE_LED_PIN, 0);
      Serial.println("Fail to join network...");
      while(1) {
        
      }
    }
  }
  
  Serial.print("Arduino WiFly IP : ");
  Serial.println(WiFly.ip());

  /* Starting Server */
  server.begin();

  
  analogWrite(RED_LED_PIN, 0);
  analogWrite(GREEN_LED_PIN, 255);
  analogWrite(BLUE_LED_PIN, 0);
  melody.playMelody();
}


void loop() {
 
  
  WiFlyClient client = server.available();
  
  if (client) {
    char buffer[512];
    char get_url[MAX_COMMAND_LENGTH];
    char cmd[MAX_COMMAND_LENGTH];
    //char command[MAX_COMMAND_LENGTH];
    boolean parsed_ok = false;
    int buffPos = 0;



    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        buffer[buffPos++] = c;
        Serial.print(c);
        
        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        if (c == '\n' && current_line_is_blank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          
          //printHTML(&client);
          
          sscanf(buffer, "GET %s HTTP/1.1", get_url);

          char *cp = strtok(get_url, delimiters);
          if(cp != NULL) {
            strcpy(cmd, cp);
            parsed_ok = true;
          } else {
            client.println("no command found");
          }
 
          if(parsed_ok) {
            execCommand(cmd);
          }
          break;
        }
        if (c == '\n') {
          // we're starting a new line
          current_line_is_blank = true;
        } else if (c != '\r') {
          // we've gotten a character on the current line
          current_line_is_blank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(100);
    client.stop();
  } else {
    /* No Client : Autonomous mode :) */
    
    /* Scanning All directions */
    servo.write(90); // Radar middle
    delay(200); // Wait for radar in position
    int distance_front = ping.ping(); 
    
    if (distance_front < SAFE_DISTANCE) {
      motor.stopRobot();
      /* Ping left and front left */
      servo.write(0);
      delay(1000);
      int cm0 = ping.ping();
      servo.write(45);
      delay(1000);
      int cm45 = ping.ping();
      
      if (cm0 < SAFE_DISTANCE && cm45 < SAFE_DISTANCE) {
        /* Can't turn LEFT, ping right and front right */
        servo.write(180);
        delay(1000);
        int cm180 = ping.ping();
        servo.write(135);
        delay(1000);
        int cm135 = ping.ping();
        
        if (cm180 < SAFE_DISTANCE && cm135 < SAFE_DISTANCE) {
          /* Can't turn right : go back (turn 3 seconds) */
          motor.turnRight(DEFAULT_SPEED);
          delay(3000);
          motor.stopRobot();
        } else {
          motor.turnRight(DEFAULT_SPEED);          
          delay(1000);
          motor.stopRobot();
        }
      } else {
          motor.turnLeft(DEFAULT_SPEED);
          delay(1000);
          motor.stopRobot();
      }
    } else {
        // Cool ! Let's GO !
        motor.goForward(DEFAULT_SPEED);
    }
  }
}


/**
 * Function used to switch ON / OFF a LED
 * @param char *led : a led name (i.e. led1, led2...)
 * @param char *command : the led status to switch (ON / OFF)
 */
void execCommand(char *cmd) {
  
  if (strcmp(cmd, "red") == 0) {
    analogWrite(RED_LED_PIN, 255);
    analogWrite(GREEN_LED_PIN, 0);
    analogWrite(BLUE_LED_PIN, 0);
  } else if (strcmp(cmd, "blue") == 0) {
    analogWrite(RED_LED_PIN, 0);
    analogWrite(GREEN_LED_PIN, 0);
    analogWrite(BLUE_LED_PIN, 255);
  } else if (strcmp(cmd, "green") == 0) {
    analogWrite(RED_LED_PIN, 0);
    analogWrite(GREEN_LED_PIN, 255);
    analogWrite(BLUE_LED_PIN, 0);
  } else if (strcmp(cmd, "play") == 0) {
    melody.playMelody();
    delay(10);
  } else if (strcmp(cmd, "play2") == 0) {
    melody.playMelody2();
    delay(10);
  } else if (strcmp(cmd, "rl") == 0) {
    /* Radar Left */
    servopos = 0;
    servo.write(0);
    delay(15);
  } else if (strcmp(cmd, "rm") == 0) {
    /* Radar Midle */
    servopos = 90;
    servo.write(90);
    delay(15);
  } else if (strcmp(cmd, "rr") == 0) {
    /* Radar Right */
    servopos = 180;
    servo.write(180);
    delay(15);
  } else if (strcmp(cmd, "fwd") == 0) {
    motor.goForward(DEFAULT_SPEED);
  } else if (strcmp(cmd, "back") == 0) {
    motor.goBack(DEFAULT_SPEED);
  } else if (strcmp(cmd, "tl") == 0) {
    motor.turnLeft(DEFAULT_SPEED);
  } else if (strcmp(cmd, "tr") == 0) {
    motor.turnRight(DEFAULT_SPEED);
  } else if (strcmp(cmd, "stop") == 0) {
    motor.stopRobot();
  } else if (strcmp(cmd, "stopt") == 0) {
    motor.stopTurn(DEFAULT_SPEED);
  }
}




