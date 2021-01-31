/**********************************************************************************

   General Neck Header file
      Has overall class definition and private and public functions
         Edited by Ethan Lauer on 1/17/20

 *********************************************************************************/
#include<Arduino.h>

class Neck {

  private:

    // SERVO PINS
    int rightServoPin;
    int leftServoPin;
    int rotServoPin;

    int servoType;


    // SENSOR PINS
    const int neckPotPinR = A13;
    const int neckPotPinL = A12;
    const int neckPotPinRot = A1;

    // MOVING VARIABLES
    boolean initRunSide = true;
    boolean initRunRot = true;
    int cmdServo = 0;
    int cmdServoR = 0; // stores the command that was sent to the right servo
    int cmdServoL = 0;// stores the command that was sent to the left servo
    int cmdServoRot = 0;// stores the command that was sent to the rotational servo
    int state = 0;    // state machine variable
    int stateTest = 0;
    const int tol = 2; // tolerance for feedback and movements
    int nodCount = 0; // keeps track of the number of times nodded
    int tiltCount = 0; // keeps track of the number of times tilted


    // TIMING VARIABLES
    const int moveTime = 20; // time delay before incrementing to the next degree
    unsigned long timeNow = 0; //time variable for moving motors without delays
    const int neckMotorCmdPause = 10;
    const int normTimeBtwnNod = 250;


    // SERVO POSITIONS
    const int rotCenterPos = 35;

    const int nodBackMax = 50;
    const int nodFwdMax = 150;

    const int tiltRightMax = 50; // servo position for tilting to the right
    const int tiltLeftMax = 160;// servo position for tilting to the left
    const int neutralPos = 100; // neutral position for left and right servos.

    // Calibration values
    const int minDeg = 0;
    const int midDeg = 135;
    const int maxDeg = 270;
    int minFeedback = 335; // found thorugh calibration testing
    int minFBs[2];
    int maxFeedback = 1023; // found thorugh calibration testing
    int maxFBs[2];

  public:
  
    const int rotLeftMax = 80;
    const int rotRightMin = 0;
    //********************************************************* SETUP AND INITIALIZE*********************
    Neck() { // pass in pin numbers here and servo types
    }

    void setUp() {
      // initialize and define servo pins
      rightServoPin = rightTiltPin;
      leftServoPin = leftTiltPin;
      rotServoPin = rotPin;
      servoType = dss;

      // SENSOR PINS Analog pot pins for neck control
      pinMode(neckPotPinR, INPUT);
      pinMode(neckPotPinL, INPUT);
      pinMode(neckPotPinRot, INPUT);

      neutral();
      Serial.println("Initializing Neck");
    }


    //**************************************************************CONVERSIONS**********************************************

    /*
      feedbackToDeg- converstion from the raw feedback into degrees of the servo
          integer rawFB - the raw data from the servo feedback potentiometer pins.
          return a double of the degrees
    */
    double feedbackToDeg(int rawFB) {
      double result = abs(map(rawFB, minFeedback, maxFeedback, minDeg, maxDeg));
      return result;
    }


    /*
       rDegToLDeg - convert the right servo degree to left  and return
          integer deg - degree for the right servo to move
              note - offset with 90 was added because the left servo was not cooperating - unknown why
    */
    int rDegToLDeg(int deg) {
      int result = abs(maxDeg - deg - 60);
      return result;
    }

    //*******************************************************************MOVING FUNCTIONS********************************************************


    //******************************************************************Basics******************************

    /*
          moveRightServo - Move right servo to a certainn degree
               integer Deg - the goal degree you want to move to.
    */
    void moveRightServo(int deg) {
      driveServo(rightServoPin, deg, dss);
    }

    /*
          moveLeftServo - Move left servo to a certainn degree
               integer Deg - the goal degree you want to move to.
    */
    void moveLeftServo(int deg) {
      driveServo(leftServoPin, deg, dss);
    }

    /*
         moveRotServo - rotate the head to a certainn degree
              integer Deg - the goal degree you want the head to turn to.
    */
    void moveRotServo(int deg) {
      driveServo(rotServoPin, deg, dss);
    }


