/**
 * Seuf Arduino WiFly
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
    execCommand("rm"); // Radar middle
    delay(500); // Wait for radar in position
    int distance_front = ping.ping(); 
    
    execCommand("rl"); // Radar left
    delay(1000); // Wait for radar in position
    int distance_left = ping.ping(); 

    execCommand("rr"); // Radar right
    delay(2000); // Wait for radar in position
    int distance_right = ping.ping(); 
    
    execCommand("rm");
    delay(100);    
    
    Serial.print("distance front :");
    Serial.println(distance_front);   
    Serial.print("distance Left :");
    Serial.println(distance_left);
    Serial.print("distance Right :");
    Serial.println(distance_right);
    
    if (distance_front > SAFE_DISTANCE) {
      /* OK, go FWD */
      Serial.println("fwd");
      execCommand("fwd");
      delay(2000);
      execCommand("stop");
    } else {
      execCommand("stop");
      /* Test if we can turn left */
      if (distance_left > SAFE_DISTANCE) {
         Serial.println("tl");
        /* OK, turn LEFT */
        execCommand("tl");
        delay(1000);
        execCommand("stop");
      } else {
        /* Test if we can Turn Right */
        if (distance_right > SAFE_DISTANCE) {
          Serial.println("TR");
          execCommand("tr");
          delay(1000);
          execCommand("stop");
        } else {
          Serial.println("demitour");
          /* impasse : demi tour ! */
          execCommand("tr");
          delay(5000);
          execCommand("stop");
        }          
      }
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




