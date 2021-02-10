/**********************************************************************************
  MQPOOPv1.ino
  
  Animatronic Head MQP 2019-2020

  This code is used to control the Humanoid Animatronic Learning Simulator for
  Medical Interactive Training (H.A.L. S.M.I.T., or HAL for short). This is the main
  .ino file that includes all the necessary header files and has the primary switch
  case that controls the overal status of HAL. In this file, the head and display
  objects are created and interact with one another. The display is the user interface
  (UI) that is used to set the three different settings (Level of Conciousness - LOC,
  Airway - Air, Circulation - Pulse). The first few states are where Head's state
  variables are set by the UI. The final "StartSimulationState" is where the head
  completes the desired test. The "head.LOCTEST()" function runs the LOC test with
  all the setting in place for all tests (LOC, Airway, and Circulation). Therefore,
  once the LOC test is complete, HAL will be idleing and the user can perform the
  Airway and Circulation tests. Due to time constraints and unforeseen circumstances,
  there is not a built in feature that ends the simulation. Several of the Serial.print
  messages can be commented out without affecting the code but are very useful
  when debugging.

  Created by Ethan Lauer on 10/3/19
  Last Edited by Ethan Lauer on 3/28/20
*********************************************************************************/

//Include Libraries
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <Audio.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//Include Header Files and Necessary Classes
#include "pinSetUp.h"
#include "helperFunctions.h"
#include "head.h"
#include "userInterface.h"

//Create Head and UI Objects
Head head = Head();
Display LCD = Display();

// State Vairable for the Overall System (Comment out the state that is not being used)
// The StartSimulationState is used for skipping past the user input and only uses default values.
// Case variables are defined as globals (see pinSetUp.h).
volatile byte state = PreSimulationState; // For Full Code
//volatile byte state = StartSimulationState; // For Skipping to a Test

// Set up - runs once on start up
void setup() {
  Serial.begin(9600);
  Serial.println("Initializing HAL's Brain...");

  //Start the PWM servos
  pwm0.begin();
  pwm1.begin();
  pwm2.begin();
  pwm3.begin();
  //this is the mx PWM freq
  pwm0.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  pwm1.setPWMFreq(60);
  pwm2.setPWMFreq(60);
  pwm3.setPWMFreq(60);

  //Set up and initialize the head and LCD
  head.setUp();
  LCD.setUp();
}


// Loop runs infinitly
void loop() {
  //Define variables for columns and rows.
  //These variables will change as the code progresses.
  int *cell;
  int column;
  int row;

  switch (state) {
    case PreSimulationState:    //************************************** PRE SIMULATION STATE **********************************
      // Check what settings are left and return to the appropriate menu display
      if (head.isAllSet() && startSim == 2) {
        state = CheckStartSimState;
      } else if (head.isAllSet() && startSim == 0) {
        startSim = 2;
        state = StartMenuState;
      } else {
        state = StartMenuState;
      }
      break;

    case StartMenuState:    //************************************** STARTING MAIN MENU**********************************
      LCD.startScreen();
      cell = LCD.checkSelection(false); // Enters While loop waiting for user input
      column = cell[0];
      row = cell[1];
      printColumnRow(column, row);

      // Cursor was on the wrong row
      if (row == 0) {
        Serial.println("Wrong row.");
      }
      // *********If LOC was selected*********
      else if (column == LOCCell && !head.isLOCSet) { // If the LOC was not set yet
        state = LOCSettingState;
        Serial.println("Switch to LOC Set");
      } else if (column == LOCCell && head.isLOCSet) {  // do you actually want to make a change to previously set setting?
        state = LCD.checkIfChangeSettings(LOCSettingState, "LOC");
      }

      // *********If AIRWAY was selected************
      else if (column == airwayCell && !head.isAirSet) { // If Air was not set yet
        state = AirwaySettingState;
        Serial.println("Switch to Airway Set");
      } else if (column == airwayCell && head.isAirSet) {   // do you actually want to make a change to previously set setting?
        state = LCD.checkIfChangeSettings(AirwaySettingState, "Airway");
      }

      // **********If PULSE was selected*********
      else if (column == pulseCell && !head.isPulseSet) { // If Pulse was not Set
        state = PulseSettingState;
        Serial.println("Switch to Pulse Set");
      } else if (column == pulseCell && head.isPulseSet) {// do you actually want to make a change to previously set setting?
        state = LCD.checkIfChangeSettings(PulseSettingState, "Pulse");
      }
      break;

    case LOCSettingState:    //******************************** LOC Setting**********************
      int stateLOC;
      stateLOC = LCD.setLOC();    // get user input
      head.setLOC(stateLOC);      // pass setting into Head Class
      state = PreSimulationState; // return to pre sim state
      break;

    case AirwaySettingState:    //************************************** AIRWAY  Setting**********************************
      Serial.println("Set  Airway");
      boolean *airSettings;
      airSettings = LCD.setAirway();  // get user input
      head.setAir(airSettings);       // pass setting into Head Class
      state = PreSimulationState;     // return to pre sim state
      break;

    case PulseSettingState:     //************************************** PULSE  Setting**********************************
      int *pulseArray;
      pulseArray = LCD.setPulse();          // get user input
      if (pulseArray[0] == 0) {
        head.setPulseBPM(pulseArray[1]);    // pass a certain BPM into Head Class
      } else if (pulseArray[0] == 1) {
        head.setPulseRange(pulseArray[1]);   // pass a BPM range into Head Class
      }
      state = PreSimulationState;           // return to pre sim state
      break;

    case CheckStartSimState:      //************************************** Check to Start SIM**********************************
      if (LCD.checkStartSim()) {    // get user input to check if want to start simulation or go back and change settings
        startSim = 1;
        state = StartSimulationState;
      } else {
        startSim = 0;
        state = PreSimulationState;
      }
      break;

    case StartSimulationState:    //**************************************STARTING SIMULATION**********************************
      // Run the test
      head.LOCTEST();

      // The tests below were used for testing each component of the head before
      // integrating it with the rest of the system. To run these, use the "StartSimulationState"
      // for the state of this switch case. Then uncomment any of the functions below. The
      // functions below were used often to test various parts of the code.

      //      head.neckTest();
      //      head.jawTest();
      //      head.voiceTest();
      //      head.headLoop();
      //      head.fsrTest();
      //      head.pulseTest();
      //      head.followHand();
      //      head.micTest();
      break;
  }

}
