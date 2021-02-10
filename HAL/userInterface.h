/**********************************************************************************
   User interface
      Has overall class definition and private and public functions

   Edited by Ethan Lauer on 10/23/19

 *********************************************************************************/
#include<Arduino.h>

class Display {

  private:
    // The shield uses the I2C SCL and SDA pins. On classic Arduinos
    // this is Analog 4 and 5 so you can't use those for analogRead() anymore
    // However, you can connect other I2C sensors to the I2C bus and share
    // the I2C bus.
    Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

    int column;
    int row;
    int *cell;
    boolean initRun = true;
    boolean initPulseRun = true;

  public:
    Display() {

    }

    void setUp() {
      lcd.begin(16, 2); //set up the LCD's number of columns and rows
      lcd.setBacklight(WHITE);
      int time = millis();
      lcd.print("Initialize Display and Keypad");
      time = millis() - time;
      Serial.println("Initialize Display and Keypad");
      Serial.print("Took "); Serial.print(time); Serial.println(" ms");
      delay(2000);
      reset();
    }

    // ************************************************ BASIC SCREEN FUNCITONS******************************************

    /*
       clear the screen of any outpus and set the backlight to white.
    */
    void reset() {
      lcd.clear();
      lcd.home();
      lcd.setBacklight(WHITE);
      lcd.noBlink();
    }

    /*
       clear the screen then print error with a red backlight
    */
    void error() {
      reset();
      lcd.print("ERROR ");
      lcd.setBacklight(RED);
      lcd.noBlink();
    }

    /*
       display the starting  screen
       LOC =  Level of Conciousness
       Air = Airway
       Pulse = Set a pulse
    */
    void startScreen() {
      reset();
      lcd.print("Set Med Settings");
      lcd.setCursor(0, 1);
      lcd.print("LOC Air Pulse");
      lcd.setCursor(0, 1);
    }

    void dispChangeSet() {
      reset();
      lcd.print("Change Settings?");
      lcd.setCursor(0, 1);
      lcd.print("Y or N");
      lcd.setBacklight(YELLOW);
    }

    void dispCheckStartSim() {
      reset();
      lcd.print("Start Sim?");
      lcd.setCursor(0, 1);
      lcd.print("Y or N");
      lcd.setBacklight(YELLOW);
    }

    void dispStartSim() {
      reset();
      lcd.noBlink();
      lcd.print("Starting");
      lcd.setCursor(0, 1);
      lcd.print("Simulation...");
      lcd.setBacklight(GREEN);
      delay(2000);
    }

    void dispSimInProgress() {
      reset();
      lcd.noBlink();
      lcd.print("Sim In Progress");
      lcd.setBacklight(GREEN);

    }

    /*
       checkStartSim - display and check if the user selected to start the simulation or not
            return true if correct cell was selected
    */
    boolean checkStartSim() {
      boolean result;
      dispCheckStartSim();
      cell = checkSelection(false);
      column = cell[0];
      row = cell[1];
      printColumnRow(column, row);
      if (row == 1 && column == yesCol) {
        result = true;
        dispStartSim();
        dispSimInProgress();
      } else if (row == 1 && column == noCol) {
        result = false;
      }
      return result;
    }



    /*
       checkSelection - determine which cell the user selected return an array [column,row]
            boolean isPulseSelection - true if this is the pulse selection in which case the starting cursor is in at a differnt location
            return integer array that holds the column and row that was selected
    */
    int *checkSelection(boolean isPulseSelection) {
      int column;
      int row;
      if (isPulseSelection) {
        column = 12;
        row = 0;
      } else {
        column = 0;
        row = 1;
      }

      lcd.setCursor(column, row);
      uint8_t buttons = lcd.readButtons();

      // while the user has not pressed the SELECT button
      //if they press left or right button, move the cursor to a specific column

      unsigned long timeNow = millis();
      int milliseconds = 250;

      while ( !(buttons & BUTTON_SELECT)) {
        if (initRun) {
          Serial.println("waiting for selection");
          initRun = false;
        }
        buttons = lcd.readButtons();
        if ((buttons & BUTTON_RIGHT) && column < 16) {
          delay(milliseconds);
          column++;
        }
        if ((buttons & BUTTON_LEFT) && column > 0) {
          delay(milliseconds);
          column--;
        }
        if ((buttons & BUTTON_DOWN) || (buttons & BUTTON_UP)) {
          delay(milliseconds);
          row = !row;
        }

        lcd.setCursor(column, row);
        lcd.blink();
      }
      initRun = true;
      static int result[2];
      result[0] = column;
      result[1] = row;

      return result;
    }



