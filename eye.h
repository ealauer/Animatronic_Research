/**********************************************************************************
  eye.h

  This file contains the class definition, attributes, and methods of each individual
  Eyeball system. Each system contains 2 servos: one for vertical movement and one for
  horizontal movement. With given prototype, the degree difference between vertical
  maximums for each eye and the horizontal maximums for each eye were different for
  each system. Therefore, the difference between these servos had to be calculated
  and taken into account when moving both eyes together. The direction the servo had
  to increment or decrement also changed depending on if it was the left or right eye.
  This file includes funtions for calculations, setting exact positions, and "step"
  functions that made the eyeballs increment slowly and create smooth motion. There
  are areas of improvement when setting the vertical or horizontal commands by making
  the functions simpler and more versatile that was not completed due to time constraints.

  Note: Unlike the vertical servos, the horizontal servos increment and decrement together
        so there does not have to be a discrepancy between the command values for each eye. This was
        found through testing the mechanical system. In future work, I suggest ensuring
        the servos are installed in such a way that the servos for the left and right eyes
        increment and decrement in the same direction.

  Last Edited by Ethan Lauer on 3/29/20
 *********************************************************************************/
#include<Arduino.h>
class Eye {
  private:
    // Servo Pins
    int vertEyePin;
    int horEyePin;

    int servoType; // turnigy for each servo

    bool isLeft; //true if left eye, false if right eye

    int downToUp;
    int upToDown;
    int stepEyeState = 0;

    int eyeRollState = 0;


  public:
    // stores where the eye was commanded to
    // made public so it could be set by the Eyeballs class (see Eyes.h for details)
    int cmdEye;
    int cmdEyeV;
    int cmdEyeH;

    //SERVO POSITIONS
    // made public so it could be used by the Eyeballs class (see Eyes.h for details)
    // it is possible to change these by adjusting the functions more (future work)
    //Up and Down
    int upVertEye;
    int downVertEye;
    //Left and Right
    int leftHorEye;
    int rightHorEye;
    //Neutral
    int neutralVertEye;
    int neutralHorEye;

    //************************************************INITIALIZE********************************
    // Define the eye
    Eye() {
    }

    /*
       setUp - set whether it is the left or right eye, set the servo positions,
                and calculate the difference for each eye(sets the current command position)
          bool isLeft - true if is the left eye apparatus, false if it is the right
          int servoType - turnigy
    */
    void setUp(bool isLeft, int servoType) {
      servoType = servoType;
      isLeft = isLeft;
      if (isLeft) {  //LEFT EYE
        //pin numbers
        vertEyePin = vertEyeL;
        horEyePin = horEyeL;

        //Neutral Servo Positions
        neutralVertEye = 79;
        neutralHorEye = 92;

        //Up Down Servo Positions
        upVertEye = leftUpVertEye;
        downVertEye = leftDownVertEye;

        //Left Right Servo Positions
        leftHorEye = leftLeftHorEye;
        rightHorEye = leftRightHorEye;
      }

      else { //RIGHT EYE
        //pin numbers
        vertEyePin = vertEyeR;
        horEyePin = horEyeR;

        //Neutral  Servo Positions
        neutralVertEye = 50;
        neutralHorEye = 32;

        //Up Down Servo Positions
        upVertEye = rightUpVertEye;
        downVertEye = rightDownVertEye;

        //Left Right Servo Positions
        leftHorEye = rightLeftHorEye;
        rightHorEye = rightRightHorEye;
      }
      cmdEye = downVertEye; // for moving eyes up and down
      calcUpDownDiff(); //calculate the difference
      calcDownUpDiff();
      Serial.println("Initializing Eyes");
    }

    //************************************************CALCULATIONS AND CMD SETTTING******************************
    /*
       calcUpDownDiff - calculate how many degrees it takes to move from the down position to the up position
    */
    void calcUpDownDiff() {
      if (isLeft) {
        downToUp = upVertEye - downVertEye; //100-55
      } else {
        downToUp = downVertEye - upVertEye; //75-30
      }
    }

