#include <stdio.h>

//Y pins
const int Y_MOTOR_PIN_1 = 4;
const int Y_MOTOR_PIN_2 = 5;
const int Y_MOTOR_PIN_3 = 6;
const int Y_MOTOR_PIN_4 = 7;

//X pins
const int X_MOTOR_PIN_1 = 2;
const int X_MOTOR_PIN_2 = 3;
const int X_MOTOR_PIN_3 = 11;
const int X_MOTOR_PIN_4 = 10;

const int DELAY_TIME = 4;

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
      if(dirX == 1) {
        stepForward(1, 'X');
      } 
      else {
        stepBackward(1, 'X');
      }
      over += dy;
      if(over > dx) {
        over -= dx;
        if(dirY == 1) {
          stepForward(1, 'Y');
        } 
        else {
          stepBackward(1, 'Y');
        }
      }
    }
  } 
  else {
    for(int i = 0; i < dy; i++) {
      if (dirY == 1) {
        stepForward(1, 'Y');
      } 
      else {
        stepBackward(1, 'Y');
      }
      over += dx;
      if(over > dy) {
        over -= dy;
        if (dirX == 1) {
          stepForward(1, 'X');
        } 
        else {
          stepBackward(1, 'X');
        }
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

void spray(int head, int val) {
  Serial.print(F("Spray "));
  Serial.print(head);
  Serial.print(F("Val "));
  Serial.print(val);
  Serial.print(F("\r\n"));
}

// GCode Interpreter
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

void processCommand() {
  char c;
  int id, consumed;
  sscanf(cmdBuf, "%c%d%n", &c, &id, &consumed);

  char * args = consumeWhitespace(cmdBuf+consumed);
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
      Serial.print("\r\n");
      break;
    }
  }

  if(cmdLoc > 0 && cmdBuf[cmdLoc-1]==';') {
    cmdBuf[cmdLoc] = 0;
    processCommand();
    ready();
  }
}

void stepForward(int steps, char xOrY){
  int motorPin1;
  int motorPin2;
  int motorPin3;
  int motorPin4;

  if(xOrY == 'X' || xOrY == 'x'){
    motorPin1 = X_MOTOR_PIN_1;
    motorPin2 = X_MOTOR_PIN_2;
    motorPin3 = X_MOTOR_PIN_3;
    motorPin4 = X_MOTOR_PIN_4;
  } 
  else {
    motorPin1 = Y_MOTOR_PIN_1;
    motorPin2 = Y_MOTOR_PIN_2;
    motorPin3 = Y_MOTOR_PIN_3;
    motorPin4 = Y_MOTOR_PIN_4;
  }

  for (int i = 0; i < steps; i++){
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
    delay(DELAY_TIME);

    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
    delay(DELAY_TIME);

    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
    delay(DELAY_TIME);

    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
    delay(DELAY_TIME);
  }

}

void stepBackward(int steps, char xOrY){
  int motorPin1;
  int motorPin2;
  int motorPin3;
  int motorPin4;

  if(xOrY == 'X' || xOrY == 'x'){
    motorPin1 = X_MOTOR_PIN_1;
    motorPin2 = X_MOTOR_PIN_2;
    motorPin3 = X_MOTOR_PIN_3;
    motorPin4 = X_MOTOR_PIN_4;
  } 
  else {
    motorPin1 = Y_MOTOR_PIN_1;
    motorPin2 = Y_MOTOR_PIN_2;
    motorPin3 = Y_MOTOR_PIN_3;
    motorPin4 = Y_MOTOR_PIN_4;
  }

  for (int i = 0; i < steps; i++){
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
    delay(DELAY_TIME);

    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
    delay(DELAY_TIME);

    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
    delay(DELAY_TIME);

    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
    delay(DELAY_TIME);
  }
}



