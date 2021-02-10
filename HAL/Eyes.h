/**********************************************************************************
   Eyes.h

   This file contains the class definition, attributes, and methods of the Eyeballs
   class. This class controls both the left and right eyeballs to make it easier for
   the Head class to control the eyeballs as a whole unit. This class includes set
   positions for the eyeballs, methods that move the eyeballs between different
   positions (ex. glance), as well as functions that move the eyeballs slower.

   Last Edited by Ethan Lauer on 3/28/20
 *********************************************************************************/

#include<Arduino.h>
#include "eye.h"// include single eyeball header file.

class Eyeballs {
  private:
    // Right and left Eyeball objects created (see eye.h)
    Eye eyeR = Eye();
    Eye eyeL = Eye();

    // Time Variables (in ms)
    unsigned long leftPause = 500;
    unsigned long rightPause = 150;
    unsigned long timeBetweenGlance = 2000;
    unsigned long timeBetweenLook = 3000;
    unsigned long timeNow = 0; //time variable for moving motors without delays
    int moveTime = 5;

    // Switch case variables
    int glanceState = 0;
    int leftRightState = 0;
    int stepEyeVertState = 0;
    int stepEyeHorState = 0;

    // Miscellaneous
    boolean initRun = true; // used to have part of the code only run for the first iteration of the loop
    int randNum;    // randum number for idleing glances


  public:
    //************************************************INITIALIZE********************************
     //Define the Eyeballs
    Eyeballs() {
    }

    /*
       setUp - get a random number from an unused analog pin, setup both eyeballs, and set to a neutral position.
    */
    void setUp() {
      randomSeed(analogRead(A0)); //analog read an unused pin
      eyeR.setUp(false, turnigy);
      eyeL.setUp(true, turnigy);
      neutralBoth();
    }


    //************************************************SET POSITIONS and Basics********************************
    // ***************These are commands for the eyeballs to move to the most commonly used setpoints.

    /*
       moveHorEyesTo - move the eyes to a desired horizontal position
              int leftHorEyePos - desired horizontal position for left eye
              int rightHorEyePos -desired horizontal position for right eye
    */
    void moveHorEyesTo(int leftHorEyePos, int rightHorEyePos) {
      eyeR.moveHorEyeTo(rightHorEyePos);
      eyeL.moveHorEyeTo(leftHorEyePos);
    }

    /*
       moveVertEyesTo - move eyes to a desired vertical position
              int leftVertEyePos - desired vertical position for left eye
              int rightVertEyePos -desired vertical position for right eye
    */
    void moveVertEyesTo(int leftVertEyePos, int rightVertEyePos) {
      eyeR.moveVertEyeTo(rightVertEyePos);
      eyeL.moveVertEyeTo(leftVertEyePos);
    }

    // NOTE: Create a function that puts both moveHorEyesTo() and moveVertEyesTo() in one function 
    //       which would make it easier to move both eyes in the vertical and oor horiztonal direction

    /*
      neutralBoth- move both eyes to a neutral position
    */
    void neutralBoth() {
      eyeR.neutralEye();
      eyeL.neutralEye();
    }
    /*
       leftBoth - have both eyes look left
    */
    void leftBoth() {
      eyeR.leftEye();
      eyeL.leftEye();
    }
    /*
           rightBoth - have both eyes look right
    */
    void rightBoth() {
      eyeR.rightEye();
      eyeL.rightEye();
    }

    /*
           upBoth - have both eyes look up
    */
    void upBoth() {
      eyeR.upEye();
      eyeL.upEye();
    }

    /*
       downBoth - have both eyes look down
    */
    void downBoth() {
      eyeR.downEye();
      eyeL.downEye();
    }
    /*
           crossEyed - have both eyes point to the center of the head
                      to be used when head is out of it
    */
    void crossEyed() {
      eyeR.leftEye();
      eyeL.rightEye();
    }