    /*
       calcDownUpDiff - calculate how many degrees it takes to move from the up position to the down position
    */
    void calcDownUpDiff() {
      if (isLeft) {
        upToDown = upVertEye - downVertEye; //100-55
      } else {
        upToDown = downVertEye - upVertEye; //75-30
      }
    }


    /*
      setCmdEye- set the command position of the of the eyes to the given input psoition
          int servoPos - servo position to be set to
    */
    void setCmdEye(int servoPos) {
      cmdEye = servoPos;
    }

    /*
        NOTE: This function was not used or tested in this code but is a more versatile way of setting
              the command positions than the "setCmdEye()" function.
        setCmdEyeVH - set the command position for the vertical command and the horizontal command
              int vertPos - desired vertical servo position to set the command to
              int horPos - desired horizontal servo position to set the command to
    */
    void setCmdEyeVH(int vertPos, int horPos) {
      cmdEyeV = vertPos;
      cmdEyeH = horPos;
    }


    //************************************************BASIC MOVEMENTS/SET TO CERTAIN POSITION******************************
    // ***************These are commands for the eye to move to the most commonly used setpoints.**************************
    /*
      moveVertEyeTo- move the eye's vertical servo to a certain position
                     used to limit the amount of times having to type the "driveServo" function
            int servoPos - desired servo position
    */
    void moveVertEyeTo(int servoPos) {
      driveServo(vertEyePin, servoPos, servoType); // see helper functions for driveServo definition
    }
    /*
       moveHorEyeTo - move the eye's horizontal servo to a certain position
                      used to limit the amount of times having to type the "driveServo" function
               int servoPos - desired servo position
    */
    void moveHorEyeTo(int servoPos) {
      driveServo(horEyePin, servoPos, servoType);
    }

    /*
       moveEyeTo - move both the horizontal and vertical servos to the specified positions
                   Used to simplify the commands used in later functions
            int vertPos -desired vertical servo position
            int horPos - desired horizontal servo position
    */
    void moveEyeTo(int vertPos, int horPos) {
      moveVertEyeTo(vertPos);
      moveHorEyeTo(horPos);
    }

    /*
      neutralEye- move the eye to a neutral position so the eye is looking straight ahead.
                 The angle values in the fuctions act as the mid points for the servos in both
                 the vertical and horizontal directions.
    */
    void neutralEye() {
      moveEyeTo(neutralVertEye, neutralHorEye);
    }

    // Note: The functions within the following four functions could be replaced with the moveEyeTo() function
    //       in order to make the movement more roboust. For example, moveVertEyeTo(upVertEye) could be replaced with
    //       moveEyeTo(upVertEye,neutralHorEye), which would ensure the horizontal servo was not affecting
    //       the vertical movement and the eye would be at the upper vertical limit.

    /*
       upEye - move the eye to look up by commanding to vertical servos to move to the upper limit
    */
    void upEye() {
      moveVertEyeTo(upVertEye);
    }

    /*
       downEye - move the eye to look down by commanding to vertical servos to move to the lower limit
    */
    void downEye() {
      moveVertEyeTo(downVertEye);
    }

    /*
       leftEye - move the eye to look left by commanding to horizontal servos to move to the left limit
    */
    void leftEye() {
      moveHorEyeTo(leftHorEye);
    }

    /*
       rightEye - move the eye to look right by commanding to horizontal servos to move to the right limit
    */
    void rightEye() {
      moveHorEyeTo(rightHorEye);
    }


