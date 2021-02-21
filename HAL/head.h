/**********************************************************************************
  head.h

  This file contains the class definition, attributes, and methods of the Head.
  The Head consists of all the mechanical systems as well as all the sensors and
  electronics (excluding the LCD user interface). The functions incorporate all
  the different features and have them interact with one another. The different
  tests for various LOC levels, different pulses, and the functions that are defined
  to interact with the UI are defined here. Several functions can be expanded on to
  improve and complete the interaction between HAL and the user.

  Last Edited by Ethan Lauer on 4/3/20
 *********************************************************************************/
#include <Arduino.h>

// Mechanical System Header Files
#include "Eyes.h"
#include "Brows.h"
#include "Lids.h"
#include "jaw.h"
#include "neck.h"

// Sensors and Electronics Header Files
#include "forceSensor.h"
#include "microphone.h"
#include "camera.h"
#include "pulse.h"
#include "voice.h"

class Head {
  private:
    // ********************************************PRIVATE**************************************
    // Overall LOC State Variable
    // 0 = Alert, 1 = Verbal Response, 2 = Painful Response, 3 = Unresponsive
    // initialize to arbitrary 7 when running full code - will be easier to see that it is an error
    int stateLOC = 0; // This value is changed when the user provides input

    // State Variables for the switch cases for the different LOC tests based on the stateLOC value
    int alertLOCState = 0; // state machine for alert actions for LOC test
    int verbalLOCState = 0; // state machine for verbal actions for LOC test
    int painLOCState = 0; // state machine for pain actions for LOC test
    int uLOCState = 0; // state machine for unresponsive actions for LOC test - note: not used in this version of the code.

    // Circulation Variable
    int pulseNum = 120; //used for exact pulse BPM - initialize to default 120 BPM

    // Listening Variables
    int micState = 0; // state variale if listening or responding or checking till done talking
    const int micRange = 20; //any value outside the range of -micRange to micRange is means the person is talking
    const int micOffset = 560;  // offest used to filter out the unneccessary data from the mic - Note: Mic Potentiometer may need to be adjusted before each use
    int timesResponded = 0; // the number of times the head responded to the person - keeps track of script and when to execute certain actions.
    boolean finishedDialogue = false; // track if the person is done talking

    // Camera Hand Tracking Variables
    int lastTime; // store the number of frames hand was tracked
    int count; // count how long the hand has been at the same location

    //Airway Variables for UI interaction
    boolean isNormAir; //true if normal airway
    boolean isLockJaw; //true if have lock jaw problem
    boolean isIrrBreath; //true if have irregular breathing
    boolean isStriBreath; // true if have stridor breathing
    boolean isAgGasp; // true if have agonal gasps


    // Sensor pins
    const int foreheadSensorPin = A14;
    const int neckSensorPin = A15;
    const int chinSensorPin = A16;
    const int jawRightSensorPin = A17;
    const int jawLeftSensorPin = A18;
    const int micPin = A20;

    // General state variable for testing functions
    int headState = 0;

  public:
    // ********************************************PUBLIC**************************************
    // Having all of the features public will let you access certain facial
    // feautres on their own and their own functions without having to rewrite them as a "getter" function.
    // Tt might be possible to have these in private after final testing is complete.

    // Variables to check if all the settigns are set
    // must be public unless want to use getter for lcd beginning
    // Have the settings been given a value?
    boolean isLOCSet = false;
    boolean isAirSet = false;
    boolean isPulseSet = false;

    // NOTE: Not used in this code. Was unable to get to this part of the testing.
    boolean isLOCTestDone = false; // true if finished LOC Test
    boolean isAirTestDone = false; // true if finished air Test
    boolean isPulseTestDone = false; // true if finished air Test

    //MECHANICAL SYSTEMS
    Eyeballs eyeBalls = Eyeballs();
    Eyebrows eyeBrows = Eyebrows();
    Eyelids eyeLids = Eyelids();
    Neck neck = Neck();
    Jaw jaw = Jaw();

    // SENSORS
    ForceSensor foreheadSensor = ForceSensor(foreheadSensorPin); // Large FSR
    ForceSensor neckSensor = ForceSensor(neckSensorPin); // Large FSR
    ForceSensor chinSensor = ForceSensor(chinSensorPin); // Small FSR
    ForceSensor jawRightSensor = ForceSensor(jawRightSensorPin); // Small FSR
    ForceSensor jawLeftSensor = ForceSensor(jawLeftSensorPin); // Small FSR
    Microphone mic = Microphone(micPin);
    Camera camera = Camera();

    // OTHER ELECTRONICS
    Pulse pulse = Pulse();
    Voice voice = Voice();

    //**************************************************************************************************INITIALIZE*********************************************************
    // Define Head
    Head() {
    }