    //************************************************COMBO MOVEMENTS********************************
    // **************These functions involve the eyeballs moving between setpoints quickly.**********
    /*
       lookLeftandRight - move between looking left and right max positions
    */
    void lookLeftandRight() {
      switch (leftRightState) {
        case 0:
          rightBoth();
          if (millis() > timeNow + timeBetweenLook) {
            timeNow = millis();
            leftRightState = 1;
          }
          break;

        case 1:
          leftBoth();
          if (millis() > timeNow + timeBetweenLook) {
            timeNow = millis();
            leftRightState = 0;
          }
          break;
      }
    }

    //****************************GLANCING******************************
    /*
       Glance - generic glancing function that breifly move eyes either left, right, up, or down
            int glanceDir = integer for the direction glancing to 0=left, 1=right, 2= up, 3=down
            int timeBtwnGlance - time in milliseconds between each glance
            int pauseTime - time (in ms) the eyes spend in the desired position
    */
    void Glance(int glanceDir, int timeBtwnGlance, int pauseTime) {
      switch (glanceState) {
        case 0:
          neutralBoth();  // have eyes start in a neutral position
          if (millis() > timeNow + timeBtwnGlance) {
            timeNow = millis();
            glanceState = 1;    // switch once the input time between glance time has passed
          }
          break;

        case 1:       // move the eyeballs to the desired direction based on the input parameter
          switch (glanceDir) {
            case 0:
              leftBoth();
              break;
            case 1:
              rightBoth();
              break;
            case 2:
              upBoth();
              break;
            case 3:
              downBoth();
              break;
          }
          if (millis() > timeNow + pauseTime) {   //has enough time has passed for the eyes to move to the position?
            timeNow = millis();         // reset the timer
            randNum = random(0, 4);     // *applicable to Idle only* recalculate the random number so the each glance is in a random direction
            glanceState = 0;            // switch back to neutral
          }
          break;
      }

    }

    /*
      glanceLeft- glance left for generic time
    */
    void glanceLeft() {
      Glance(0, timeBetweenGlance, leftPause);
    }

    /*
       glanceRight-glance right for generic time
    */
    void glanceRight() {
      Glance(1, timeBetweenGlance, leftPause);
    }


    /*
      glanceUp- glance up for generic time
    */
    void glanceUp() {
      Glance(2, timeBetweenGlance, leftPause);
    }

    /*
       glanceDown-glance down for generic time
    */
    void glanceDown() {
      Glance(3, timeBetweenGlance, leftPause);
    }

    //************************************DYNAMIC MOVEMENTS*************************************************
    // ************** These are movements that have the eyeballs move at a slower pace,steping between******
    // ************** setpoints to give a more natural, smooth motion.**************************************

    //**************** SET COMMAND POSITIONS***************
    /*
      setEyeCmdUpVert- set the command of both of the eyes to the maximum upwards vertical position
    */
    void setEyeCmdUpVert() {
      eyeR.setCmdEye(eyeR.upVertEye);
      eyeL.setCmdEye(eyeL.upVertEye);
    }


    /*
      setEyeCmdDownVert- set the command of both of the eyes to the minimum downwards vertical position
    */
    void setEyeCmdDownVert() {
      eyeR.setCmdEye(eyeR.downVertEye);
      eyeL.setCmdEye(eyeL.downVertEye);
    }


    //**************** STEPPING UP AND DOWN***************

    /*
       stepEyesUp -step both the eyes up in increments of a given degree change
            int degChange = how many degrees the servos are moving at at time (how fast the eyes are moving up)
         return boolean result - true if both eyes are at the upper limit
    */
    boolean stepEyesUp(int degChange) {
      boolean result; // store result boolean (default to false)
      // variables to store result of individual eye step functions
      boolean rightSet;
      boolean leftSet;
      // stepEyeUp returns true if the eye has been commanded to (and assumed to reach) the upper limit
      // and commands the eye to stay at that position until told otherwise
      rightSet = eyeR.stepEyeUp(timeNow, moveTime, false, degChange);
      leftSet = eyeL.stepEyeUp(timeNow, moveTime, true, degChange);
      result = rightSet && leftSet;
      return result; // return true if both are at the upper limit
    }

