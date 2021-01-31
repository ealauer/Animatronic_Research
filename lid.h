/**********************************************************************************

   General Eyelid Header file
      Has overall class definition and private and public functions
         Edited by Ethan Lauer on 10/9/19


 *********************************************************************************/

#include <Arduino.h>

class Eyelid {
  private:
    // Servo Pins
    int topLidPin;
    int botLidPin;


    // SERVO POSITIONS
    int botClosePos;
    int botOpenPos;

    int topClosePos;
    int topOpenPos;

    //left or right eyelid
    bool isLeft;

    //Servo Type
    int servoType;
    int cmdLid = 0;
    int stepLidState = 0;


  public:
    //***********************************************SETUP AND INITIALIZE*************************************************
    // Define the eyelid
    Eyelid() {

    }

    void setUp(bool  isLeft, int servoType) {
      isLeft = isLeft;
      servoType = servoType;
      if (isLeft) { //left eye lids

        // Set servo pins
        topLidPin = topEyelidL;
        botLidPin = botEyelidL;

        //Bottom Servo Positions
        botClosePos = 97;
        botOpenPos = 113;

        //Top Servo Positions
        topClosePos = 135;
        topOpenPos = 75;

      } else {           //right lids
        // Set servo pins
        topLidPin = topEyelidR;
        botLidPin = botEyelidR;

        //Bottom Servo Positions
        botClosePos = 133;
        botOpenPos = 115;

        //Top Servo Positions
        topClosePos = 60;
        topOpenPos = 125;

      }
      Serial.println("Initializing Eyelids");
    }


    //***********************************************SET POSITION*************************************************
    /*
       moveTopLidTo - move top lid to a set servo position
            int servoPos - servo position of the top servo
    */
    void moveTopLidTo(int servoPos) {
      driveServo(topLidPin, servoPos, servoType);
    }

    /*
       moveBotLidTo - move bot lid to a set servo position
            int servoPos - servo position of the top servo
    */
    void moveBotLidTo(int servoPos) {
      driveServo(botLidPin, servoPos, servoType);
    }


    /*
      openLidPercent -open the lids
      float percent - percentage they eyes are set to be open (ie. 75 is 75% open eyes)
          based on the open and closed position of both the top and bottom servos
          find the position of the "percentage point". once that point is found withing that range,
          add it to top/bottom positions to it moves in reference to the top or bottom.
          drive the servos
    */
    void openLidPercent(float percent) {
      int topPos;
      int botPos;
      if (isLeft) { // calcualted top and bottom positions
        topPos = (int) (-1 * ((topClosePos - topOpenPos) * (percent / 100)));
        botPos = (int) ((botOpenPos - botClosePos) * (percent / 100));
        topPos = topClosePos + topPos;
        botPos = botClosePos + botPos;

        // Drive the servos
        moveTopLidTo(topPos);
        moveBotLidTo(botPos);

      } else { // calcualted top and bottom positions
        topPos = (int) ((topOpenPos - topClosePos) * (percent / 100));
        botPos = (int) (-1 * ((botClosePos - botOpenPos) * (percent / 100)));
        topPos = topClosePos + topPos;
        botPos = botClosePos + botPos;
        
        // Drive the servos
        moveTopLidTo(topPos);
        moveBotLidTo(botPos);
      }
    }


    /*
      closeEyelid - close the eyelids
    */
    void closeEyelid() {
      moveTopLidTo(topClosePos);
      moveBotLidTo(botClosePos);
    }


    /*
        openEyelid-  open the eyelid
    */
    void openEyelid() {
      moveTopLidTo(topOpenPos);
      moveBotLidTo(botOpenPos);
    }

    //***********************************************DYNAMIC MOVEMENTS*************************************************


    /*
       stepLidOpenClose - slowly open and close the eyelid
              unsigned long timeNow - time in milliseconds so can count if need to increment eyes
              int moveTime - time it takes to move the servo
              float percent - how much to open the eyelid
              int degChangeOpen - the number of degrees the eyelids should move at a time when opening
              int degChangeClose - the number of degrees the eyelids should move at a time when closing

    */
    void stepLidOpenClose(unsigned long timeNow, int moveTime, float percent, int degChangeOpen, int degChangeClose) {
      switch (stepLidState) {
        case 0:
          openLidPercent(cmdLid);
          if (millis() > timeNow + moveTime && cmdLid != percent) {
            cmdLid += degChangeOpen;
            timeNow = millis();
          } else if (cmdLid == percent) {

            stepLidState = 1;
          }

          break;

        case 1:
          openLidPercent(cmdLid);
          if (millis() > timeNow + moveTime && cmdLid != 0) {
            cmdLid -= degChangeClose;
            timeNow = millis();
          } else if (cmdLid == 0) {
            stepLidState = 0;
          }
          break;

      }
    }


};