    /*
       setUp - Run the setup function for each of the mechanical systems and all the electronics.
               Run this in the void setup() function in the main .ino file.
    */
    void setUp() {
      // Mechanical Systems
      eyeLids.setUp();
      eyeBalls.setUp();
      eyeBrows.setUp();
      neck.setUp();
      jaw.setUp();

      // Sensors and Electronics
      foreheadSensor.setUp(foreheadSensorPin);
      neckSensor.setUp(neckSensorPin);
      chinSensor.setUp(chinSensorPin);
      jawRightSensor.setUp(jawRightSensorPin);
      jawLeftSensor.setUp(jawLeftSensorPin);
//      camera.setUp(); // uncomment once you are testing with camera
      mic.setUp(micPin);
      pulse.setUp();
//      voice.setUp(); // uncomment once you are testing with voice
    }


    //*********************************************************************SET PARAMETERS/FEATURES  - FOR UI INTERACTION*******************************************************************

    /*
      setLOC - Set the LOC state variable to the desired input
               Set the respective boolean indicating the LOC is now set.
          int LOCSetting - integer indicating the level  where
                           0 = Alert, 1 = Verbal Response, 2 = Painful Response, 3 = Unresponsive
    */
    void setLOC(int LOCSetting) {
      stateLOC = LOCSetting;
      isLOCSet = true;
    }


    /*
       setAir - Set the airway conditions to conditions that were passed into the function
                Set the respective boolean indicating the airway is now set
           boolean *airCondition - array of booleans where each index represents the following
                                   airCondition[0] - true if normal airway, false if there is something wrong
                                   airCondition[1] - true if lock jaw problem, false otherwise
                                   airCondition[2] - true if have irregular breathing, false otherwise
                                   airCondition[3] - true if have stridor breathing, false otherwise
                                   airCondition[4] - true if have agonal gasps, false otherwise
    */
    void setAir(boolean *airCondition) {
      isNormAir = airCondition[0];
      if (!isNormAir) {
        isLockJaw = airCondition[1];
        isIrrBreath = airCondition[2];
        isStriBreath = airCondition[3];
        if (!isIrrBreath && !isStriBreath) {  // agonal gasps cannot occur at the same time as irregular breathing or stridor breathing
          isAgGasp = airCondition[4];
        }
      }
      isAirSet = true;  // indicate airway is set now
      Serial.print("Normal airway condition"); Serial.println(isNormAir); // for debuggin
    }

    /*
       setPulseBPM - Set the Pulse BPM to the value that is inputted.
                     Set the respective boolean indicating the pulse/circulation is now set
              int pulseBPM - integer of the exact BPM the pulse is set to
    */
    void setPulseBPM(int pulseBPM) {
      pulseNum = pulseBPM;
      Serial.print("SET PULSE TO = "); Serial.println(pulseNum);
      isPulseSet = true;
    }

    /*
      setPulseRange - Set the Pulse range to the desired input
                      Set the respective boolean indicating the pulse/circulation is now set
          int pulseRange - integer from 0-2 where:
                          0 = normal pulse range
                          1 = fast pulse range
                          2 = slow pulse range
    */
    void setPulseRange(int pulseRange) {
      if (pulseRange == 0) {        // obtain a pulse value withing the normal, fast, or slow ranges (see pulse.h for details)
        pulseNum = pulse.normalPulse();
      } else if (pulseRange == 1) {
        pulseNum = pulse.fastPulse();
      } else if (pulseRange == 2) {
        pulseNum = pulse.slowPulse();
      } else {
        Serial.println("ERROR SETTING PULSE FROM RANGE");
      }
      isPulseSet = true;  //indicate the pulse is now set
    }

    /*
       isAllSet - checks to see if all of the settings for the head are set
            return boolean - true if all the different settings have been assigned a value, false otherwise
    */
    boolean isAllSet() {
      return (isLOCSet && isAirSet && isPulseSet);
    }


    //************************************************************************************** LOC TESTS **********************************************************************
    /*
         LOCTEST - Switch case that calls the different LOC tests depending on the state
                   that was set from the user. See each LOCTest() for details.
    */
    void LOCTEST() {
      switch (stateLOC) {
        case 0: // alert
          alertLOCTest();
          break;

        case 1: // verbal
          verbalLOCTest();
          break;

        case 2: // pain
          painLOCTest();
          break;

        case 3: // unresponsive
          unresponsive();
          break;
      }
    }


