/*ArduinoComboLock.ino
 Caspar Hansen, 07/2014
 Use an encoder as a dial for a combination lock
 Four digits, turn three times to the right to reset
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

void setup() {
  Serial.begin(9600);
  Serial.println("Basic combination lock test:");
}

int oldPosition  = -999;
int newPosition = 0;
boolean CCW = false; //CCW = counter clockwise direction
boolean oldCCW = true;
int value;
int loops = 0;
int combo[4];
int combopos = -1;
int secret[4] = {
  3, 5, 8, 9};

void loop() {
  newPosition = myEnc.read();
  newPosition = newPosition / 8;

  if (newPosition != oldPosition) 
  {
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
        
      if(loops == 3)
      {
        loops = 0;
        combo[0] = 0;
        combo[1] = 0;
        combo[2] = 0;
        combo[3] = 0;
        combopos = 0;
      }
    }
    
    if (oldCCW != CCW)
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
    D: Direction of turning
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