    /*
       stepEyesDown -step both the eyes down in increments of a given degree change
               int degChange = how many degrees the servos are moving at at time (how fast the eyes are moving)
           return boolean result - true if both eyes are at the lower limit
    */
    boolean stepEyesDown(int degChange) {
      boolean result;// store result boolean (default to false)
      // variables to store result of individual eye step functions
      boolean rightSet;
      boolean leftSet;
      // stepEyeDown returns true if the eye has been commanded to (and assumed to reach) the lower limit
      // and commands the eye to stay at that position until told otherwise
      rightSet = eyeR.stepEyeDown(timeNow, moveTime, false, degChange);
      leftSet = eyeL.stepEyeDown(timeNow, moveTime, true, degChange);
      result = rightSet && leftSet;
      return result;  // return true if both are at the lower limit
    }


    /*
       stepEyesUpandDown -step both the eyes up and down continuously
            int degChangeUp = how many degrees the servos should move at a time (how fast the eyes are moving up)
            int degChangeDown = how many degrees the servos should move at a time (how fast the eyes are moving down)
    */
    void stepEyesUpandDown(int degChangeUp, int degChangeDown) {
      boolean bothSet;  // boolean storing true if both the left and right eyes have reached the goal (upper or lower limit)
      if (initRun) {  // when the function is first called
        setEyeCmdDownVert();  // set the cmd variable for each eye to the downward vertical position
        downBoth();         // move the eyeballs to the downward vertical position
        initRun = false;    // set boolean to false to prevent this portion from being called with each iteration
      }
      switch (stepEyeVertState) {
        case 0:
          bothSet = stepEyesUp(degChangeUp); // step both the eyeballs up until they reach the upper limit
          if (bothSet) {          // switch states
            stepEyeVertState = 1;
            bothSet = false;
          }
          break;

        case 1:
          bothSet = stepEyesDown(degChangeDown);// step both the eyeballs up until they reach the lower limit
          if (bothSet) {
            stepEyeVertState = 0;
            bothSet = false;
          }
          break;
      }
    }

    /*
       stepEyesUpandDownVerySlow - step both the eyes up at the slowest pace with both eyes moving at
                                  increments of 1 degree for both up and down
    */
    void stepEyesUpandDownVerySlow() {
      stepEyesUpandDown(1, 1);
    }

    /*
      stepEyesUpandDownSlow - step both the eyes up at a motderate pace with both eyes moving at
                                 increments of 3 degree for both up and down
    */
    void stepEyesUpandDownSlow() {
      stepEyesUpandDown(3, 3);
    }



    //*******NOTE: Not as efficient as the stepEyesDown and stepEyesUp functions defined previously*******//
    //
    //
    /*
       stepEyesUpFast - step eyes up fast
    */
    void stepEyesUpFast() {
      eyeR.stepEyeUp(timeNow, moveTime, false, 5);
      eyeL.stepEyeUp(timeNow, moveTime, true, 5);
    }
    /*
      stepEyesUpFast - step eyes up fast
    */
    void stepEyesDownFast() {
      eyeR.stepEyeDown(timeNow, moveTime, false, 5);
      eyeL.stepEyeDown(timeNow, moveTime, true, 5);
    }
    //
    //
    //******NOTE: Not as efficient as the stepEyesDown and stepEyesUp functions defined previously*******//


    //**************** STEPPING LEFT AND RIGHT***************
    /*
      stepEyesLeft -step both the eyes to the left in increments of a given degree change
           int degChange = how many degrees to move the servo (how fast the eyes are moving left)
        return boolean result - true if both the left and right eyes are at the left limit
    */
    boolean stepEyesLeft(int degChange) {
      boolean result;// store result boolean (default to false)
      // variables to store result of individual eye step functions
      boolean rightSet;
      boolean leftSet;
      // stepEyeLeft returns true if the eye has been commanded to (and assumed to reach) the left limit
      // and commands the eye to stay at that position until told otherwise
      rightSet = eyeR.stepEyeLeft(timeNow, moveTime, false, degChange);
      leftSet = eyeL.stepEyeLeft(timeNow, moveTime, true, degChange);
      result = rightSet && leftSet;
      return result; // true if both at left
    }