    /*
       alertLOCTest - This is the fully alert LOC Test called by the LOCTEST(). The pulse that was inputted
                      by the user runs continuously during this test. This test assumes the user is folowing a standard
                      script the entire time. The responses should be fairly quick and realsitic.
                      State 0 = First, HAL listens to the user and responds accordingly (see listenAndRespond() for details),
                      State 1 = Once he is done responding, he then tracks the person's glove until it is out of the frame for
                                more than 40 iterations
                      State 2 = Then Hal moves to an general idling state.
                      NOTE: State 3 is included and can be switched to for future work. Any futher interaction with Hal can occur here.
                            The test can be concluded by adding and exit case at the end once the test is complete.
                            If statements are used in this function due to an odd bug in the switch case that only occured occasionally.
                            If this bug can be fixed, a switch case would be better than if statements.
    */
    void alertLOCTest() {
      //      Serial.print("ALERT LOC STATE = "); Serial.println(alertLOCState); // for debugging
      pulse.pulseByBPM(pulseNum);     // run the pulse constantly
      //***************** State is equal to 0****************
      if (alertLOCState == 0) {
        if (finishedDialogue) { // if the person is done speaking back and forth, switch states
          alertLOCState = 1;
        }
        eyeLids.blinkEyes();  // can be changed to an idleBlink for more realism
        listenAndRespond();   // use mic to listen to user and respond appropriately
      }
      //***************** State is equal to 1****************
      if (alertLOCState == 1) {
        int *blockAge = camera.getBlockInfo(4); // use the camera to detect objects
        int framesTracked = blockAge[0];    // how long has the object been tracked
        Serial.print("AGE OF BLOCK = "); Serial.println(framesTracked);
        followHand();     // move the eyeballs to follow the detected hand
        if (framesTracked > 0 && framesTracked == lastTime) {  // if the object is no longer detected or being tracked
          if (count > 40) {     // if it doesnt comeback into the frame within the given time, switch states
            alertLOCState = 2;
            Serial.println ("Set to second state");
            delay(2000);
          } else {  //increment the count
            count++;
            Serial.print(" count = "); Serial.println(count);
          }
        } else {
          lastTime = framesTracked;   // update the variable that stores the previous value of how long it is tracked
        }
      }
      //***************** State is equal to 2****************
      if (alertLOCState == 2) {     // complete generic movements as a place holder
        //        Serial.println("IN THE SECOND CASE");
        eyeBalls.neutralBoth();
        eyeLids.blinkEyes();
        //        alertLOCState = 3;  // commented out for now for testing
      }
      //****************** State is equal to 3****************
      if (alertLOCState == 3) { // final test case
        Serial.println("IN THE THIRD CASE");
        delay(2000);
        alertLOCState = 2;
      }
    }


    /*
       verbalLOCTest - This is the verbal LOC Test called by the LOCTEST(). The pulse that was inputted
                       by the user runs continuously during this test. This test assumes the user is folowing a standard
                       script the entire time. The responses should be more dazed and slow. The system should
                       respond to verbal stimuli but slower than alert levels.
                       State 0 = First, HAL listens to the user and responds accordingly (see listenAndRespond() for details),
                       State 1 = Once he is done responding, he then tracks the person's glove until it is out of the frame for
                                 more than 40 iterations. The neck should also move to follow the hand as well.
                       State 2 = Then Hal moves to an general idling state.
                       NOTE: The test can be concluded by adding and exit case at the end once the test is complete.
                             If statements are used in this function due to an odd bug in the switch case that only occured occasionally.
                             If this bug can be fixed, a switch case would be better than if statements.
    */
    void verbalLOCTest() {
      //      Serial.print("VERBAL LOC STATE = "); Serial.println(verbalLOCState); // for debugging
      pulse.pulseByBPM(pulseNum); // pulse runs continuously
      //***************** State is equal to 0****************
      if (verbalLOCState == 0) { // first speak with the person. after 2 responses then change state
        if (finishedDialogue) {// if the person is done speaking back and forth, switch states
          verbalLOCState = 1;
        }
        eyeBalls.stepEyesUpandDownVerySlow(); // can be replaced with a more condensed function of the "dazed" movements
        listenAndRespond(); // use mic to listen to user and respond appropriately
      }
      //***************** State is equal to 1****************
      if (verbalLOCState == 1) { // next follow the persons finger with eyes.
        int *blockAge = camera.getBlockInfo(4);// use the camera to detect objects
        int framesTracked = blockAge[0];   // how long has the object been tracked
        Serial.print("AGE OF BLOCK = "); Serial.println(framesTracked);
        followHand(); // move the eyeballs to follow the detected hand
        if (framesTracked > 0 && framesTracked == lastTime) { // if the object is no longer detected or being tracked
          if (count > 40) {// if it doesnt comeback into the frame within the given time, switch states
            verbalLOCState = 2;
            Serial.println("Set to second state");
            delay(1000);
          } else {
            Serial.print("count = "); Serial.println(count);
            count++;  // increment the count
          }
        } else {
          lastTime = framesTracked; // update the variable that stores the previous value of how long it is tracked
        }
      }
      //***************** State is equal to 2****************
      if (verbalLOCState == 2) {   // complete generic movements as a place holder, then complete the test
        Serial.println("IN SECOND CASE OF VERBAL LOC TEST");
        //        eyeBalls.neutralBoth();
        eyeLids.blinkEyes();
        eyeBalls.stepEyesUpandDownSlow();
      }
    }

