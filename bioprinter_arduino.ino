#include <stdio.h>
#include "gcode.h"
#include "movement.h"

#include <InkShield.h>

InkShieldA0A3 InkShield(8);

const int BAUD = 19200;
const int BUF_SIZE = 64;

// buffer for gcode
char cmdBuf[BUF_SIZE];
int cmdLoc = 0;


// Tell host we are ready for another command
void ready() {
  cmdLoc = 0;
  Serial.print(F("\r\n>"));
}

void setup() {
  Serial.begin(BAUD);
  initMotors();
  ready();
}

void loop() {
  while(Serial.available() > 0) {
    char c = Serial.read();
    Serial.print(c); 
    if(cmdLoc<BUF_SIZE) { 
      cmdBuf[cmdLoc++] = c;
    }
    if(cmdBuf[cmdLoc-1] ==';') {
      Serial.print("\r\n");
      break;
    }
  }

  if(cmdLoc > 0 && cmdBuf[cmdLoc-1]==';') {
    cmdBuf[cmdLoc] = 0;
    processCommand(cmdBuf);
    ready();
  }
}

void spray(int head, int val) {
  Serial.print(F("Spray "));
  Serial.print(head);
  Serial.print(F("Val "));
  Serial.print(val);
  Serial.print(F("\r\n"));
  //Ignores 'head' will only work for one inkshield currently
  InkShield.spray_ink(val);
  
}

