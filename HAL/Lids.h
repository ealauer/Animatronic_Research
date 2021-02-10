/**********************************************************************************
   Lids.h

   This file contains the class definition, attributes, and methods of the Eyelids
   class. This class controls both the left and right eyelids to make it easier for
   the Head class to control the eyelids as a whole unit. This class includes set
   positions for the eyelidss, methods that move the eyelids between different
   positions (ex. wince), as well as functions that move the eyelids slower.

   Last Edited by Ethan Lauer on 3/28/20
 *********************************************************************************/
#include<Arduino.h>
#include "lid.h"// include single eyelid header file.

class Eyelids {
  private:
    // Right and left Eyelid objects created (see lid.h)
    Eyelid eyeLidR = Eyelid();
    Eyelid eyeLidL = Eyelid();


    // Time Variables (in ms)
    // regular blink
    const unsigned long blinkPause = 150;
    const unsigned long timeBetweenBlink = 5000;
    // lazy blink
    const unsigned long lazyBlinkPause = 350;
    const unsigned long timeBetweenLazyBlink = 3000;
    // fast blink
    const unsigned long fastBlinkPause = 100;
    const unsigned long timeBetweenFastBlink = 1500;
    //long blink
    const unsigned long timeLongClose = 2000;


    unsigned long timeNow = 0; //time variable for moving motors without delays
    const int moveTime = 5;

    // State Variables
    int blinkState = 0;
    int winceState = 0;

    long randNum; // used for random/idel blinking

  public:
    //************************************************INITIALIZE********************************
    //Define the Eyelids
    Eyelids() {
    }

    /*
       setUp - get a random number from an unused analog pin, setup both eyelids, and open both eyes
    */
    void setUp() {
      randomSeed(analogRead(A0)); //analog read an unused pin
      eyeLidR.setUp(false, turnigy);
      eyeLidL.setUp(true, turnigy);
      openBoth();
    }

    //********************************SET POSITIONS*************************************************
    // ***************These are commands for the eyelids to move to the most commonly used setpoints.

    /*
       openPercentBoth - open/close both eyelids to a certain percentage (see lid.h for more details)
            float percent - number from 0-100 indicating how wide open the eyelids should open
    */
    void openPercentBoth(float percent) {
      eyeLidR.openLidPercent(percent);
      eyeLidL.openLidPercent(percent);
    }

    /*
       openBoth - move both pairs of eyelids to full open position
    */
    void openBoth() {
      eyeLidR.openEyelid();
      eyeLidL.openEyelid();
    }

    /*
       closeBoth - move both pairs fo eyelids to full closed position
    */
    void closeBoth() {
      eyeLidR.closeEyelid();
      eyeLidL.closeEyelid();
    }

    /*
       winceLeft - open the right eyelid fully and close the left one
    */
    void winceLeft() {
      eyeLidR.openEyelid();
      eyeLidL.closeEyelid();
    }
    /*
       winceRight - open the left eyelid fully and close the right one
    */
    void winceRight() {
      eyeLidR.closeEyelid();
      eyeLidL.openEyelid();
    }

    //***************************************BLINKING*************************************

    /*
       Blink - generic blinking function that uses millis to determine when to move the servos to simulate a blink
            int timeBtwnBlink - time in milliseconds between each blink
            int timeBlinkPause - time the lids spend in the closed position
            float percentOpen - how wide open the eyes are in the open state
    */
    void Blink(int timeBtwnBlink, int timeBlinkPause, float percentOpen) {
      switch (blinkState) {
        case 0:
          openPercentBoth(percentOpen);     // start with eyelids open
          if (millis() > timeNow + timeBtwnBlink) {   // once the time has passed, switch states
            timeNow = millis();
            blinkState = 1;
          }
          break;

        case 1:
          openPercentBoth(0);           // close the eyelids
          if (millis() > timeNow + timeBlinkPause) {    // once the shorter amount of time has passes, switch cases
            timeNow = millis();
            randNum = random(0.25, 5); //*for idle blink only* recalculate random time between blink (max is exclusive)
            blinkState = 0;
          }
          break;
      }
    }

    /*
       blinkEyes - blink the eyes at a generic, average time and open at 100% (For average, alert person)
    */
    void blinkEyes() {
      Blink(timeBetweenBlink, blinkPause, 100);
    }