    /*
        painLOCTest - This is the pail LOC Test called by the LOCTEST(). A default random pulse
                      runs continuously during this test but can be changed to a poor pulse in future iterations.
                      This test assumes the user is folowing a standard script the entire time. The system
                      should only respond to physical stimuli and does not listen to the user at all. The movements
                      should be more dazed. This function soley relies on the neck sensor to change reactions
                      but the other 4 FSRs can be incorporated into this test in future iterations.
                      The switch cases changes states based on the level of force applied to the neck sesnor.
                      Uncomment/comment different lines to have different reactions based on the degree of force applied.
    */
    void painLOCTest() {
      pulse.randPulse();    // random/irregular pulse
      headState = neckSensor.forceRange(neckSensorPin); // read force sensor and divide into a specific range
      switch (headState) {
        case 0:     // act dazed or tired while no force is applied
          //          alertActions();
          eyeBrows.raiseAndSlightRaise();
          eyeLids.stepLidsSleepy();
          //          eyeLids.stepLidsVerySlow();

          eyeBalls.stepEyesUpandDownVerySlow();
          neck.neutral();
          break;
        case 1:
          //          eyeBalls.glanceRight();
          break;

        case 2: // when a light force is applied, move to set points (also helps when from large force back to no force, mechanical systems have a midpoint to move back to)
          eyeBrows.slightRaiseBoth();
          eyeBalls.neutralBoth();
          eyeLids.openPercentBoth(50);
          break;

        case 3:
          //          eyeBalls.lookLeftandRight();
          break;

        case 4:     // a big force is applied, complete the preset pain actions (can be replaced with specific actions if using other force sensors in the future)
          //          eyeBalls.stepEyesDown();
          painActions();
          break;
      }
    }



    //************************************************************************************** LOC FUNCTIONS **********************************************************************
    /*
       listenAndRespond - The system listens to the user and responds twice with mechanical and audio feedback. This is a modified verstion of micTest().
                          State 0 = The microphone is read, filtered, and if it is outside of the normal range,
                          State 1 = the person is talking so the microphone waits to see if the audio goes back to normal at which point
                          State 2 = The system responds with an audio response and a mechanical responds that lasts until the audio is complete.
                          This process is repeated twice (ie. user speaks, Hal responds, user speaks, Hal Responds).
                          The timesResponded limit can be changed based on the script that is followed. The 3.5 second wait after the person has finished
                          talking can be adjusted to a different time if it proves to be too slow.
    */
    void listenAndRespond() {
      int anVal;  // store raw analog value
      int trimmedVal; // store filtered value
      switch (micState) {
        case 0:       // The head is listening
          // if the head has responded two times already, then stop and say that the dialogue is finished
          if (timesResponded == 2) {
            timesResponded = 0;
            finishedDialogue = true;
//            return finishedDialogue;  // this line can be removed since finishedDialogue is global. Just have not tested at this point
            break;
          }
          // check the microphone to see if its has heard anything out of the ordinary range
          anVal = analogRead(micPin);
          trimmedVal = anVal - micOffset;
          Serial.print(" Analog Value = "); Serial.println(trimmedVal, DEC);
          // if the mic heard anything out of the ordinary range, then switch to next case
          if (trimmedVal > micRange || trimmedVal < -micRange) {
            micState = 1;
            timeNow = millis(); // reset timer for next case
          }
          break;

        case 1:       // The head is waiting for user to finish speaking
          anVal = analogRead(micPin);
          trimmedVal = anVal - micOffset;
          Serial.println("You are talking ");
          Serial.print(" Analog Value = "); Serial.println(trimmedVal, DEC);
          // if the mic has picked up values back in the normal range and has waits for 3.5 seconds of "no talking", then move to the next state
          if ((trimmedVal < micRange || trimmedVal > -micRange) && (millis() > timeNow + 3500)) {
            micState = 2;
            timeNow = millis(); // reset timer for next case
          }
          break;

        case 2:   // Head is responding to dialogue
          stateVoiceLOCResponses(); // completes certain responses depending on LOC state
          anVal = analogRead(micPin);
          trimmedVal = anVal - micOffset; // NOTE: future work could include having HAL continue to listen as he responds and stop/change his reaction if the user interrupts him
          Serial.println("responding ");
          Serial.print(" Analog Value = "); Serial.println(trimmedVal, DEC);
          while (voice.stillPlaying()) {  // if the audio is still playing
            stateMechLOCResponses(); // completes the corresponding responses that match the audio
          }
          timesResponded++;          // increment the times responded and go back to listening
          micState = 0;
          break;
      }
    }


    /*
          followHand - This function obtains camera info and the pixel coordinates, converts these values to servo values for both the
                       left and right eyeballs, and drives servos to these positions. If the LOC is a verbal response, the neck should
                       rotate to follow the pixel moving in the X(horizontal) direction.
                       NOTE: in future work, the head can be tilted so it can follow the Y(vertical) position.
    */