    //***********************************************************************Combined******************************

    /*
          rotToDeg - rotate servo to a set point and ensure it made it there using a boolean.
                      can be used to move from any location.
               integer goalDeg - the goal position of rotation servo
                           int degChange - how much you want the servo to move by (speed)

               return true if it successfully moved to that degree
    */
    boolean rotToDeg(int goalDeg,int degChange) {
      boolean result = false;
      int currPosRot;
      int errRot;
      if (initRunRot) {
        currPosRot = (int)feedbackToDeg(analogRead(neckPotPinRot));
        Serial.print("Rotational Reading = ");  Serial.println(currPosRot);
        cmdServoRot = currPosRot;
        initRunRot = false;
      }
      errRot = goalDeg - cmdServoRot;
//      Serial.print("        Rotation ERR = ");  Serial.println(errRot);
      moveRotServo(cmdServoRot);
      // if engough time has passed for the servos to move
      // check the errors to determine which direcetion the servos should move and if they need to move more at all
      if (millis() > timeNow + moveTime) {
        if (errRot >= tol) {
          cmdServoRot += degChange;
        } else if (errRot <= -tol) {
          cmdServoRot -= degChange;
        }
        timeNow = millis();
      }
      // if you are within the tolerance, then return true
      if (errRot < tol && errRot > -tol) {
        result = true;
        initRunRot = true;
//        Serial.print("result = ");  Serial.println(result);
        //        delay(2000);
      }

      return result;
    }



    /*
       moveToDeg - move the left and right servos to a set point and ensure it made it there using a boolean.
                   can be used to move from any location.
            integer goalDeg - the goal position based on the right servo ( convertes to left servo goal in the fnction)
            boolean moveTogether - true if you want the servos to line up with each other whereever they are, false if you want them opposite.
            int degChange - how much you want the servos to move by (speed)
            return true if it successfully moved to that degree
    */
    boolean moveToDeg(int goalDeg, boolean moveTogether, int degChange) {
      boolean result = false;      // initialize result to return false
      int leftGoalDeg;
      if (moveTogether) { // if you want the servos to line up with one another
        leftGoalDeg = rDegToLDeg(goalDeg); // calculate the left goal
      } else {
        leftGoalDeg = goalDeg;
      }
      // initialize varibles fro the current positions and errors
      int currPosR;
      int currPosL;
      int errR;
      int errL;
      // if it is the initial run , find out where the servos are currently and set that to the current cmd
      if (initRunSide) {
        currPosR = (int)feedbackToDeg(analogRead(neckPotPinR));
        currPosL = (int)feedbackToDeg(analogRead(neckPotPinL));
        Serial.print("Right Reading = ");  Serial.println(currPosR);
        Serial.print("LEft Reading = ");  Serial.println(currPosL);
        cmdServoR = currPosR;
        cmdServoL = currPosL;
        initRunSide = false;
      }
      // Calculate the error between the goal degree and the command number
      errR = goalDeg - cmdServoR;
      errL = leftGoalDeg - cmdServoL;
//      Serial.print("        Right ERR = ");  Serial.println(errR);
//      Serial.print("        LEft ERR = ");  Serial.println(errL);

      // move both the servos
      moveRightServo(cmdServoR);
      moveLeftServo(cmdServoL);
      // if engough time has passed for the servos to move
      // check the errors to determine which direcetion the servos should move and if they need to move more at all
      if (millis() > timeNow + moveTime) {
        if (errR >= tol) {
          cmdServoR += degChange;
        } else if (errR <= -tol) {
          cmdServoR -= degChange;
        }
        if (errL >= tol) {
          cmdServoL += degChange;
        } else if (errL <= -tol) {
          cmdServoL -= degChange;
        }
        timeNow = millis();
      }
      // if you are within the tolerance, then return true and set initial run back to true
      if (errR < tol && errR > -tol && errL < tol && errL > -tol) {
        result = true;
        //        initRunSide = true;
//        Serial.print("result = ");  Serial.println(result);
      }
      return result;
    }