    //************************************************DYNAMIC MOVMENTS******************************
    /*
       stepEyeUp - Start with the eyes at the down limits and step up to the vertical upper limit
             unsigned long timeNow - time in milliseconds to keep track of
             int moveTime - time for servo to move
             bool isLeft - true if the eye is the left eye, false if it is the right eye
             int degChange - how many degrees the servo moves in each interation (how fast the eye moves)
        return boolean result - true if eye was moved to the upper vertical limit, false otherwise
    */
    boolean stepEyeUp(unsigned long timeNow, int moveTime, bool isLeft, int degChange) {
      boolean result = false;   // return false otherwise
      moveVertEyeTo(cmdEye);      // move the vertical servo to the position stored in the command value (start with eyes at DOWN limitss)
      if ((isLeft && cmdEye >= upVertEye) || (!isLeft && cmdEye <= upVertEye)) {  // depending on which side, if vertical servo is at or past the upper limit, result is true
        result = true;
      } else if (millis() > timeNow + moveTime && cmdEye != upVertEye) {  // if eye is not at the upper limit and enough time has passed from the previous iteration
        if (isLeft) {           // increment or decrement the command value based on if it is the left or right eye
          cmdEye += degChange;
        } else if (!isLeft) {
          cmdEye -= degChange;
        }
        timeNow = millis(); // update the timer
      }
      return result;    // return true if reached or exceeded goal
    }

    /*
        stepEyeDown - step the start with the eyes at the up limits and set down to the vertical min
                 unsigned long timeNow - time in milliseconds to keep track of
                 int moveTime - time for servo to move
                 bool isLeft - true if the eye is the left eye, false if it is the right
                 int degChange - how many degrees the servo moves in each iteration (how fast the eye moves)
             return boolean result - true if eye was moved to the lower vertical limit, false otherwise
    */
    boolean stepEyeDown(unsigned long timeNow, int moveTime, bool isLeft, int degChange) {
      boolean result = false;     //otherwise return false
      moveVertEyeTo(cmdEye);      // move the vertical servo to the position stored in the command value (start with eyes at UP limits)
      if ((isLeft && cmdEye <= downVertEye) || (!isLeft && cmdEye >= downVertEye)) {// depending on which side, if vertical servo is at or past the lower limit, result is true
        result = true;
      } else if (millis() > timeNow + moveTime && cmdEye != downVertEye) {// if eye is not at the lower limit and enough time has passed from the previous iteration
        if (isLeft) {     // increment or decrement the command value by the given amount based on if it is the left or right eye
          cmdEye -= degChange;
        } else if (!isLeft) {
          cmdEye += degChange;
        }
        timeNow = millis(); // update the timer
      }
      return result;  // return true if reached or exceeded goal
    }


    /*
       stepEyeLeft - Start with the eyes at the right limits and step left to the horizontal max
                     Note: Unlike the vertical servos, the horizontal servos increment and decrement together
                           so there does not have to be a discrepancy between the command values for each eye. This was
                           found through testing the mechanical system. In future work, I suggest ensuring
                           the servos are installed in such a way that the servos for the left and right eyes
                           increment and decrement in the same direction.
               unsigned long timeNow - time in milliseconds to keep track of
               int moveTime - time for servo to move
               bool isLeft - true if the eye is the left eye, false if right
               int degChange -  how many degrees the servo moves in each iteration (how fast the eye moves)
          return boolean result - true if eye was moved to the left horizontal limit, false otherwise
    */
    boolean stepEyeLeft(unsigned long timeNow, int moveTime, bool isLeft, int degChange) {
      boolean result = false; //otherwise return false
      moveHorEyeTo(cmdEye); // move the horizontal servo to the position stored in the command value (start with eyes at right limits)
      if (cmdEye <= leftHorEye) { // if horizontal servo is at or past the left limit, result is true
        result = true;
      } else if (millis() > timeNow + moveTime && cmdEye != leftHorEye) { // if horizontal servo is not at the left limit and enough time has passed for the servo to move
        cmdEye -= degChange;  // decrement the command value by the given amount
        timeNow = millis(); // reset the timer
      }
      return result;  // return true if at or exceeded the goal
    }