    void followHand() {
      eyeLids.openBoth(); // keep eyelids open the entire time - can be replaced with an idle blink or different blink functions based on LOC
      int *pixCoords;
      pixCoords = camera.getBlockInfo(0); // get the x and y coordinates of the detected block
      //      Serial.print("Pix X = "); Serial.println(pixCoords[0]);
      //      Serial.print("Pix Y = "); Serial.println(pixCoords[1]);
      int xPix = pixCoords[0];
      int yPix = pixCoords[1];

      // Get the pixel coordinates for the edges of the camera frame
      int pixMaxLeft = camera.pixMaxLeft;
      int pixMaxRight = camera.pixMaxRight;
      int pixMaxUp = camera.pixMaxUp;
      int pixMaxDown = camera.pixMaxDown;
      //      Serial.print("LEft Eye Up Max = "); Serial.println(leftUpVertEye);
      //      Serial.print("Right Eye Up Max = "); Serial.println(rightUpVertEye);

      //
      // Convert pixels to eye servo positions - note: currently using global variables for eye positions. Change this to a public attribute in the eyes (not tested yet)
      //                                               I have tried to use a separate function outside of this to condense the "map" function but the data transfer between
      //                                               functions failed. Used volatiles, etc. Possibly try it again or another way if at all possible but this works with minimal delay.
      int horLeftEyeCoord = map(xPix, pixMaxLeft, pixMaxRight, leftLeftHorEye, leftRightHorEye);
      int horRightEyeCoord = map(xPix, pixMaxLeft, pixMaxRight, rightLeftHorEye, rightRightHorEye);
      int vertLeftEyeCoord = map(yPix, pixMaxUp, pixMaxDown, leftUpVertEye, leftDownVertEye);
      int vertRightEyeCoord = map(yPix, pixMaxUp, pixMaxDown, rightUpVertEye, rightDownVertEye);

      //      Serial.print(pixMaxUp); Serial.print(","); Serial.print(pixMaxDown); Serial.print(","); Serial.print(leftUpVertEye); Serial.print(","); Serial.println(leftDownVertEye);
      // printing for debugging
      //      Serial.println("in head");
      //      Serial.print("Head Left X = "); Serial.println(horLeftEyeCoord);
      //      Serial.print("Head Left Y = "); Serial.println(vertLeftEyeCoord);
      //      Serial.print("Head Right X = "); Serial.println(horRightEyeCoord);
      //      Serial.print("Head  Right Y = "); Serial.println(vertRightEyeCoord);

      //Move eyes or neck to appropriate positions
      if (stateLOC == 0) {
        eyeBalls.moveHorEyesTo(horLeftEyeCoord, horRightEyeCoord);
        eyeBalls.moveVertEyesTo(vertLeftEyeCoord, vertRightEyeCoord);
      } else if (stateLOC == 1) {
        eyeBalls.moveHorEyesTo(horLeftEyeCoord + 5, horRightEyeCoord + 5);  // add an arbitrary offset (5) of the eyeball positions since the LOC is lower than alert
        eyeBalls.moveVertEyesTo(vertLeftEyeCoord + 5, vertRightEyeCoord + 5);
        int neckRotCoord = map(xPix, pixMaxLeft, pixMaxRight, neck.rotLeftMax, neck.rotRightMin); // convert pixels to neck rotation position
        neck.rotToDeg(neckRotCoord, 3); // rotate the neck to follow the hand
      }
    }

    //*********************************************RESPONSES for the  LISTEN AND RESPOND FUNCTION*********************************************

    /*
          stateMechLOCResponses - This funtion uses a switch case that calls the various mechanical responses
                                  in the LOC test (see listenAndRespond()) based on the LOC set by the user.
                                  This function enables the listenAndRespond() function to be cleaner.
    */
    void stateMechLOCResponses() {
      switch (stateLOC) {
        case 0:
          alertResponses();
          break;
        case 1:
          verbalResponses();
          break;
        case 2:
          painResponses();
          break;
        case 3:
          unresponsive();
          break;
      }
    }

    /*
       stateVoiceLOCResponses -  This funtion uses a switch case that calls the various audio responses
                                 used in the LOC test (see listenAndRespond()) based on the LOC set by the user.
                                 This function enables the listenAndRespond() function to be cleaner.
    */
    void stateVoiceLOCResponses() {
      switch (stateLOC) {
        case 0:
          alertVoiceRespond();
          break;
        case 1:
          verbalVoiceRespond();
          break;
        case 2:
          painVoiceRespond();
          break;
        case 3:
          unresponsive();
          break;
      }
    }

    /*
       alertVoiceRespond - Based on the times the system has responded to the user, (the number of back
                           and forths of the conversation) the Voice plays different .wav files that correspond with the alert LOC. To avoid
                           using a loop to play the file, an if statement is called with each iteration to
                           check if the voice is playing the audio file (see listenAndRespond() for more
                           details of implementation).
                           NOTE: These files should be replaced with new recordings so they are more accurate
                                 and professional. More cases can be added if the system is set up to respond more than twice.
    */
    void alertVoiceRespond() {
      switch (timesResponded) {
        case 0:
          if (voice.stillPlaying()) {     //first dialogue: respond with a greeting and a brief description of medical problem
          } else {
            voice.playFile("HIBUS.WAV");
          }
          break;
        case 1:
          if (voice.stillPlaying()) { //second dialogue: agree to follow the hand
          } else {
            voice.playFile("YESFOLOW.WAV");
          }
          break;
      }
    }