    /*
       lazyBlinkEyes  - blink the eyes with a slower pause and only open 50% (for someone in a haze)
    */
    void lazyBlinkEyes() {
      Blink(timeBetweenLazyBlink, lazyBlinkPause, 50);

    }

    /*
       longClose  - blink the eyes fully open and closes for a long time at each position (for someone dazed)
    */
    void longClose() {
      Blink(timeLongClose, timeLongClose, 100);
    }

    /*
       fastBlink - blink eyes at a faster pace and have wide open (for someone who is nervous or hyper alert)
    */
    void fastBlink() {
      Blink(timeBetweenFastBlink, fastBlinkPause, 100);
    }

    //**********************************************WINKING******************************
    /*
       Wink - generic winking function. operates the same as the blink function but takes in a boolean for left or right winking
            boolean isLeftWink - true if you are winking the left eye
            int timeBtwnBlink - time in milliseconds between each wink
            int timeBlinkPause - time the lids spend in the closed position
            float percentOpen - how wide open the eyes are in the open state
    */
    void Wink(boolean isLeftWink, int timeBtwnBlink, int timeBlinkPause, float percentOpen) {
      switch (blinkState) {
        case 0:
          openPercentBoth(percentOpen);// start with eyelids open
          if (millis() > timeNow + timeBtwnBlink) {// once the time has passed, switch states
            timeNow = millis();
            blinkState = 1;
          }
          break;

        case 1:
          if (isLeftWink) {
            eyeLidL.closeEyelid(); // close left eyelid

          } else {
            eyeLidR.closeEyelid(); // close right eyelid
          }
          if (millis() > timeNow + timeBlinkPause) {  // switch back to open state
            timeNow = millis();
            blinkState = 0;
          }
          break;
      }
    }


    /*
       winkLeft - wink the left eye a basic amount (average time between blinks and open 100%)
    */
    void winkLeft() {
      Wink(true, timeBetweenBlink, blinkPause, 100);
    }

    /*
         winkRight - wink the right eye a basic amount(average time between blinks and open 100%)
    */
    void winkRight() {
      Wink(false, timeBetweenBlink, blinkPause, 100);
    }

    // NOTE: more functions for these can be writen based on the state of the simulation
    // but most likely if a patient is not alert, they are unlikely to intentially wink.


    //*********************************************DYNAMIC MOVEMENTS****************************************
    // ************** These are movements that have the eyelidss move at a slower pace,steping between******
    // ************** setpoints to give a more natural, smooth motion.**************************************

    /*
       stepLidsOpenClose - step both the eyelids open and closed with the servos moving at specified increments
                  float percent - how much to open the eyelid when in the "open" position
                  int degChangeOpen - the number of degrees the eyelids should move at a time when opening (how fast the lid opens)
                  int degChangeClose - the number of degrees the eyelids should move at a time when closing (how fast the lid closes)
    */
    void stepLidsOpenClose(float percent, int degChangeOpen, int degChangeClose) {
      eyeLidR.stepLidOpenClose(timeNow, moveTime, percent, degChangeOpen,  degChangeClose);
      eyeLidL.stepLidOpenClose(timeNow, moveTime, percent, degChangeOpen,  degChangeClose);
    }

    /*
       stepLidsVerySlow - move the eyelids open and closed as slow as possible by incrementing by one degree at a time
                            and only have the eyes open at 60 percent (to be used when LOC is below alert)
    */
    void stepLidsVerySlow() {
      stepLidsOpenClose(60, 1, 1);
    }

    /*
          stepLidsSleepy- move the eyelids open and closed at different speeds
                           The eyes open up to 100 percent open by incrementing at 10 degrees at a time
                           and closes at a slower increment of 1 degree at a time (to be used when LOC is below alert and is "sleepy")
    */
    void stepLidsSleepy() {
      stepLidsOpenClose(100, 10, 1);
    }

    //*********************************************REALISTIC MOVEMENTS****************************************

    /*
       idleBlink - blink the eyes with a random pause that can be betwee 0.25-5seconds
                  this is to emulate a more realistic blink from a normal alert human.
                  (see Blink() for details on how randNum is calculated)
    */
    void idleBlink() {
      int timeBtwnBlink = (int)(randNum * 1000);
      Serial.print(" RANDOM TIME BETWEEN BLINK   ==    "); Serial.println(timeBtwnBlink);
      Blink(timeBtwnBlink, blinkPause, 100);
    }

};