    /*
       moveBtwnPts - have the neck move betweentwo points based on time
            int startPos - the starting position you want the nod to start at (one of the setpoints you want move between)
            int goalDeg - other setpoint you want to
             boolean moveTogether - true for together
           return boolean - true if it completed one round (ie. moved to startPos and goalDeg)
    */
    boolean moveBtwnPts(int startPos, int goalDeg, boolean moveTogether) {
      int result = false;
      switch (state) {
        case 0:
          if (moveToDeg(startPos, moveTogether,2)) {
            state = 1;
          }
          break;
        case 1:
          if (moveToDeg(goalDeg, moveTogether,2)) {
            state = 0;
            result = true;
          }
          break;
      }
      return result;
    }


    /*
          rotBtwnPts - have the neck rotate between two points based on time
               int startPos - the starting position you want the nod to start at (one of the setpoints you want move between)
               int goalDeg - other setpoint you want to
              return boolean - true if it completed one round (ie. moved to startPos and goalDeg)
    */
    boolean rotBtwnPts(int startPos, int goalDeg) {
      int result = false;
      switch (state) {
        case 0:
          if (rotToDeg(startPos,2)) {
            state = 1;
          }
          break;
        case 1:
          if (rotToDeg(goalDeg,2)) {
            state = 0;
            result = true;
          }
          break;
      }
      return result;
    }







    //******************************************************************************Complex***************************************

    /*
       neutral - move neck to neutral position
    */
    boolean neutral() {
      boolean result = false;
      boolean sideResult = false;
      boolean rotResult = false;

      sideResult = moveToDeg (neutralPos, true,2);
      rotResult = rotToDeg(rotCenterPos,2);
      result = sideResult && rotResult;
      return result;
    }

    /*
       nod - generic nod to a back and forth with no end until function is not called (place inside a loop).
    */
    void nod() {
      boolean result;
      result = moveBtwnPts(nodFwdMax, nodBackMax, true);
    }

    /*
       tilt - tilt head left and right with no end until function is not calle (place this inside a loop)
    */
    void tilt() {
      moveBtwnPts(tiltLeftMax, tiltRightMax, false);
    }


    /*
       wince - have the neck wince/bend toward one of the given sides and rotate away from that side
          boolean isLeft - true if wincing/tilting towards the left, false if towards the right
    */
    boolean wince(boolean isLeft) {
      boolean result;
      if (isLeft) {
        result = moveToDeg(tiltLeftMax, false,5);
      } else {
        result = moveToDeg(tiltRightMax, false,5);
      }
      rotToDeg(rotCenterPos,2);
      return result;
    }

    /*
       nodTimes - have the neck nod a forward and back at a certain speed for a given number of times
       moving to the maximum locations
          int numNods - the number of times the neck should be nodding
          int timeBtwnNod - how long between the fwd and back
          return boolean - true if finished nodding
    */
    boolean nodTimes(int numNods, int timeBtwnNod) {
      boolean result = false;
      if (nodCount < numNods) {
        boolean oneCycle = moveBtwnPts(nodFwdMax, nodBackMax, true);
        if (oneCycle) {
          nodCount++;
          delay(timeBtwnNod);
        }
      } else {
        nodCount = 0;
        Serial.print("FINISHED NODDING ");  Serial.print(numNods);  Serial.println(" Times");
        result = true;
        delay(2000); /// remove eventually

      }
      return result;
    }


    /*
           tiltTimes - have the neck tilte a left and right at a certain speed for a given number of times
                  moving to the maximum locations
              int numTilts - the number of times the neck should be tilting
              int timeBtwnTilt - how long between the left and right
              return boolean - true if finished nodding
    */
    boolean tiltTimes(int numTilts, int timeBtwnTilt) {
      boolean result = false;
      if (tiltCount < numTilts) {
        boolean oneCycle = moveBtwnPts(tiltRightMax, tiltLeftMax, false);
        if (oneCycle) {
          tiltCount++;
          delay(timeBtwnTilt);
        }
      } else {
        tiltCount = 0;
        Serial.print("FINISHED TILTING ");  Serial.print(numTilts);  Serial.println(" Times");
        result = true;
        delay(2000); /// remove eventually
      }
      return result;

    }