    /*
       alertResponses - Based on the times the system has responded to the user, (the number of back
                        and forths of the conversation) the mechanical componenets of the head move
                        in sync with the audio file that was called (see cooresponding case in alertVoiceRespond()).
                        These movements shoud be similar to average human movement that is a reasonable, natural speed.
                        NOTE: The mechanical movements should be changed to be in sync with the audio file.
                              Currently, the mechanical movements are just place holders. Once the audio
                              file is defined, the respective mechancial components (particularly the jaw)
                              should be programmed to move in sync. Functions for each response should be
                              created in each respective class. More cases can be added if the system is
                              set up to respond more than twice.
    */
    void alertResponses() {
      switch (timesResponded) {
        case 0:// first mechanical response - saying hello
          eyeLids.winkLeft();
          jaw.helloJaw();
          break;

        case 1:// second mechanical response - agreeing to follow the hand.
          eyeLids.winkRight();
          jaw.helloJawSlow();
          neck.nodTimes(3, 250);
          break;
      }
    }

    /*
       verbalVoiceRespond - Based on the times the system has responded to the user, (the number of back
                           and forths of the conversation) the Voice plays different .wav files that correspond with the verbal LOC state. To avoid
                           using a loop to play the file, an if statement is called with each iteration to
                           check if the voice is playing the audio file (see listenAndRespond() for more
                           details of implementation).
                           NOTE: These files should be replaced with new recordings so they are more accurate
                                 and professional. The current files are place holders and DO NOT represent the
                                 LOC state right now. More cases can be added if the system is set up to respond more than twice.
    */
    void verbalVoiceRespond() {
      switch (timesResponded) {
        case 0:
          if (voice.stillPlaying()) {  //first dialogue: respond with a dazed greeting and a poor description of medical problem
          } else {
            voice.playFile("AHHH.WAV");
          }
          break;
        case 1:
          if (voice.stillPlaying()) { //second dialogue: agree to follow the hand but delayed
          } else {
            voice.playFile("YESFOLOW.WAV");
          }
          break;
      }
    }



    /*
       verbalResponses - Based on the times the system has responded to the user, (the number of back
                         and forths of the conversation) the mechanical componenets of the head move
                         in sync with the audio file that was called (see cooresponding case in verbalVoiceRespond()).
                         These movements shoud slower than average human movement or could be faster to
                         illustrate the person is nervous.
                         NOTE: The mechanical movements should be changed to be in sync with the audio file.
                              Currently, the mechanical movements are just place holders. Once the audio
                              file is defined, the respective mechancial components (particularly the jaw)
                              should be programmed to move in sync. Functions for each response should be
                              created in each respective class. More cases can be added if the system is
                              set up to respond more than twice.
    */
    void verbalResponses() {
      switch (timesResponded) {
        case 0:
          eyeBrows.raiseLeftFurrowRight();  //first dialogue- currently placeholder (replace)
          break;

        case 1:
          eyeBalls.lookLeftandRight();//second dialogue- currently placeholder (replace)
          // add neck rotation here
          neck.tilt();
          break;
      }
    }


    /*
      painVoiceRespond - This function should hold any pain audio responses the system can provide if the patient is able to.
                         In the pain LOC, the system should not respond to any audio stimuli but should instead only respond
                         to physical response.
                         NOTE: These files should be replaced with new recordings so they are more accurate
                               and professional. The current files are place holders (or defaults)and DO NOT represent the
                               LOC state right now. More cases can be added if the system is set up to respond more than twice.
                               The switch case can be adjusted to change based on a variable that keeps track of the physical
                               responses rather than the audio responses.
    */
    void painVoiceRespond() {
      switch (timesResponded) {
        case 0:
          if (voice.stillPlaying()) {
          } else {
            voice.playFile("SDTEST1.WAV");  // default sample .wav file from Teensy website
          }
          break;
        case 1:
          if (voice.stillPlaying()) {
          } else {
            voice.playFile("AHHH.WAV"); // placeholder pain yell
          }
          break;
      }
    }



    /*
       painResponses - This function should hold any pain mechanical responses the system can provide if the patient is able to.
                       In the pain LOC, the system should not respond to any audio stimuli but should instead only respond
                       to physical response.
                       NOTE: These files should be replaced with mechanical movements so they are more accurate
                             and professional. The current movements place holders (or defaults)and DO NOT represent the
                             LOC state right now. More cases can be added if the system is set up to respond more than twice.
                             The switch case can be adjusted to change based on a variable that keeps track of the physical
                             responses rather than the audio responses.
    */
    void painResponses() {
      switch (timesResponded) {
        case 0:
          eyeBrows.raiseAndFurrow();
          break;

        case 1:
          eyeLids.longClose();
          neck.wince(true); // wincing to the left or right depending on the FSR pressd
          break;
      }
    }



    //*********************************************GENERAL LOC ACTIONS*********************************************
    //******* These functions are currently used to have a standard LOC actions for testing. New functions can be
    //******* created that combine all the different response functions for each mechanical components. For example,
    //******* a function called "alertGreetAction()" could include a jaw.alertGreet(), eyeLids.idleBlink(), and other
    //******* actions that can be synchronized with the audio file called in case 0 of alertVoiceRespond(). 
    //******* Note: painActions() is currently used in the painLOC with the voice included in the response. Audio files
    //*******       can also be included in these functions to make the code more condensed as well.

