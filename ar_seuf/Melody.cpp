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


#include "Melody.h"
#include "IO_Pin.h"
#include <Arduino.h>

/* Melody Variables */
int length = 4; // the number of notes
char notes[] = "ddef";
int beats[] = {1, 1, 1, 3};
int tempo = 100;


Melody::Melody() {
}

void Melody::attach() {
  pinMode(ZIK, OUTPUT);
}

/* Melody Functions */
void Melody::playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(ZIK, HIGH);
    delayMicroseconds(tone);
    digitalWrite(ZIK, LOW);
    delayMicroseconds(tone);
  }
}

void Melody::playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void Melody::playMelody() {
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


void Melody::playMelody2() {
  int length_2 = 15; // the number of notes
  char notes_a_vous_dirais_je_maman[] = "ccggaagffeeddc "; // a space represents a rest
  int beats_a_vous_dirais_je_maman[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
  int tempo2 = 1000;
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


