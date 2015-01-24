#include <stdio.h>

const int X_STEPS_PER_MM = 10;
const int Y_STEPS_PER_MM = 10;

const int BAUD = 19200;
const int BUF_SIZE = 64;

char cmdBuf[BUF_SIZE];
int cmdLoc = 0;

// Current location of print head
float px, py;

// Syncronously moves the head to the new coords
void moveTo(float nx, float ny) {
  Serial.print(F("Starting move to "));
  Serial.print(nx);
  Serial.print(F(","));
  Serial.print(ny);
  Serial.print(F("\r\n"));
  
  // Linear interp
  int dx = (nx - px) * X_STEPS_PER_MM;
  int dirX = dx>0?1:-1;
  int dy = (ny - py) * Y_STEPS_PER_MM;
  int dirY = dy>0?1:-1;
  
  dx = abs(dx);
  dy = abs(dy);
  
  int over = 0;
  
  if(dx > dy) {
    for(int i = 0; i < dx; i++) {
      Serial.print("X");
      over += dy;
      if(over > dx) {
        over -= dx;
        Serial.print("Y");
      }
    }
  } else {
    for(int i = 0; i < dy; i++) {
      Serial.print("Y");
      over += dx;
      if(over > dy) {
        over -= dy;
        Serial.print("X");
      }
    }
  }

  px += float(dx) / X_STEPS_PER_MM * dirX;
  py += float(dy) / Y_STEPS_PER_MM * dirY;

  Serial.print(F("\r\nEnding at "));
  Serial.print(px);
  Serial.print(",");
  Serial.print(py);
  Serial.print("\r\n");
}

void processG1(char * buf) {
  float nx;
  float ny;
  
  char * nbuf = strtok(buf, "X");
  nx = atof(nbuf);
  nbuf = strtok(0, "Y");
  ny = atof(nbuf);
  moveTo(nx, ny);
}

void processGCommand(char * buf) {
  int id, consumed;
  sscanf(buf, "%d%n", &id, &consumed);
  switch(id) {
  case 0:
  case 1:
    // G0 and G1 are both the same here
    processG1(buf + consumed);
    break;
  default:
    error();
    break;
  }
}

void processCommand() {
  switch(cmdBuf[0]) {
  case 'G':
    processGCommand(cmdBuf+1);
    break;
  default:
    error();
  }
}

// Tell host we are ready for another command
void ready() {
  cmdLoc = 0;
  Serial.print(F("\r\n>"));
}

void error() {
  Serial.print(F("\r\nError bad command: "));
  Serial.print(cmdBuf);
}

void setup() {
  Serial.begin(BAUD);
  px = 0;
  py = 0;
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
      break;
    }
  }

  if(cmdLoc > 0 && cmdBuf[cmdLoc-1]==';') {
    cmdBuf[cmdLoc] = 0;
    processCommand();
    ready();
  }
}