    /*
       stateMechLOCActions - This includes a switch case that calls the different standard actions based on the LOC state.
                             This is used to make the code cleaner and easer to read when implemented in other parts of the code.
                             NOTE: This can be changed or new functions could be added so a variety of actions can be called
                                   for each LOC state.
    */
    void stateMechLOCActions() {
      switch (stateLOC) {
        case 0:
          alertActions();
          break;
        case 1:
          verbalActions();
          break;
        case 2:
          painActions();
          break;
        case 3:
          unresponsive();
          break;
      }
    }


    /*
        alertActions -  The head conducts generalized alert actions wehre the eyelids blink,
                        the eyes glance to the left every so often, and the eyebrows raise and furrow.
                        NOTE: The other mechanical systems were not set at the time this function was
                              created but the jaw and neck can be included in these actions.
    */
    void alertActions() {
      eyeLids.blinkEyes();
      eyeBalls.glanceLeft();
      eyeBrows.raiseAndFurrow();
    }


    /*
        verbalActions -  The head conducts generalized verbal actions wehre the eyelids blink normally,
                         the eyeballs slowly move uup and down, and the eyebrows raise and furrow.
                         NOTE: The other mechanical systems were not set at the time this function was
                               created but the jaw and neck can be included in these actions.
    */
    void verbalActions() {
      eyeBalls.stepEyesUpandDownVerySlow();
      eyeBrows.raiseAndFurrow();
      eyeLids.blinkEyes();
    }

    /*
      painActions -  The head conducts generalized pain actions in reaction high pressure applied to the left neck FSR where the voice 
                     yells "aahhhh", the eyeballs look left, the eyelids wince to the left, and the left eyebrow furrows.
    */
    void painActions() {
      if (voice.stillPlaying()) {
      } else {
        voice.playFile("AHHH.WAV");
      }
      eyeBrows.raiseRightFurrowLeft();
      eyeLids.winceLeft();
      eyeBalls.leftBoth();
      neck.wince(true);
    }


    /*
      unresponsive - The head completes very dazed movements constantly regardless of any stimuli.
    */
    void unresponsive() {
      eyeLids.lazyBlinkEyes();
      eyeBrows.slightRaiseBoth();
      eyeBalls.neutralBoth();
    }

    //************************************************************** TESTING FUNCTIONS **********************************************************************
    //****** These functions are used for testing different features and capabilities on their own and slowly integrating it with the rest of the code*******
    /*
       micTest - Tests the microphone and if the head can respond to an input that is louder than other parts.
                 State 0: The microphone waits to get input that is out of the normal range (listening),
                 State 1: waits until the value goes back into the normal range (waits until the person is done speaking),
                 State 2: then performs the arbitrary actions for arbitrary amount of time (responding).
    */
    void micTest() {
      int anVal; // stores raw analog input value
      int trimmedVal; // stores the value after the offset is subtracted
      switch (micState) {
        case 0:
          anVal = analogRead(micPin);   // get raw input
          trimmedVal = anVal - micOffset;   // subtract the excess data
          Serial.print(" Analog Value = "); Serial.println(trimmedVal, DEC);  // for debugging
          if (trimmedVal > micRange || trimmedVal < -micRange) {  // switch states if the mic picked up a value outside of the normal range
            micState = 1;
            timeNow = millis(); // reset timer for next state
          }
          break;
        case 1:
          anVal = analogRead(micPin);   // continue to read input
          trimmedVal = anVal - micOffset; // subtract excess data
          Serial.println("You are talking ");   // for debugging
          Serial.print(" Analog Value = "); Serial.println(trimmedVal, DEC);
          if ((trimmedVal < micRange || trimmedVal > -micRange) && (millis() > timeNow + 3500)) { // when the input goes back to the normal range and at least 3.5 seconds have passed within this range
            micState = 2;   // switch states
            timeNow = millis(); // reset timere
          }
          break;
        case 2:
          alertActions(); // complete generic reactions
          anVal = analogRead(micPin); // continue to read input and filter in case other actions need to be taken
          trimmedVal = anVal - micOffset; // NOTE: future work could include having HAL continue to listen as he responds and stop/change his reaction if the user interrupts him
          Serial.println("responding ");  // for debugging
          Serial.print(" Analog Value = "); Serial.println(trimmedVal, DEC);
          if (millis() > timeNow + 10000) {   //respond for 10 seconds until returning to listening.
            micState = 0;
          }
          break;
      }
    }

    /*
       fsrTest - This function tests to see if all 5 force sensors can actuate differnt parts
                 of the head at the same time.
    */