    /*
      checkIfChangeSettings  checks if you want to actually change the previously set settings
            int medState  =is the state to that you want to adjust or switch to
            String medSetting = this is used for printing to the serial monitor ********CAN REMOVE LATER ON ONCE TESTING WITH
            return integer that coorespons with the medical state or with the start menue
    */
    int checkIfChangeSettings(int medState, String medSetting) {
      int state;
      dispChangeSet();
      cell = checkSelection(false);
      column = cell[0];
      row = cell[1];
      printColumnRow(column, row);
      if (column == changeSetYesCell) { // yes I want to make changes
        state = medState;
        Serial.print("Switch to "); Serial.println(medSetting);

      } else if (column == changeSetNoCell) { // no I don't want to make changes
        state = StartMenuState;
      }
      return state;
    }


    //********************************************* LOC SETTING ***********************************************

    void dispLOCOptions() {
      reset();
      lcd.print("Select a LOC");
      lcd.setCursor(0, 1);
      lcd.print("A V P U");
    }

    void dispLOCSet() {
      lcd.noBlink();
      lcd.clear();
      lcd.home();
      lcd.print("LOC is Set");
      lcd.setBacklight(GREEN);
      delay(1500);
    }


    /*
      setLOC - set the level of conciousness
          return integer -the level of conciousness that will be passed to the head as a parameter
    */
    int setLOC() {
      Serial.println("Set  LOC");
      dispLOCOptions();
      cell = checkSelection(false);
      column = cell[0];
      row = cell[1];
      printColumnRow(column, row);
      int result;

      if (row == 0) {
        Serial.println("Wrong row.");
      } else if (column == alertCell) {
        Serial.println("Set as Alert");
        result = 0;
      } else if (column == verbalCell) {
        Serial.println("Set as Verbal");
        result = 1;

      } else if (column == painCell) {
        Serial.println("Set as Pain");
        result = 2;

      } else if (column == unrespCell) {
        Serial.println("Set as Unresponsive");
        result = 3;

      }
      dispLOCSet();
      Serial.println("Back to Main Menu");
      return result;
    }

    //****************************************************************** AIRWAY SETTING *********************************


    void dispAirNorm() {
      reset();
      lcd.print("Normal Airway?");
      lcd.setCursor(0, 1);
      lcd.print("Y or N?");
    }

    void dispLockJaw() {
      reset();
      lcd.print("Lock Jaw?");
      lcd.setCursor(0, 1);
      lcd.print("Y or N?");
    }

    void dispIrrBreath() {
      reset();
      lcd.print("Irreg Breath?");
      lcd.setCursor(0, 1);
      lcd.print("Y or N?");
    }

    void dispStridBreath() {
      reset();
      lcd.print("Stridor Breath?");
      lcd.setCursor(0, 1);
      lcd.print("Y or N?");
    }

    void dispAgonGasp() {
      reset();
      lcd.print("Agonal Gasp?");
      lcd.setCursor(0, 1);
      lcd.print("Y or N?");
    }

    void dispAirSet() {
      lcd.noBlink();
      lcd.clear();
      lcd.home();
      lcd.print("Air is Set");
      lcd.setBacklight(GREEN);
      delay(1500);

    }





    /*

      setAirway - sets the airway fnctions
         return the array of booleans for the head
                int normAirState=0;
                int lockJawState=1;
                int irrBreathState=2;
                int stridBreathState=3;
                int agonGaspState=4;
                int airSetState=5;
    */
    boolean *setAirway() {
      boolean *result;
      Serial.println("Set  Airway");

      int state = normAirState;
      while (state != 6) {
        switch (state) {
          case 0:           //normAirState:
            dispAirNorm();
            cell = checkSelection(false);
            column = cell[0];
            row = cell[1];
            printColumnRow(column, row);
            if (row == 1 && column == yesCol) {
              result[0] = true;
              state = airSetState;
            } else if (row == 1 && column == noCol) {
              result[0] = false;
              state = lockJawState;
            }
            break;

          case 1:           //lockJawState:
            dispLockJaw();
            Serial.println("In Lock Jaw");

            cell = checkSelection(false);
            column = cell[0];
            row = cell[1];
            printColumnRow(column, row);
            if (row == 1 && column == yesCol) {
              Serial.println("Selected Yes");

              result[1] = true;
              state = irrBreathState;
            } else if (row == 1 && column == noCol) {
              result[1] = false;
              state = irrBreathState;
            }
            break;

          case 2:         //irrBreathState:
            dispIrrBreath();
            cell = checkSelection(false);
            column = cell[0];
            row = cell[1];
            printColumnRow(column, row);
            if (row == 1 && column == yesCol) {
              result[2] = true;
              state = stridBreathState;
            } else if (row == 1 && column == noCol) {
              result[2] = false;
              state = stridBreathState;
            }
            break;

          case 3:       //stridBreathState:
            dispStridBreath();
            cell = checkSelection(false);
            column = cell[0];
            row = cell[1];
            printColumnRow(column, row);
            if (row == 1 && column == yesCol) {
              result[3] = true;
              state = airSetState;
            } else if (row == 1 && column == noCol) {
              result[3] = false;
              if (result[2] == true) {
                state = airSetState;
              } else {
                state = agonGaspState;
              }
            }
            break;

          case 4:             //agonGaspState:
            dispAgonGasp();
            cell = checkSelection(false);
            column = cell[0];
            row = cell[1];
            printColumnRow(column, row);
            if (row == 1 && column == yesCol) {
              result[4] = true;
              state = airSetState;
            } else if (row == 1 && column == noCol) {
              result[4] = false;
              if (result[0] == false && result[1] == false && result[2] == false && result[3] == false) {
                boolean newResult[] = {true};
                result = newResult;
              }
              state = airSetState;
            }
            break;
          case 5:             //airSetState:
            dispAirSet();
            state = 6;
            Serial.println("State is 6: break out of airway selection");
            break;

        }
      }
      return result;
    }



