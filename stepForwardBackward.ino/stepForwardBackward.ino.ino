
/* 
 Stepper Motor Control - one step at a time
 
 This program drives a unipolar or bipolar stepper motor. 
 The motor is attached to digital pins 8 - 11 of the Arduino.
 
 The motor will step one step at a time, very slowly.  You can use this to
 test that you've got the four wires of your stepper wired to the correct
 pins. If wired correctly, all steps should be in the same direction.
 
 Use this also to count the number of steps per revolution of your motor,
 if you don't know it.  Then plug that number into the oneRevolution
 example to see if you got it right.
 
 Created 30 Nov. 2009
 by Tom Igoe
 
 */

#include <Stepper.h>

const int stepsPerRevolution = 20;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper topStepper(stepsPerRevolution, 4,5,6,7);
Stepper bottomStepper(stepsPerRevolution, 2,3,11,10);

int stepCount = 0;         // number of steps the motor has taken

//Y pins
int yPin1 = 4;
int yPin2 = 5;
int yPin3 = 6;
int yPin4 = 7;
int delayTime = 4;

//X pins
int xPin1 = 2;
int xPin2 = 3;
int xPin3 = 11;
int xPin4 = 10;




void stepForward(int steps, char xOrY){
     int motorPin1;
     int motorPin2;
     int motorPin3;
     int motorPin4;

     if(xOrY == 'X' || xOrY == 'x'){
          motorPin1 = yPin1;
          motorPin2 = yPin2;
          motorPin3 = yPin3;
          motorPin4 = yPin4;
     } else {
          motorPin1 = xPin1;
          motorPin2 = xPin2;
          motorPin3 = xPin3;
          motorPin4 = xPin4;
     }

  for (int i = 0; i < steps; i++){
     digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, HIGH);
     digitalWrite(motorPin3, HIGH);
     digitalWrite(motorPin4, LOW);
     delay(delayTime);
  
     digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, HIGH);
     digitalWrite(motorPin3, LOW);
     digitalWrite(motorPin4, HIGH);
     delay(delayTime);
  
     digitalWrite(motorPin1, HIGH);
     digitalWrite(motorPin2, LOW);
     digitalWrite(motorPin3, LOW);
     digitalWrite(motorPin4, HIGH);
     delay(delayTime);
   
     digitalWrite(motorPin1, HIGH);
     digitalWrite(motorPin2, LOW);
     digitalWrite(motorPin3, HIGH);
     digitalWrite(motorPin4, LOW);
     delay(delayTime);
  }

}

void stepBackward(int steps, char xOrY){
     int motorPin1;
     int motorPin2;
     int motorPin3;
     int motorPin4;

     if(xOrY == 'X' || xOrY == 'x'){
          motorPin1 = yPin1;
          motorPin2 = yPin2;
          motorPin3 = yPin3;
          motorPin4 = yPin4;
     } else {
          motorPin1 = xPin1;
          motorPin2 = xPin2;
          motorPin3 = xPin3;
          motorPin4 = xPin4;
     }
  for (int i = 0; i < steps; i++){
     digitalWrite(motorPin1, HIGH);
     digitalWrite(motorPin2, LOW);
     digitalWrite(motorPin3, HIGH);
     digitalWrite(motorPin4, LOW);
     delay(delayTime);
     
     digitalWrite(motorPin1, HIGH);
     digitalWrite(motorPin2, LOW);
     digitalWrite(motorPin3, LOW);
     digitalWrite(motorPin4, HIGH);
     delay(delayTime);
  
     digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, HIGH);
     digitalWrite(motorPin3, LOW);
     digitalWrite(motorPin4, HIGH);
     delay(delayTime);
  
     digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, HIGH);
     digitalWrite(motorPin3, HIGH);
     digitalWrite(motorPin4, LOW);
     delay(delayTime);
  }
}


void setup() {
 pinMode(xPin1, OUTPUT);
 pinMode(xPin2, OUTPUT);
 pinMode(xPin3, OUTPUT);
 pinMode(xPin4, OUTPUT);
 pinMode(yPin1, OUTPUT);
 pinMode(yPin2, OUTPUT);
 pinMode(yPin3, OUTPUT);
 pinMode(yPin4, OUTPUT);
}
  
void loop() {
 //stepForward(20,'x');
 delay(1000);
 //stepBackward(20,'x');
 delay(1000);
 stepBackward(20,'y');
 delay(1000);
 stepForward(20,'y');
 delay(1000);
}


