/**********************************************************************************

   General Jaw Header file
      Has overall class definition and private and public functions
   Edited by Ethan Lauer on 2/24/20

 *********************************************************************************/
#include<Arduino.h>

class Jaw {
  private:

    // Servo Pins /// change these based on design of jaw system
    int jawLVertPin;
    int jawRVertPin;
    int jawLHorPin;
    int jawRHorPin;
    //Servo Type
    int servoType = towerpro;


    //JAW POSITIONS
    int jawLHorBack = 120;
    int jawLHorForw = 80;
    int jawRHorBack = 60;
    int jawRHorForw = 102;

    int jawLVertClose = 165;
    int jawLVertOpen = 130;
    int jawRVertClose = 34;
    int jawRVertOpen = 80;

    int jawState = 0;

    // Timing variables
    unsigned long timeNow = 0; //time variable for moving motors without delays
    unsigned long timeBtwnEvenTalk = 250; //motor command pulse
    unsigned long timeBtwnFastTalk = 150;

    // keep track of the previous command for each of the servos
    int cmdJawVertL;
    int cmdJawVertR;
    int cmdJawHorL;
    int cmdJawHorR;

    boolean initRun = true;
    int hingeCycleCount = 0;
    int thrustCycleCount = 0;


  public:
    //**************************************************************INITIALIZE AND SETUP*********************************************************
    Jaw() {

    }

    void setUp() {
      jawLVertPin = jawLVert;
      jawRVertPin = jawRVert;
      jawLHorPin = jawLHor;
      jawRHorPin = jawRHor;
      servoType = towerpro;
      neutralMouth();
      Serial.println("Initializing Jaw");
    }

    //*************************************************************************FUNCTIONS*********************************************************

    //***************************************************Basics**************************************

    /*
       moveLeftVert - move servo to specified degree
              int deg - degree you want to move to
    */
    void moveLeftVert(int deg) {
      driveServo(jawLVertPin, deg, servoType);
    }

    /*
      moveRightVert - move servo to specified degree
           int deg - degree you want to move to
    */
    void moveRightVert(int deg) {
      driveServo(jawRVertPin, deg, servoType);
    }


    /*
        moveLeftHor - move servo to specified degree
              int deg - degree you want to move to
    */
    void moveLeftHor(int deg) {
      driveServo(jawLHorPin, deg, servoType);
    }

    /*
      moveRightHor - move servo to specified degree
           int deg - degree you want to move to
    */
    void moveRightHor(int deg) {
      driveServo(jawRHorPin, deg, servoType);
    }


    /*
       openJawPercent- open the jaw
                float percent- floating number of how much you want jaw to be open (ex. open jaw 75%)
    */
    void openJawPercent(float percent) {
      //calculate the  position based on the open and close positions, the given percentage and make into integer
      int leftPos = (int) ((jawLVertClose - jawLVertOpen) * (percent / 100));
      int rightPos = (int) ((jawRVertOpen - jawRVertClose) * (percent / 100));
      //when left jaw servo opens, servo pos is decreasing (the num)
      leftPos = jawLVertClose - leftPos;
      //when right jaw servo opens, servo pos is increasing (the num)
      rightPos = jawRVertClose + rightPos;
      //move the left and right jaw servos to the left/right position

      moveLeftVert(leftPos);
      moveRightVert(rightPos);
    }

    /* thrustJawPercent -thrust the jaw
                float percent - floating percent to thrust (ex. thrust the jaw by 50% of full range of motion)
    */
    void thrustJawPercent(float percent) {
      //Serial.println(((jawLHorForw-jawLHorBack)*(percent/100)));
      //calculate the  position based on the fwd and back positions, the given percentage and make into integer
      int leftPos = (int) ((jawLHorForw - jawLHorBack) * (percent / 100));
      int rightPos = (int) ((jawRHorBack - jawRHorForw) * (percent / 100));
      //when left jaw servo moves fwd, servo pos is decreasing (the num)
      leftPos = jawLHorBack + leftPos;
      //when right jaw servo moves fwd, servo pos is increasing (the num)
      rightPos = jawRHorBack - rightPos;

      moveLeftHor(leftPos);
      moveRightHor(rightPos);
    }



    //******************************************************************** Setpoints******************************
    /*
       move the left and right jaw vertical linkages to a neutral position
    */
    void neutralMouth() {
      openJawPercent(20);
      thrustJawPercent(0);
    }

    /*
       close the jaw by moving the left and right vertical jaw servos
    */
    void closeMouth() {
      openJawPercent(0);
      thrustJawPercent(0);
    }

    /*
       move the left and right jaw vertical linkages to a open position
    */
    void openMouth() {
      openJawPercent(100);
      thrustJawPercent(20);
    }


    //******************************************************************* Dynamic movements******************************************

    //***********************************************************MOVING TO SETPOINTS******************************************

