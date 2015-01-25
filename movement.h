#ifndef MOVEMENT_H
#define MOVEMENT_H

// Initialize motor data, eventually calibrate, test for end stops etc
void initMotors();
  
// Syncronously moves the head to the new coords
void moveTo(float nx, float ny);

// Spray the ink
void spray(int head, int val);


#endif