    /*
      stepEyesRight -step both the eyes right
             int degChange = how many degrees to move the servo (how fast the eyes are moving right)
         return boolean result - true if both the left and right eyes are at the right limit
    */
    boolean stepEyesRight(int degChange) {
      boolean result;// store result boolean (default to false)
      // variables to store result of individual eye step functions
      boolean rightSet;
      boolean leftSet;
      // stepEyeRight returns true if the eye has been commanded to (and assumed to reach) the right limit
      // and commands the eye to stay at that position until told otherwise
      rightSet = eyeR.stepEyeRight(timeNow, moveTime, false, degChange);
      leftSet = eyeL.stepEyeRight(timeNow, moveTime, true, degChange);
      result = rightSet && leftSet;
      return result; // true if both at right
    }



    /*
       stepEyesLeftandRight -step both the eyes left and right continuously
            int degChangeLeft = how many degrees the servo is invrementing at (how fast the eyes are moving left)
            int degChangeRight = how many degrees the servo is invrementing at(how fast the eyes are moving right)
    */
    void stepEyesLeftandRight(int degChangeLeft, int degChangeRight) {
      boolean bothSet;    // variable for when both eyes are set(default=false)
      if (initRun) {
        //        setEyeCmdDownVert();
        eyeR.setCmdEye(eyeR.rightHorEye);   // Note: these can be replaced with a setEyeCmdRightHor() that was not created
        eyeL.setCmdEye(eyeL.rightHorEye);
        rightBoth();        // move both eyeballs to the right
        initRun = false;    // set to false so this does not run with each loop.
      }
      switch (stepEyeHorState) {
        case 0:
          bothSet = stepEyesLeft(degChangeLeft);  // step both eyeballs to the left at given degree change
          if (bothSet) {            // switch directions if both eyeballs are at the left limit
            stepEyeHorState = 1;
            bothSet = false;
          }
          break;

        case 1:
          bothSet = stepEyesRight(degChangeRight);  // step both eyeballs to the right at given degree change
          if (bothSet) {          // switch directions if both eyeballs are at the right limit
            stepEyeHorState = 0;
            bothSet = false;
          }
          break;
      }
    }


    /*
       stepEyesLeftandRightSlow - Step the eyes left and right at a slower pace (incrementing by 3 degrees for both the left and right
    */
    void stepEyesLeftandRightSlow() {
      stepEyesLeftandRight(3, 3);
    }

    //********************************************************* REALISTIC MOVEMENTS******************************************************

    /*
       rollEyes - command the eyes to roll (see eye.h for details)
    */
    void rollEyes() {
      eyeR.rollEye(timeNow, moveTime, false, 1);
      eyeL.rollEye(timeNow, moveTime, true, 1);
    }

    /*
       idleEyes - have the eyes glance in a random direction for a different period of time based on the LOC
                  the random direction is calculated after each glance (see Glance() for details)
            int LOCState - input of the LOC of HAL and change the time between each glance based on this state
                           (0-alert, 1=verbal, 2=pain, 3=unresponsive)
    */
    void idleEyes(int LOCState) {
      int timeBtwnGlance;
      switch (LOCState) {   // change the time between each glance based on the level of conciousness
        case 0:
          timeBtwnGlance = 5000;
          break;
        case 1:
          timeBtwnGlance = 3000;
          break;
        case 2:
          timeBtwnGlance = 3000;
          break;
        case 3:
          timeBtwnGlance = 10000;
          break;
      }
      Serial.print("glancing to   = "); Serial.println(randNum); // for debugging
      Glance(randNum, timeBtwnGlance, leftPause);   // glance in the direction
    }

};
