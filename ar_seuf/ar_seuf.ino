/**
 * Seuf Arduino WiFly
 */

#include <SPI.h>
#include <WiFly.h>
#include <Servo.h>
#include "Credentials.h"

#define LED1 2 
#define LED2 3 
#define LED3 4 
#define LED4 5 
#define ZIK 6 //PIEZO ELEMENT
#define RED_LED_PIN  7
#define GREEN_LED_PIN 8
#define BLUE_LED_PIN 9
#define SERVO_PIN 10

#define MAX_COMMAND_LENGTH 10 //Max command Length

const char delimiters[] = "/"; //Delimiter for url



/* Melody Variables */
int length = 4; // the number of notes
//char notes_a_vous_dirais_je_maman[] = "ccggaagffeeddc "; // a space represents a rest
//int beats_a_vous_dirais_je_maman[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
char notes[] = "ddef";
int beats[] = {1, 1, 1, 3};
int tempo = 100;


/* Variables globales pour gérer l'état des loupiottes */
//int nb_pins = 4;
//boolean powered[4] = {false, false, false, false};
//int pins[4] = {LED1, LED2, LED3, LED4};
boolean led1_on = false;
boolean led2_on = false;
boolean led3_on = false;
boolean led4_on = false;

WiFlyServer server(80);
Servo servo;

void setup() {
  analogWrite(RED_LED_PIN, 0);
  analogWrite(GREEN_LED_PIN, 0);
  analogWrite(BLUE_LED_PIN, 255);
  
  Serial.begin(9600);
  
  /* Initializing LEDs... */
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(ZIK, OUTPUT);
  servo.attach(SERVO_PIN);
  
//  pinMode(LY1, OUTPUT);
//  pinMode(LY2, OUTPUT);
 
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
  playMelody();
}


void loop() {

 // Serial.println("Program Started.. Waiting for clients..");
  
  WiFlyClient client = server.available();
  
  if (client) {
    char buffer[512];
    char get_url[MAX_COMMAND_LENGTH];
    char cmd[MAX_COMMAND_LENGTH];
//    char *get_url;
//    char *cmd;
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
  }
  
}


/**
 * Function used to switch ON / OFF a LED
 * @param char *led : a led name (i.e. led1, led2...)
 * @param char *command : the led status to switch (ON / OFF)
 */
void execCommand(char *cmd) {
  
  if (strcmp(cmd, "led1") == 0) {
    if (led1_on) {
      digitalWrite(LED1, LOW);
      led1_on = false;
    } else {
      digitalWrite(LED1, HIGH);
      led1_on = true;
    }
  } else if (strcmp(cmd, "led2") == 0) {
    if (led2_on) {   
      digitalWrite(LED2, LOW);
      led2_on = false;
    } else {
      digitalWrite(LED2, HIGH);
      led2_on = true;
    }
  } else if (strcmp(cmd, "led3") == 0) {
    if (led3_on) {   
      digitalWrite(LED3, LOW);
      led3_on = false;
    } else {
      digitalWrite(LED3, HIGH);
      led3_on = true;
    }
  } else if (strcmp(cmd, "led4") == 0) {
    if (led4_on) {   
      digitalWrite(LED4, LOW);
      led4_on = false;
    } else {
      digitalWrite(LED4, HIGH);
      led4_on = true;
    }
  } else if (strcmp(cmd, "red") == 0) {
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
  } else if (strcmp(cmd, "left") == 0) {
    servo.write(-90);
    delay(10);
  } else if (strcmp(cmd, "right") == 0) {
    servo.write(90);
    delay(10);
  } else if (strcmp(cmd, "play") == 0) {
    playMelody2();
    delay(10);
  }
}




/* Melody Functions */
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(ZIK, HIGH);
    delayMicroseconds(tone);
    digitalWrite(ZIK, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void playMelody() {
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }

    // pause between notes
    delay(tempo / 2); 
  } 
}


void playMelody2() {
  int length_2 = 15; // the number of notes
  char notes_a_vous_dirais_je_maman[] = "ccggaagffeeddc "; // a space represents a rest
  int beats_a_vous_dirais_je_maman[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
  int tempo2 = 300;
  for (int i = 0; i < length_2; i++) {
    if (notes_a_vous_dirais_je_maman[i] == ' ') {
      delay(beats_a_vous_dirais_je_maman[i] * tempo2); // rest
    } else {
      playNote(notes_a_vous_dirais_je_maman[i], beats_a_vous_dirais_je_maman[i] * tempo2);
    }

    // pause between notes
    delay(tempo2 / 2); 
  } 
  
}