    /*
           stepEyeRight - step the start with the eyes at the left limits and set right to the horizontal min
                          Note: Unlike the vertical servos, the horizontal servos increment and decrement together
                                         so there does not have to be a discrepancy between the command values for each eye. This was
                                         found through testing the mechanical system. In future work, I suggest ensuring
                                         the servos are installed in such a way that the servos for the left and right eyes
                                         increment and decrement in the same direction.
                 unsigned long timeNow - time in milliseconds to keep track of
                 int moveTime - time for servo to move
                 bool isLeft - true if the eye is the left eye, false if right
                 int degChange -  how many degrees the servo moves in each iteration (how fast the eye moves)
            return boolean result - true if eye was moved to the left horizontal limit, false otherwise
    */
    boolean stepEyeRight(unsigned long timeNow, int moveTime, bool isLeft, int degChange) {
      boolean result = false;//otherwise return false
      moveHorEyeTo(cmdEye);// move the horizontal servo to the position stored in the command value (start with eyes at left limits)
      if (cmdEye >= rightHorEye) {// if horizontal servo is at or past the right limit, result is true
        result = true;
      } else if (millis() > timeNow + moveTime && cmdEye != rightHorEye) {// if horizontal servo is not at the right limit and enough time has passed for the servo to move
        cmdEye += degChange;// increment the command value by the given amount
        timeNow = millis();// reset the timer
      }
      return result;// return true if at or exceeded the goal
    }



    /*
       rollEye - "roll" the eye upward by having the center of the eyeball move/"step" in a semicircle.
                  Start with the eye in the neutral position, then move horizontally to the left limit,
                  thenmove from the left in a curve to the top upper vertical limit, then move from
                  the upper limit to curve down to the right horizontal limit, and then back to the neutral position.
                  Since the horizontalservo might reach the goal before the vertical servo (or vice versa), this code
                  waits until both servos are at the goal before moving to the next movement.
             unsigned long timeNow - time in milliseconds to keep track of
             int moveTime - time for servo to move
             bool isLeft - true if the eye is the left eye, false if right
             int degChange -  how many degrees the servo moves in each iteration (how fast the eye moves)
    */
    void rollEye(unsigned long timeNow, int moveTime, bool isLeft, int degChange) {
      moveEyeTo(cmdEyeV, cmdEyeH);  // command the eye to move/ drive the servo
      // variables to store if the servos are at the desired locations
      boolean horSet;
      boolean vertSetL;
      boolean vertSetR;
      Serial.print(" EYE ROLLING STATE ==  "); Serial.println(eyeRollState); // for debugging
      if (millis() > timeNow + moveTime) {    // if enough time has passed for the servos to move
        switch (eyeRollState) {
          case 0: // move from neutral to the left limit
            if (cmdEyeH <= leftHorEye) {  // if the horizontal command is at or exceeded the left limit, change states
              eyeRollState = 1;
            } else {
              cmdEyeH -= degChange;
            }
            break;
          case 1: // move from left in a curve up to the top
            // update boolean variables
            horSet = cmdEyeH >= neutralHorEye;
            vertSetL = isLeft && (cmdEyeV >= upVertEye);
            vertSetR = !isLeft && (cmdEyeV <= upVertEye);

            if (horSet && (vertSetL || vertSetR)) {// if the horizontal and vertical servos are at the goal or exceeded, change state
              eyeRollState = 2;
            } else {
              if (cmdEyeH < neutralHorEye) {  // increment the horiztonal servo if needed
                cmdEyeH += degChange;
              }
              if ((isLeft && (cmdEyeV < upVertEye))) {  // increment or decrement(if left or right eye) the vertical servo if needed
                cmdEyeV += degChange;
              } else if (!isLeft && (cmdEyeV > upVertEye)) {
                cmdEyeV -= degChange;
              }
            }
            break;

          case 2: // move from  top to curve down to right
            //            Serial.print("HERE YOU ARE = "); Serial.println(eyeRollState); // for debugging
            // update booleans
            horSet = cmdEyeH >= rightHorEye;
            vertSetL = isLeft && (cmdEyeV <= neutralVertEye);
            vertSetR = !isLeft && (cmdEyeV >= neutralVertEye);
            //            Serial.print("booleans  = "); Serial.println(horSet && (vertSetL || vertSetR)); // for debugging
            if ( horSet && (vertSetL || vertSetR)) {// if horiztonal and vertical servos are at the goal or exceeded, change state
              eyeRollState = 3;
            } else {
              if (cmdEyeH < rightHorEye) {// increment horizontal servo if needed
                cmdEyeH += degChange;
              }
              if ((isLeft && (cmdEyeV > neutralVertEye))) { // increment or decrement(if left or right eye) the vertical servo if needed
                cmdEyeV -= degChange;
              } else if (!isLeft && (cmdEyeV < neutralVertEye)) {
                cmdEyeV += degChange;
              }
            }
            break;

          case 3:
            if (cmdEyeH <= neutralHorEye) {
              eyeRollState = 0;
            } else {
              cmdEyeH -= degChange;
            }
            break;
        }
        timeNow = millis();
      }

    }