    /*
          neckTest - have neck nod back, forward, tilt right, left
                  test moving to multiple setpoints to see the range of motion
    */
    void neckTest() {
      switch (stateTest) {
        case 0:
          if (moveToDeg(nodBackMax, true,2)) { // nodded back
            stateTest = 1;
            delay(1000);
          }
          break;
        case 1:
          if (moveToDeg(nodFwdMax, true,2)) { // nodded forward
            stateTest = 2;
            delay(1000);
          }
          break;
        case 2:
          if (moveToDeg(tiltRightMax, false,2)) { // tilted right
            stateTest = 3;
            delay(1000);
          }
          break;
        case 3:
          if (moveToDeg(tiltLeftMax, false,2)) { // tilted left
            stateTest = 4;
            delay(1000);
          }
          break;
        case 4:
          if (moveToDeg(neutralPos, true,2)) { // move to neutral position
            stateTest = 0;
            delay(1000);
          }
          break;
      }
    }






    //****************************************OLDER FUNCTIONS***************************************
    //***************************************************************************************************
    /*
       driveRandL - function to move both servos and print any feedback
           int rightPos - right servo desired position
           boolean moveTogether - true if you want to move the servos together , false if they are opposite
    */
    void driveRandL(int rightPos, boolean moveTogether) {
      int currPosL;
      if (moveTogether) {
        currPosL = rDegToLDeg(rightPos);
      } else {
        currPosL = rightPos;
      }
      Serial.print("                          Left POSITION = ");  Serial.println(currPosL);
      moveRightServo(rightPos);
      moveLeftServo(currPosL);  // drive the servo
      Serial.print("Rotating to "); Serial.println(rightPos);
    }

    /*
       moveRandL - use for loops to move left and right servos together either forward or back
               int startPos - starting position of the servos
            int goalDeg - goal degree to move too
            boolean moveTogether - true if the servos should move together (nod), false if opposite (tilt)
    */
    boolean moveRandL(int startPos, int goalDeg, boolean moveTogether) {
      // current feedback/reading
      int currPos;
      moveRightServo(startPos);
      moveLeftServo(rDegToLDeg(startPos));
      if (startPos < goalDeg) {
        for (currPos = startPos; currPos < goalDeg; currPos++) {
          driveRandL(currPos, moveTogether);
        }
      } else if (startPos > goalDeg) {
        for (currPos = startPos; currPos >= goalDeg; currPos--) {
          driveRandL(currPos, moveTogether);
        }
      }
      return true;
    }


