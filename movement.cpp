#include <Arduino.h>

struct Motor {
  const int * pins;
  int curStep;
};

//Y pins
const int Y_MOTOR_PINS[] = {
  4, 5, 6, 7};

//X pins
const int X_MOTOR_PINS[] = {
  2, 3, 11, 10};

const int DELAY_TIME = 4;

const int X_STEPS_PER_MM = 4;
const int Y_STEPS_PER_MM = 4;

const int COOLDOWN_TIME = 5000;
const int COOLDOWN_STEPS = 250;

const bool STEP_PATTERN[4][4] = {
  {
    LOW, HIGH, HIGH, LOW      }
  ,
  {
    LOW, HIGH, LOW, HIGH      }
  ,
  {
    HIGH, LOW, LOW, HIGH      }
  ,
  {
    HIGH, LOW, HIGH, LOW      }
  ,
};

// Current location of print head
float px, py;

Motor mx, my;
int cooldownStep;

void setPins(Motor* m) {
  for(int i=0; i<4; i++) {
    pinMode(m->pins[i], OUTPUT);
    digitalWrite(m->pins[i], LOW);
  }
}

void initMotors() {
  mx.pins = X_MOTOR_PINS;
  mx.curStep = 0;

  my.pins = Y_MOTOR_PINS;  
  my.curStep = 0;

  setPins(&mx);
  setPins(&my);
  cooldownStep = 0;
  
  //assume table is at 0,0.
  // TODO add end stops and find this
  px = 0.0;
  py = 0.0;
}

void off(Motor *m, int cooldownTime) {
  for(int i=0; i<4; i++) {
    digitalWrite(m->pins[i], LOW);
  }
  delay(cooldownTime);
}

void step(Motor *m, int steps, int dir) {

  for(int s=0; s<steps; s++) {
    Serial.print("STEP");
    for(int i=0; i<4; i++) {
      digitalWrite(m->pins[i], STEP_PATTERN[m->curStep][i]);
    }

    m->curStep += dir;
    if (m->curStep < 0) {
      m->curStep = 3;
    } 
    else if (m->curStep > 3) {
      m->curStep = 0;
    }

    delay(DELAY_TIME);

    cooldownStep++;
  }
}

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
      step(&mx, 1, dirX);
      over += dy;
      if(over > dx) {
        over -= dx;
        step(&my, 1, dirY);
      }
    }
  } 
  else {
    for(int i = 0; i < dy; i++) {
      step(&my, 1, dirY);
      over += dx;
      if(over > dy) {
        over -= dy;
        step(&mx, 1, dirX);
      }
    }
  }

  px += float(dx) / X_STEPS_PER_MM * dirX;
  py += float(dy) / Y_STEPS_PER_MM * dirY;
  
  off(&mx, 0);
  off(&my, 0);
  if(cooldownStep >= COOLDOWN_STEPS) {
    off(&mx, COOLDOWN_TIME/2);
    off(&my, COOLDOWN_TIME/2);
    cooldownStep = 0;
  }

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