    /*
       openAndClose - open and close jaw quickly by setting to open and closed position
              int timeOpen - the pause time for the mouth to stay open
              int timeClose - the pause time for the mouth to stay closed
              int numCycles - the number of cycles (one cycle = open and close);
            return boolean - true if opened and closed the number of times provided

    */
    boolean openAndClose(int timeOpen, int timeClose, int numCycles) {
      boolean result = false;
      if (hingeCycleCount < numCycles) {
        switch (jawState) {
          case 0: // open jaw
            openMouth();
            if (millis() > timeNow + timeOpen) {
              timeNow = millis();
              jawState = 1;
            }
            break;
          case 1: // close jaw
            closeMouth();
            if (millis() > timeNow + timeClose) {
              timeNow = millis();
              hingeCycleCount++;
              jawState = 0;
            }
            break;
        }
      } else {
        hingeCycleCount = 0;
        result = true;
      }
      return result;
    }


    /*
       regOpenAndClose - open and close mouth with an even time difference betwen open and closed
    */
    void regOpenAndClose() {
      openAndClose(timeBtwnEvenTalk, timeBtwnEvenTalk, 3);
    }

    boolean helloJaw() {
      boolean result = false;
      result = openAndClose(timeBtwnEvenTalk,timeBtwnEvenTalk, 2);
      return result;
    }


    /*
           thrustAndRetract - thrust jaw quickly by setting to thrust fwd pos and back position
                  int timeFwd - the pause time for the mouth to stay fwd
                  int timeBack - the pause time for the mouth to stay back
                  int numCycles - the number of cycles (one cycle = fwd and back);
                return boolean - true if thrustAndRetractthe number of times provided
    */
    boolean thrustAndRetract(int timeFwd, int timeBack, int numCycles) {
      boolean result = false;
      if (thrustCycleCount < numCycles) {
        switch (jawState) {
          case 0: // open jaw
            thrustJawPercent(100);
            if (millis() > timeNow + timeFwd) {
              timeNow = millis();
              jawState = 1;
            }
            break;
          case 1: // close jaw
            thrustJawPercent(0);
            if (millis() > timeNow + timeBack) {
              timeNow = millis();
              thrustCycleCount++;
              jawState = 0;
            }
            break;
        }
      } else {
        thrustCycleCount = 0;
        result = true;
      }
      return result;
    }





    //**************************************************************************SLOW MOVENTS (STEPPING)***************************************************


    /*
       stepVertOpen - step the jawtp opens up slowly.
              int degChange - how big of a degree change to move the jaw servos
              return boolean - true if both sides of the jaw have stepped to the open position
    */
    boolean stepVertOpen(int degChange) {
      boolean result = false;
      // if initial run, set points to the close position
      if (initRun) {
        cmdJawVertL = jawLVertClose;
        cmdJawVertR = jawRVertClose;
        initRun = false;
      }
      // drive servos
      moveLeftVert(cmdJawVertL);
      moveRightVert(cmdJawVertR);
      // if left and right jaws are not at the open positions
      if (cmdJawVertL <= jawLVertOpen && cmdJawVertR >= jawRVertOpen) {
        result = true;
      } else if (millis() > timeNow + 100) {
        if (cmdJawVertL != jawLVertOpen) {
          cmdJawVertL -= degChange;
        }
        if (cmdJawVertR != jawRVertOpen) {
          cmdJawVertR += degChange;
        }
        timeNow = millis();
      }
      return result;
    }


    /*
           stepVertClose - step the jaw closes  slowly.
                  int degChange - how big of a degree change to move the jaw servos
                  return boolean - true if both sides of the jaw have stepped to the close position
    */
    boolean stepVertClose(int degChange) {
      boolean result = false;
      if (initRun) {
        cmdJawVertL = jawLVertOpen;
        cmdJawVertR = jawRVertOpen;
        initRun = false;
      }

      moveLeftVert(cmdJawVertL);
      moveRightVert(cmdJawVertR);
      if ( cmdJawVertL >= jawLVertClose && cmdJawVertR <= jawRVertClose) {
        result = true;
      } else if (millis() > timeNow + 100) {
        if (cmdJawVertL != jawLVertClose) {
          cmdJawVertL += degChange;
        }
        if (cmdJawVertR != jawRVertClose) {
          cmdJawVertR -= degChange;
        }
        timeNow = millis();
      }
      return result;
    }

    /*
       stepVertOpenClose- step the jaw open and close and continue to repeat this
            int degChange - the degree change for both moving up and down
            int numCycles - the number of cycles (one cycle = open and close);
            return boolean - true if opened and closed the number of times provided
    */
    boolean stepVertOpenClose(int degChange, int numCycles) {
      boolean result = false;
      boolean isSet = false;
      if (hingeCycleCount < numCycles) {
        switch (jawState) {
          case 0:
            Serial.println("opening jaw");
            isSet = stepVertOpen(degChange);
            if (isSet) {
              isSet = false;
              jawState = 1;
            }
            break;
          case 1:
            Serial.println("closing jaw");
            isSet = stepVertClose(degChange);
            if (isSet) {
              hingeCycleCount++;
              isSet = false;
              jawState = 0;
            }
            break;
        }
      } else {
        hingeCycleCount = 0;
        result = true;
      }
      return result;
    }


    boolean helloJawSlow() {
      return stepVertOpenClose(3, 2);
    }






};
