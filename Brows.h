/**********************************************************************************
   Brows.h

   This file contains the class definition, attributes, and methods of the Eyebrows
   class. This class controls both the left and right eyebrows to make it easier for
   the Head class to controll the eyebrows as a whole unit. This class includes set
   positions for the eyebrows, and two methods that move the brows between two
   different positions.

   Last Edited by Ethan Lauer on 3/28/20
 *********************************************************************************/

#include<Arduino.h>
#include "brow.h"   // include single eyebrow header file.

class Eyebrows {
  private:
    // Right and left Eyebrow objects created (see brow.h)
    Eyebrow eyeBrowR = Eyebrow();
    Eyebrow eyeBrowL = Eyebrow();

    // Time Variables (in ms)
    unsigned long furrowPause = 250;  // The time the eyebrows stay in the furrow position
    unsigned long slightPause = 550;
    unsigned long timeBetweenFurrow = 4000;
    unsigned long timeNow = 0; //time variable for moving motors without delays
    int moveTime = 5;
    // State variable used to switch between two different brow states
    int browState = 0;

  public:
    //************************************************INITIALIZE********************************
     //Define the Eyebrows
    Eyebrows() {
    }

    /*
       setUp - set up both individual eyebrow systems and set the positions to neutral
    */
    void setUp() {
      eyeBrowR.setUp(false, turnigy);
      eyeBrowL.setUp(true, turnigy);
      neutralBoth();
    }

    //************************************************SET POSITIONS********************************
    // These are some useful,common positions the eyeberows are set to

    /*
       neutralBoth - set both eyebrows to a neutral position
    */
    void neutralBoth() {
      eyeBrowR.neutralBrow();
      eyeBrowL.neutralBrow();
    }

    /*
       regRaiseBoth - raise both eyebrows the regular height
    */
    void regRaiseBoth() {
      eyeBrowR.regRaiseBrow();
      eyeBrowL.regRaiseBrow();
    }

    /*
       slightRaiseBoth - raise both eyebrows only a little bit
    */
    void slightRaiseBoth() {
      eyeBrowR.slightRaiseBrow();
      eyeBrowL.slightRaiseBrow();
    }

    /*
       furrowBoth - furrow both eyebrows
    */
    void furrowBoth() {
      eyeBrowR.furrowBrow();
      eyeBrowL.furrowBrow();
    }

    /*
       raiseLeftFurrowRight - raise the left eyebrow regularly and furrow the right eyebrow
    */
    void raiseLeftFurrowRight() {
      eyeBrowR.furrowBrow();
      eyeBrowL.regRaiseBrow();

    }

    /*
       raiseRightFurrowLeft - raise the right eyebrow regularly and furrow the left eyebrow
    */
    void raiseRightFurrowLeft() {
      eyeBrowR.regRaiseBrow();
      eyeBrowL.furrowBrow();
    }

    //************************************************COMBO MOVES********************************
    // The eyebrows are moved between two setpoints based on the time passed, measured by millis()

    /*
       raiseAndFurrow - move the eyebrows between two different state
                raise the eyebrows then furrow then return after the correct time
    */
    void raiseAndFurrow() {
      switch (browState) {
        case 0:
          regRaiseBoth();
          if (millis() > timeNow + timeBetweenFurrow) {
            timeNow = millis();
            browState = 1;
          }
          break;
        case 1:
          furrowBoth();

          if (millis() > timeNow + furrowPause) {
            timeNow = millis();
            browState = 0;
          }
          break;
      }
    }


    /*
       raiseAndSlightRaise - move the eyebrows between two different state
                raise the eyebrows then lower them slightly then return after the correct time
    */
    void raiseAndSlightRaise() {
      switch (browState) {
        case 0:
          regRaiseBoth();

          if (millis() > timeNow + timeBetweenFurrow) {
            timeNow = millis();
            browState = 1;
          }
          break;

        case 1:
          slightRaiseBoth();
          if (millis() > timeNow + slightPause) {
            timeNow = millis();
            browState = 0;
          }
          break;
      }
    }


};