    //*********************************************************** PULSE SETTING ****************************************

    /*
       display options for narrowing down the pulse by either BPM or by a range
    */

    void dispPulseOptions() {
      reset();
      lcd.print("Certain BPM? Y N");
      lcd.setCursor(0, 1);
      lcd.print("BPM Range? Y N");
      lcd.setCursor(0, 12);
    }

    /*
       display the possible ranges, must select the first letter fo the word to select that pulse tempo
    */
    void dispPulseRanges() {
      reset();
      lcd.home();
      lcd.print("Select BPM Range");
      lcd.setCursor(0, 1);
      lcd.print("Norm Fast Slow");
    }

    /*
       display the input BPM
        start with 60
          user will later adjust using the up and down arrow keys
    */
    void dispPulseNum(int BPM) {
      if (initPulseRun) {
        reset();
        initPulseRun = false;
      } else {
        lcd.setCursor(0, 0);
        lcd.print("     ");
      }
      lcd.home();
      lcd.print("Input BPM");
      lcd.setCursor(0, 1);
      lcd.print(BPM);
      lcd.setCursor(0, 5);

    }

    void dispPulseSet() {
      reset();
      lcd.noBlink();
      lcd.print("Pulse is Set");
      lcd.setBacklight(GREEN);
      delay(1500);

    }

    /*
       manually adjust the pulse using the up and down arrow keys
    */
    int manualAdjustPulse() {
      int pulse = 60;
      uint8_t buttons = lcd.readButtons();

      while (!(buttons & BUTTON_SELECT)) {
        buttons = lcd.readButtons();

        if ((buttons & BUTTON_UP) && (pulse < 220)) {
          delay(250);
          pulse++;
        }
        if ((buttons & BUTTON_DOWN) && (pulse > 0)) {
          delay(250);
          pulse--;
        }
        dispPulseNum(pulse);
      }
      return pulse;
    }



    /*
      setPulse- set the pulse
          return an array that will tell the head what settings/pulse to have
            first index is if it was set by certain bumber or by range
            second index is the actual number produced

            int certOrRangeState=0;
            int certState=1;
            int rangeState=2;
            int pulseSetState=3;
    */
    int *setPulse() {
      Serial.println("Set Pulse");
      int state = certOrRangeState;
      static int result[2];

      while (state != 4) {

        switch (state) {
          case 0:       //certain or range state
            dispPulseOptions();
            Serial.println("Checking for pulse options");
            cell = checkSelection(true); // contains a while loop
            column = cell[0];
            row = cell[1];
            printColumnRow(column, row);

            if (row == certainBPMYes[0] && column == certainBPMYes[1] ) {   // set a specific BPM
              Serial.println("Change state to seting a certain BPM");

              state = certState;
            } else if (row == rangeBPMYes[0] && column == rangeBPMYes[1] ) { //set a range bpm
              Serial.println("Change state to seting a range BPM");

              state = rangeState;
            }
            break;

          case 1:     //certain bpm state
            result[0] = 0;
            Serial.println("Setting a certain BPM");
            dispPulseNum(60);
            int desiredPulseBPM;
            desiredPulseBPM = manualAdjustPulse();
            result[1] = desiredPulseBPM;
            state =  pulseSetState;
            break;

          case 2:     ///range bpm
            result[0] = 1;
            Serial.println("Setting a BPM Range");
            dispPulseRanges();
            cell = checkSelection(false);
            column = cell[0];
            row = cell[1];
            printColumnRow(column, row);

            if (row == 0) {
              //              delay(250);
              Serial.println("Wrong row.");
            } else if (column == normRangeCell) {
              //              delay(250);
              result[1] = 0;
              Serial.println("Set Pulse as Normal.");
            } else if (column == fastRangeCell) {
              //              delay(250);
              result[1] = 1;
              Serial.println("Set Pulse as Fast.");
            } else if (column == slowRangeCell) {
              //              delay(250);
              result[1] = 2;
              Serial.println("Set Pulse as Slow.");
            }
            //            delay(250);
            Serial.println("Back to Main Menu");
            state = pulseSetState;
            break;

          case 3:       //bpm is set
            Serial.println("Pulse Set");
            dispPulseSet();
            state = 4;
            break;

        }
      }
      return result;
    }

};