    /*
       rotAndTilt - rotate the head and tilt the head to a certain set point for each
            int startRotPos - starting servo position for the rotation servo
            int goalRotDeg - goal degree position for the servo to rotate to
            int startSidePos - starting servo position for the right servo left servo is calculated afterwards)
            int goalSidePos - end servo position for the right servo left servo is calculated afterwards)
            boolean moveTogether - true if the left and right servos shoudl move together(nod), false if they are opposite (tilt)
    */
    void rotAndTilt(int startRotPos, int goalRotDeg, int startSidePos, int goalSidePos, boolean moveTogether) {
      int currPosRot = startRotPos;
      int currPosSide = startSidePos;
      moveRightServo(startSidePos);
      moveLeftServo(rDegToLDeg(startSidePos));
      moveRotServo(startRotPos);

      // calculate which difference is bigger, the rotation or the tilt diff
      int rotDiff = abs(goalRotDeg - startRotPos);
      int sideDiff = abs(goalSidePos - startSidePos);

      if (sideDiff > rotDiff) {// SIDE MOVES MORE THAN ROT
        Serial.println("SIDE MOVES MORE THAN ROT");
        if (startRotPos < goalRotDeg && startSidePos < goalSidePos) {               // if both SIDE and rot servos need to increment
          Serial.println("side AND rot servos are  =  INCREASING .......bigger SIDE diff");
          for (currPosSide = startSidePos; currPosSide < goalSidePos; currPosSide++) {
            driveRandL(currPosSide, moveTogether);
            if (currPosRot >= goalRotDeg) {  //  if it went too high past the goal, stay at the goal
              moveRotServo(goalRotDeg);
            } else {
              moveRotServo(currPosRot); // otherwise go up to the goal
              currPosRot++;
            }
            //            delay(neckMotorCmdPause);
          }
        } else if (startRotPos > goalRotDeg && startSidePos > goalSidePos) {            // if both right and rot servos need to decrement
          Serial.println("side AND rot servos are  =  DECREASING .......bigger SIDE diff");
          for (currPosSide = startSidePos; currPosSide > goalSidePos; currPosSide--) {
            driveRandL(currPosSide, moveTogether);
            if (currPosRot <= goalRotDeg) {   //  if it went too low past the goal, stay at the goal
              moveRotServo(goalRotDeg);
            } else {
              moveRotServo(currPosRot);// otherwise go down to the goal
              currPosRot--;
            }
            //            delay(neckMotorCmdPause);
          }
        } else if (startRotPos > goalRotDeg && startSidePos < goalSidePos) {            // rot servo needs to decrement, side servos increment
          Serial.println("side servos   =  INCREASING !!!!!  ROT servos  =  DECREASING !!...bigger SIDE diff");
          for (currPosSide = startSidePos; currPosSide < goalSidePos; currPosSide++) {
            driveRandL(currPosSide, moveTogether);
            if (currPosRot <= goalRotDeg) {   //  if it went too low past the goal, stay at the goal
              moveRotServo(goalRotDeg);
            } else {
              moveRotServo(currPosRot);// otherwise go down to the goal
              currPosRot--;
            }
            //            delay(neckMotorCmdPause);
          }
        } else if (startRotPos < goalRotDeg && startSidePos > goalSidePos) {          // rot servo needs to increment, side servos decrement
          Serial.println("side servos   =  DECREASING !!!!!  ROT servos  =  INCCREASING !!...bigger SIDE diff");
          for (currPosSide = startSidePos; currPosSide > goalSidePos; currPosSide--) {
            driveRandL(currPosSide, moveTogether);
            if (currPosRot >= goalRotDeg) {   //  if it went too high past the goal, stay at the goal
              moveRotServo(goalRotDeg);
            } else {
              moveRotServo(currPosRot);   // otherwise go up to the goal
              currPosRot++;
            }
            //            delay(neckMotorCmdPause);
          }
        }
      }
      else if (sideDiff <= rotDiff) {// ROT MOVES MORE THAN SIDE
        Serial.println("ROT MOVES MORE THAN SIDE");
        if (startRotPos < goalRotDeg && startSidePos < goalSidePos) {             // if both right and rot servos need to increment
          Serial.println("side AND rot servos are  =  INCREASING .......bigger ROT diff");
          for (currPosRot = startRotPos; currPosRot < goalRotDeg; currPosRot++) {
            moveRotServo(currPosRot);
            if (currPosSide >= goalSidePos) {  // if you went too high past the goal, then stay at the goal
              driveRandL(goalSidePos, moveTogether);
            } else {
              driveRandL(currPosSide, moveTogether);// otherwise go up to the goal
              currPosSide++;
            }
            //            delay(neckMotorCmdPause);
          }
        } else if (startRotPos > goalRotDeg && startSidePos > goalSidePos) {            // if both right and rot servos need to decrement
          Serial.println("side AND rot servos are  =  DECREASING .......bigger ROT diff");
          for (currPosRot = startRotPos; currPosRot > goalRotDeg; currPosRot--) {
            moveRotServo(currPosRot);
            if (currPosSide <= goalSidePos) {   // if you went too low past the goal, then stay at the goal
              driveRandL(goalSidePos, moveTogether);
            } else {
              driveRandL(currPosSide, moveTogether);// otherwise go down to the goal
              currPosSide--;
            }
            //            delay(neckMotorCmdPause);
          }
        } else if (startRotPos > goalRotDeg && startSidePos < goalSidePos) {         // rot servo needs to decrement, side servos increment
          Serial.println("side servos   =  INCREASING !!!!!  ROT servos  =  DECREASING !!...bigger ROT diff");
          for (currPosRot = startRotPos; currPosRot > goalRotDeg; currPosRot--) {
            moveRotServo(currPosRot);
            if (currPosSide >= goalSidePos) {  // if you went too high past the goal, stay at the goal
              driveRandL(goalSidePos, moveTogether);
            } else {
              driveRandL(currPosSide, moveTogether); // otherwise, go up to the goal
              currPosSide++;
            }
            //            delay(neckMotorCmdPause);
          }
        } else if (startRotPos < goalRotDeg && startSidePos > goalSidePos) {          // rot servo needs to increment, side servos decrement
          Serial.println("side servos   =  DECREASING !!!!!  ROT servos  =  INCCREASING !!...bigger ROT diff");
          for (currPosRot = startRotPos; currPosRot < goalRotDeg; currPosRot++) {
            moveRotServo(currPosRot);
            if (currPosSide <= goalSidePos) {  // if you went too low past the goal, stay at the goal
              driveRandL(goalSidePos, moveTogether);
            } else {
              driveRandL(currPosSide, moveTogether);// otherwise, go down to the goal
              currPosSide--;
            }
            //            delay(neckMotorCmdPause);
          }
        }
      }
    }



