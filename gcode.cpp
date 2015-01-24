#include <Arduino.h>
#include <stdio.h>

void moveTo(float nx, float ny);

// Spray the ink
void spray(int head, int val);

// GCode Interpreter
void error() {
  Serial.print(F("\r\nError bad command: "));
}

char * consumeWhitespace(char * buf) {
  while(*buf != '\0' && (*buf == ' ' || *buf == '\t')) {
    ++buf;
  }
  return buf;
}

void processG1(char * buf) {
  float nx;
  float ny;

  char * nbuf = strtok(buf, "XY");
  nx = atof(nbuf);
  nbuf = strtok(0, "XY");
  ny = atof(nbuf);
  moveTo(nx, ny);
}

void processGCommand(char * buf, int id) {
  switch(id) {
  case 0:
  case 1:
    // G0 and G1 are both the same here
    processG1(buf);
    break;
  default:
    error();
    break;
  }
}

void processM700(char *buf) {
  char * b = strtok(buf, "PS");
  int head = atoi(b);
  b = strtok(0, "PS");
  int val = atoi(b);

  spray(head, val);
}

void processMCommand(char * buf, int id) {
  switch(id) {
  case 400:
    //Finish movement command from image_to_gcod, since 
    //we are syncronous here, ignore it
    break;
  case 700:
    //spray
    processM700(buf);
    break;
  default:
    error();
    break;
  }
}

void processCommand(char * cmd) {
  char c;
  int id, consumed;
  sscanf(cmd, "%c%d%n", &c, &id, &consumed);

  char * args = consumeWhitespace(cmd+consumed);
  switch(c) {
  case 'G':
    processGCommand(args, id);
    break;
  case 'M':
    processMCommand(args, id);
    break;
  default:
    error();
  }
}

