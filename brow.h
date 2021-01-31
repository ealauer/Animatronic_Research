/**********************************************************************************
  brow.h

  This file contains the class definition, attributes, and methods of each individual
  Eyebrow system. Each system contains 2 servos: one for the medial, one for the lateral.
  There are 4 main setpoints for each brow that have been tested: neutral, furrow,
  regular raise, and a slight raise. Given the time constraints, the eyebrows have not
  been programmed to have slower, "step" movements but only move between setpoints.
  These functions can be created by following the similar structure of the functions
  found in the lid.h and eye.h files.

  Last Edited by Ethan Lauer on 3/28/20
 *********************************************************************************/
#include<Arduino.h>

class Eyebrow {
  private:
    // Servo Pins
    int medialPin;
    int lateralPin;

    int servoType;// turnigy

    bool isLeft; // true if left, false if right brow

    //Servo Positions
    //NEUTRAL
    int neutralMedialDown;
    int neutralLateralDown;
    //FURROW
    int furrowMedialUp;
    int furrowLateralUp;
    //REGULAR RAISE
    int regRaiseMedialUp;
    int regRaiseLateralUp;
    //SLIGHT RAISE
    int slightRaiseMedialUp;
    int slightRaiseLateralUp;

  public:
    //************************************************INITIALIZE********************************
    //Define an eyebrow
    Eyebrow() {

    }

    /*
       setUp - set each of the private parameters. Different values are used if it is the left or right eyebrows
           bool isleft - true if is the left eyebrow system
           int servoType - turnigy for all brows
         NOTE: the hard-coded values can be changed to globals if desired
    */
    void setUp(bool isLeft, int servoType) {
      servoType = servoType;
      isLeft = isLeft;
      if (isLeft) {  //LEFT BROW
        //Pin numbers
        medialPin = medialLeft;
        lateralPin = lateralLeft;

        //Servo Positions
        //NEUTRAL
        neutralMedialDown = 72;
        neutralLateralDown = 144;
        //FURROW
        furrowMedialUp = 85;
        furrowLateralUp = 140;
        //REGULAR RAISE
        regRaiseMedialUp = 0;
        regRaiseLateralUp = 108;
        //SLIGHT RAISE
        slightRaiseMedialUp = 37;
        slightRaiseLateralUp = 132;
      }
      else {                    //RIGHT BROW
        //Pin numbers
        medialPin = medialRight;
        lateralPin = lateralRight;

        //Servo Positions
        //NEUTRAL
        neutralMedialDown = 50;
        neutralLateralDown = 10;
        //FURROW
        furrowMedialUp = 37;
        furrowLateralUp = 14;
        //REGULAR RAISE
        regRaiseMedialUp = 112;
        regRaiseLateralUp = 46;
        //SLIGHT RAISE
        slightRaiseMedialUp = 85;
        slightRaiseLateralUp = 22;
      }
      Serial.println("Initializing Eyebrows");
    }

    //************************************************SET POSITIONS********************************
    // ***************These are commands for the brow to move to the most commonly used setpoints.

    /*
      neutralBrow - drive the medial and lateral servos to a neutral  position
    */
    void neutralBrow() {
      driveServo(medialPin, neutralMedialDown, servoType);
      driveServo(lateralPin, neutralLateralDown, servoType);
    }

    /*
      furrowBrow - drive the medial and lateral servos to the furrow position
    */
    void furrowBrow() {
      driveServo(medialPin, furrowMedialUp, servoType);
      driveServo(lateralPin, furrowLateralUp, servoType);
    }


    /*
       regRaiseBrow -drive the medial and lateral servos to the regular raise position
    */
    void regRaiseBrow() {
      driveServo(medialPin, regRaiseMedialUp, servoType);
      driveServo(lateralPin, regRaiseLateralUp, servoType);
    }
   /*
       slightRaiseBrow -drive the medial and lateral servos to the slight raise position
    */
    void slightRaiseBrow() {
      driveServo(medialPin, slightRaiseMedialUp, servoType);
      driveServo(lateralPin, slightRaiseLateralUp, servoType);
    }

};