    //**************************************************************TESTING and BIG MOVEMENTS******************************************************
    //    /*
    //       neckTest - nod back, forward, tilt right, left
    //    */
    //    void neckTest() {
    //      switch (stateTest) {
    //        case 0:
    //          if (moveRandL(nodFwdMax, nodBackMax,  true)) {
    //            stateTest = 1;
    //            delay(1000);
    //          }
    //          break;
    //        case 1:
    //          if (moveRandL( nodBackMax, nodFwdMax, true)) {
    //            stateTest = 2;
    //            delay(1000);
    //          }
    //          break;
    //
    //        case 2:
    //          if (moveRandL(tiltRightMax, tiltLeftMax,  false)) {
    //            stateTest = 3;
    //            delay(1000);
    //          }
    //          break;
    //        case 3:
    //          if (moveRandL(tiltLeftMax, tiltRightMax,  false)) {
    //            stateTest = 4;
    //            delay(1000);
    //          }
    //          break;
    //        case 4:
    //          if (moveRandL(tiltRightMax, neutralPos,  false)) {
    //            stateTest = 0;
    //            delay(1000);
    //          }
    //          break;
    //      }
    //    }



    /*
       nod - have the neck nod a forward and back at a certain speed for a given number of times
          int numNods - the number of times the neck should be nodding
          int timeBtweenNod - how long between the fwd and back
    */
    //    void nod(int numNods, int timeBtwnNod) {
    //      boolean isDone;
    //      for (int i = 0; i < numNods * 2; i++) {
    //        moveRotServo(rotCenterPos);
    //        switch (stateTest) {
    //          case 0:
    //            Serial.println("Nodding Forward");
    //            isDone = moveRandL(tiltLeftMax, tiltRightMax, true);
    //            if (isDone) {
    //              isDone = false;
    //              stateTest = 1;
    //              delay(timeBtwnNod);
    //            }
    //
    //            break;
    //          case 1:
    //            Serial.println("Nodding Backward");
    //
    //            isDone = moveRandL(tiltRightMax, tiltLeftMax, true);
    //            if (isDone) {
    //              isDone = false;
    //              stateTest = 0;
    //              delay(timeBtwnNod);
    //            }
    //            break;
    //        }
    //      }
    //      Serial.print("FINISHED NODDING ");  Serial.print(numNods);  Serial.println(" Times");
    //      delay(5000); /// remove eventually
    //    }



    /*
       tilt - have the neck tilte a left and right at a certain speed for a given number of times
          int numTilts - the number of times the neck should be tilting
          int timeBtwnTilt - how long between the left and right
    */
    //    void tilt(int numTilts, int timeBtwnTilt) {
    //      boolean isDone;
    //      for (int i = 0; i < numTilts * 2; i++) {
    //        moveRotServo(rotCenterPos);
    //        switch (stateTest) {
    //          case 0:
    //            isDone = moveRandL(nodBackMax, nodFwdMax, false);
    //            if (isDone) {
    //              isDone = false;
    //              stateTest = 1;
    //              delay(timeBtwnTilt);
    //            }
    //            break;
    //          case 1:
    //            isDone = moveRandL(nodFwdMax, nodBackMax, false);
    //            if (isDone) {
    //              isDone = false;
    //              stateTest = 0;
    //              delay(timeBtwnTilt);
    //            }
    //            break;
    //        }
    //      }
    //      Serial.print("FINISHED Tilting ");  Serial.print(numTilts);  Serial.println(" Times");
    //      delay(5000); /// remove eventually - just there for testing purposes
    //    }




