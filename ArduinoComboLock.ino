/*ArduinoComboLock.ino
 Caspar Hansen, 07/2014
 Use an encoder as a dial for a combination lock
 Four digits, turn three times to the right to reset
 
 Gratuitously copies code from this: http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Components/LED/_7Seg_Example.pde
 listed on Sparkfun's product page for the COM-09483 4-digit 7-segment display: https://www.sparkfun.com/products/9483
 
 Seven segment code license: "This code is public domain but you buy me [Nathan Seidle] a beer if you use this and we meet someday (Beerware license)."
 */

#include <Encoder.h>
#include "Spinner.h"
// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(5, 6);
//   avoid using pins with LEDs attached

Spinner mSpinner;

// Encoder combo lock 
int oldPosition  = -999;
int newPosition = 0;
boolean CCW = false; //CCW = counter clockwise direction
boolean oldCCW = true;
int value;
int loops = 0;
int combo[4];
int combopos = -1;
int comboDisplay = 0;
int secret[4] = {3, 5, 8, 9}; //Code to unlock
int deltaPos = 0;
int debounce = 2; //Must move by at least 2 counts for a poor man's debounce

// Seven segment display
int digit1 = 11; //PWM Display pin 12
int digit2 = 10; //PWM Display pin 9
int digit3 = 9; //PWM Display pin 8
int digit4 = 3; //PWM Display pin 6

int segA = A1; //Display pin 11
int segB = 2; //Display pin 7
int segC = 4; //Display pin 4
int segD = 12; //Display pin 2
int segE = A0; //Display pin 1
int segF = 7; //Display pin 10
int segG = 8; //Display pin 5

void setup() {
  // Encoder combo lock
  Serial.begin(9600);
  Serial.println("Basic combination lock test:");
  
  // Seven segment display
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
}

void loop() 
{
  UpdateEncoder();
  displayNumber(combo);

}

void UpdateEncoder()
{
  newPosition = myEnc.read();
  newPosition = newPosition / 4;

  if (newPosition != oldPosition) 
  {
    deltaPos += (newPosition - oldPosition);
    if (abs(deltaPos) >= debounce)
    {
      deltaPos = 0;

      //CCW turn = increasing count, CW = decresing
      //in order to match common lock dials
      if (newPosition > oldPosition)
      {
        value = mSpinner.Decrement();
        CCW = true;
      }
      else
      {
        value = mSpinner.Increment();
        CCW = false;

        //Keep track of how many times you make a revolution for reset
        if(value == 0)
          loops++;

        if(loops == 3) // Turn three times clockwise to reset
        {
          loops = 0;
          combo[0] = 0;
          combo[1] = 0;
          combo[2] = 0;
          combo[3] = 0;
          combopos = 0;
        }
      }

      if (oldCCW != CCW) // Change direction - move to the next digit
      {
        combopos++;
        loops = 0;
      }

      //Don't overflow
      if(combopos < 4 && combopos > -1)
        combo[combopos] = value;

      /*For debugging, print to serial:
       CCCC DLU
       C: 4 combo digits
       D: Direction of turning (L or R)
       L: Number of loops
       U: Lock unlocked
       */
      
      for (int i = 0; i < 4; i++)
      {
        Serial.print(combo[i]);
      }

      if (CCW == true)
        Serial.print(" L");  
      else
        Serial.print(" R");

      Serial.print(loops);

      Serial.print(IsUnlocked());

      Serial.println("");

      oldPosition = newPosition;
      oldCCW = CCW;    
    }
  }
}

boolean IsUnlocked()
{
  if(combopos == 3)
  {
    for (int i = 0; i < 4; i++)
    {
      if(combo[i] != secret[i])
        return false;
    }
    return true;
  }
  return false;
}

void displayNumber(int* toDisplay) {
#define DISPLAY_BRIGHTNESS  500

#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW

  long beginTime = millis();

  for(int digit = 0; digit < 4 ; digit++) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 0:
      digitalWrite(digit1, DIGIT_ON);
      break;
    case 1:
      digitalWrite(digit2, DIGIT_ON);
      break;
    case 2:
      digitalWrite(digit3, DIGIT_ON);
      break;
    case 3:
      digitalWrite(digit4, DIGIT_ON);
      break;
    }

    //Turn on the right segments for this digit
    lightNumber(toDisplay[digit] % 10);


    delayMicroseconds(DISPLAY_BRIGHTNESS); //Display this digit for a fraction of a second (between 1us and 5000us, 500 is pretty good)

    //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
  }

  while( (millis() - beginTime) < 10) ; //Wait for 20ms to pass before we paint the display again
}

//Seven segment display

//Given a number, turns on those segments
//If number == 10, then turn off number
void lightNumber(int numberToDisplay) {

#define SEGMENT_ON  LOW
#define SEGMENT_OFF HIGH

  switch (numberToDisplay){

  case 0:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 1:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 2:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 3:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 4:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 5:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 6:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 7:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 8:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 9:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 10:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;
  }
}

