#include <Arduino.h>

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

// Current location of print head
float px, py;


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