    //******************************************************************************CALIBRATION****************************************************************
    /*
       step servo to the maximum position in a degree incrementation
    */
    void stepToMax() {
      for (int currPos = minDeg; currPos < maxDeg; currPos += 2) {
        moveRightServo(currPos);
        moveLeftServo(currPos);  // drive the servo
//        moveRotServo(currPos);

        Serial.print("Rotating to "); Serial.println(currPos);
        delay(neckMotorCmdPause);
        uint16_t val = analogRead(neckPotPinR);
        Serial.print("Pos = ");  Serial.print(val); Serial.println("Deg");
      }

    }

    /*
       step servo to the minimum position in a degree incrementation
    */
    void stepToMin() {
      for (int currPos = maxDeg; currPos >= minDeg; currPos -= 2) {
        moveRightServo(currPos);              // tell servo to go to position in variable 'pos'
        moveLeftServo(currPos);  // drive the servo
//        moveRotServo(currPos);

        Serial.print("Rotating to "); Serial.println(currPos);
        delay(neckMotorCmdPause);
        uint16_t val = analogRead(neckPotPinR);
        Serial.print("Pos = "); Serial.print(val); Serial.println("Deg");
      }
    }


    /*
       calibration of the sensor
          integer minPos: minimum deg position to move to
          integer maxPos: max deg position to move to
          integer analogPin: servo pin to read
    */
    void calibration(int minPos, int maxPos, int analogPin)
    {
      int i;
      for (i = 0; i < 3; i++) {
        // Move to the minimum position and record the feedback value
        stepToMin();
        delay(2000); // make sure it has time to get there and settle
        minFeedback = analogRead(analogPin);
        Serial.println("Cal Min");
        Serial.print("Min Reading = "); Serial.println(minFeedback);
        delay(1000);

        // Move to the maximum position and record the feedback value
        stepToMax();
        delay(2000); // make sure it has time to get there and settle
        maxFeedback = analogRead(analogPin);
        Serial.println("Cal Max");
        Serial.print("Max Reading = "); Serial.println(maxFeedback);
        delay(1000);
        minFBs[i] = minFeedback;
        maxFBs[i] = maxFeedback;

      }
      // calculate and print all of the min and max values, and the average of each respectively
      Serial.print("Min = ");    Serial.print(minFBs[0]); Serial.print(", "); Serial.print(minFBs[1]); Serial.print(", "); Serial.println(minFBs[2]);
      double aveMin = (minFBs[0] + minFBs[1] + minFBs[2]) / 3;
      Serial.print("aveerage MIN ="); Serial.println(aveMin);
      Serial.print("Max = ");    Serial.print(maxFBs[0]); Serial.print(", "); Serial.print(maxFBs[1]); Serial.print(", "); Serial.println(maxFBs[2]);
      double aveMax = (maxFBs[0] + maxFBs[1] + maxFBs[2]) / 3;
      Serial.print("aveerage MAX ="); Serial.println(aveMax);
      delay(5000);

      //calculate and print all of the converted min and max values
      i = 0;
      Serial.print("Min converted = ");
      for (i = 0; i < 3; i++) {
        double deg = feedbackToDeg(minFBs[i]);
        Serial.print(deg); Serial.print(", ");
      }
      Serial.println("  done");
      Serial.print("Max converted = ");
      int j;
      for (j = 0; j < 3; j++) {
        double deg = feedbackToDeg(maxFBs[j]);
        Serial.print(deg); Serial.print(", ");
      }
      Serial.println("  done 2");
      delay(10000);
    }


};