    void fsrTest() {
      if (neckSensor.forceRange(neckSensorPin) == 0) {
        eyeLids.blinkEyes();
      }
      if (foreheadSensor.forceRange(foreheadSensorPin) == 4) {
        eyeBalls.glanceLeft();
      }
      if (chinSensor.forceRange(chinSensorPin) == 4) {
        eyeBrows.raiseAndFurrow();
      }
      if (jawRightSensor.forceRange(jawRightSensorPin) == 4) {
        jaw.regOpenAndClose();
      }else{
        jaw.neutralMouth();
      }
      if (jawLeftSensor.forceRange(jawLeftSensorPin) == 4) {
         jaw.thrustJawPercent(100);

      }else{
        jaw.neutralMouth();
      }
    }




    /*
      neckTest - General function for testing different neck movements and setpoints.
                 The commented code can be commented/uncommented in order to test different parts.
                 See neck.h for details.
    */
    void neckTest() {
      //      neck.tiltTimes(3, 250);
      //      neck.nodTimes(3, 140);
      //      neck.neutral();
      //      neck.moveToDeg(120,2);
      //      neck.calibration(0,270,A13);
      //      neck.nod();
      //      neck.tilt();
      switch (headState) {    // iterate through different setpoints
        case 0:
          if ( neck.rotToDeg(35, 2)) {  // rotate to the center point
            Serial.println("COMMANDED Rotating to 35");
            headState = 1;
            delay(2000);
          }
          break;
        case 1:
          if ( neck.rotToDeg(80, 2)) {  // rotate to the left
            Serial.println("COMMANDED Rotating to 80");
            headState = 2;
            delay(2000);

          }
          break;
        case 2:
          if ( neck.rotToDeg(0, 2)) { // rotate to the right
            Serial.println("COMMANDED Rotating to 0");
            headState = 3;
            delay(2000);
          }
          break;

        case 3:
          if ( neck.rotToDeg(35, 2)) {  // rotate to the center
            headState = 4;
            delay(1000);
          }
          break;
        case 4:
          if ( neck.wince(true)) {
            headState = 5;
            //            delay(1000);
          }
          break;
        case 5:
          if ( neck.neutral()) {
            headState = 6;
            delay(1000);
          }
          break;

        case 6:
          if (neck.wince(false)) {
            headState = 7;
            //            delay(1000);
          }
          break;
        case 7:
          if (neck.neutral()) {
            headState = 0;
            delay(1000);
          }
          break;
      }

    }


    /*
       pulseTest - General function used to test the regular and irregular pulse
                   to ensure the BPM calaculation is accurate and the randomizer is indeed random.
                   See pulse.h for details.
    */
    void pulseTest() {
      pulse.pulseByBPM(60);
      //      pulse.randPulse();
    }


    /*
       voiceTest - General function to test the capabilities of the speaker. See voice.h for details
    */
    void voiceTest() {
      voice.playFile("HELLO.WAV");
      Serial.print("Bool   === "); Serial.println(voice.stillPlaying());
      while (voice.stillPlaying()) {
        Serial.println("Still Playing file");
        delay(500);
      }
    }
    /*
       jawTest - General function to test the capabilities of the jaw movements. See jaw.h for details.
    */
    void jawTest() {
//      jaw.stepVertOpenClose(3, 3);
      //      jaw.regOpenAndClose();
            jaw.openJawPercent(100);
            Serial.println("open jaw all the way");
            delay(1000);
            jaw.openJawPercent(0);
            Serial.println("close jaw all the way");
            delay(1000);
            jaw.openJawPercent(50);
            Serial.println("open jaw Halfway");
            delay(1000);
      
            jaw.thrustJawPercent(100);
            Serial.println("thrust jaw all the way");
            delay(1000);
            jaw.thrustJawPercent(0);
            Serial.println("retract jaw all the way");
            delay(1000);
            jaw.thrustJawPercent(50);
            Serial.println("thrust jaw Halfway");
            delay(1000);
    }



    /*
           headLoop - General testing function for testing different head actions that are called depending
                      on how much force is applied ot the neck sensor.The commented code can be commented/uncommented
                      in order to test different parts. See the respective header files for the function descriptions.
    */
    void headLoop() {
      //      pulse.pulseByBPM(pulseNum);
      headState = neckSensor.forceRange(neckSensorPin);
      switch (headState) {
        case 0:
          //          alertActions();
          //          neck.moveToDeg(270);
          //          pulse.pulseByBPM(100);
          neck.neutral();
          //          eyeLids.stepLidsSleepy();
          eyeLids.idleBlink();
          //          eyeBalls.stepEyesLeftandRightSlow();
          //          eyeBalls.rollEyes();
          eyeBalls.idleEyes(0);
          //          jaw.stepVertOpenClose(3, 3);

          break;
        case 1:
          break;

        case 2:
          //          eyeBrows.raiseAndSlightRaise();
          break;

        case 3:
          //          eyeBalls.lookLeftandRight();
          eyeBalls.neutralBoth();
          break;

        case 4:
          eyeLids.winkLeft();

          //          eyeBalls.lookLeftandRight();
          eyeBalls.crossEyed();
          neck.wince(true);
          jaw.regOpenAndClose();
          //          neck.nodTimes(3, 250);
          //          neck.moveToDeg(0);
          //          eyeLids.longClose();
          //          painActions();
          //          pulse.pulseByBPM(160);
          break;
      }
    }

};