    // ******************** Older functions **********************************************
    // *Note: The following  functions are  old versions and are not used in this code.
    //
    //
    /*
            Note: This function is an old version and is not used in this code.
            stepEyeUpandDown - step the start with the eye at the down limits and step up to the vertical max
                               position then move back down continuously.
                    unsigned long timeNow - time in milliseconds to keep track of
                    int moveTime - time for servo to move
                    bool isLeft - true if the eye is the left eye, false if right
                    int degChange - how many degrees the servo increments each time (how fast the eye moves)
                return boolean result - true if moved to up position or to the down position
    */
    boolean stepEyeUpandDown(unsigned long timeNow, int moveTime, bool isLeft, int degChange) {
      boolean result = false;
      switch (stepEyeState) {
        case 0:
          //start with eyes at DOWN limits
          moveVertEyeTo(cmdEye);
          //          driveServo(vertEyePin, cmdEye, servoType);
          if (millis() > timeNow + moveTime && cmdEye != upVertEye) {
            if (isLeft) {
              cmdEye += degChange;
            } else {
              cmdEye -= degChange;
            }
            timeNow = millis();
          } else if (cmdEye == upVertEye) {
            result = true;
            stepEyeState = 1;
          }
          break;

        case 1:
          //start with eyes at UP limits
          moveVertEyeTo(cmdEye);
          //          driveServo(vertEyePin, cmdEye, servoType);
          if (millis() > timeNow + moveTime && cmdEye != downVertEye) {
            if (isLeft) {
              cmdEye -= degChange;
            } else {
              cmdEye += degChange;
            }
            timeNow = millis();
          } else if (cmdEye == downVertEye) {
            result = true;
            stepEyeState = 0;
          }
          break;

      }
      return result;
    }

    /*
                Note: This function is an old version and is not used in this code.
                stepEyeLeftandRight - step the start with the eyes at the right limits and setp up to the horizontal max then move back right
                      unsigned long timeNow - time in milliseconds to keep track of
                      int moveTime - time for servo to move
                      bool isLeft - true if the eye is the left eye
                      int degChange - how many degrees the eye moves at each time
    */
    void stepEyeLeftandRight(unsigned long timeNow, int moveTime, bool isLeft, int degChange) {
      switch (stepEyeState) {
        case 0:
          //start with eyes at DOWN limits
          moveHorEyeTo(cmdEye);
          if (millis() > timeNow + moveTime && cmdEye != leftHorEye) {
            if (isLeft) {
              cmdEye += degChange;
            } else {
              cmdEye -= degChange;
            }
            timeNow = millis();
          } else if (cmdEye == leftHorEye) {
            stepEyeState = 1;
          }
          break;

        case 1:
          //start with eyes at UP limits
          moveHorEyeTo(cmdEye);
          //          driveServo(vertEyePin, cmdEye, servoType);
          if (millis() > timeNow + moveTime && cmdEye != rightHorEye) {
            if (isLeft) {
              cmdEye -= degChange;
            } else {
              cmdEye += degChange;
            }
            timeNow = millis();
          } else if (cmdEye == rightHorEye) {
            stepEyeState = 0;
          }
          break;

      }
    }

};
