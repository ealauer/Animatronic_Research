/**********************************************************************************
   Pulse file
      Has overall class definition and private and public functions

   Edited by Ethan Lauer on 1/29/20

 *********************************************************************************/
#include<Arduino.h>

class Pulse {

  private:
    int leftSolPin;
    int rightSolPin;

    unsigned long timeNow = 0;
    int state = 0;
    boolean isFirstRun = true;
    float milliseconds;

  public:
    //**********************INITIALIZE*******************************************

    // Create an object
    Pulse() {
    }


    void setUp() {
      leftSolPin = leftPulsePin;
      rightSolPin = rightPulsePin;
      isFirstRun = true;
      pinMode(leftSolPin, OUTPUT);
      pinMode(rightSolPin, OUTPUT);
      randomSeed(analogRead(A0)); //analog read an unused pin
      Serial.println("Initializing Pulse");
      off();
    }

    //**********************BASIC FUNCTIONS*******************************************

    //turn on
    void on() {
      digitalWrite(leftSolPin, HIGH);
      digitalWrite(rightSolPin, HIGH);
    }

    //turn off
    void off() {
      digitalWrite(leftSolPin, LOW);
      digitalWrite(rightSolPin, LOW);
    }

    //**********************SET PULSES*******************************************

    /*
      pulseByBPM - actuate solenoid at acertain BPM
          int BPM- the heart rate in BPM and it the solenoid will actuate at the desired rate evenly!
    */
    void pulseByBPM(int BPM) {
      milliseconds = (60000 / (BPM + 10)); // 60 seconds per minute, 60000 ms per minute  *add 10 to BPM because many delays in movements make the pulse change
      float actuationDelay = 30;
      switch (state) {
        case 0:
          on();
          if (millis() > timeNow + milliseconds) {
            timeNow = millis();
            state = 1;
          }
          break;
        case 1:
          off();
          if (millis() > timeNow + actuationDelay) {
            timeNow = millis();
            state = 0;
          }
          break;
      }
    }


    /*
      randPulse
            the solenoid will actuate random rates
    */
    void randPulse() {
      if (isFirstRun) {
        milliseconds = irregularPulse();
        isFirstRun = false;
      }
      float msDelay = 30;
      switch (state) {
        case 0:
          on();
          if (millis() > timeNow + milliseconds) {
            milliseconds = 60000 / (irregularPulse() + 10);
            timeNow = millis();
            state = 1;
          }
          break;

        case 1:
          off();
          if (millis() > timeNow + msDelay) {
            timeNow = millis();
            state = 0;
          }
          break;
      }
    }

    //**********************CALCULATING PUSLES*******************************************

    /*
      normalPulse
         actuate the solenoid at a normal range between 60-100 BPM(average for adults)
         NOTE: run first to generate a random normal pulse then put into pulseByBPM which is running in a loop

    */
    int normalPulse() {
      int normBPM;
      long randNum = random(60, 101); //max is exclusive
      normBPM = (int)randNum;
      Serial.println("Normal BPM = "); Serial.println(normBPM);
      //  pulseByBPM(normBPM);
      return normBPM;
    }


    /*
      fastPulse
       return a pulse withing a fast range between 100-220BPM
         NOTE: run first to generate a random fast pulse then put into pulseByBPM which is running in a loop

    */
    int fastPulse() {
      int fastBPM;
      long randNum = random(101, 221);
      fastBPM = (int)randNum;
      Serial.println("Fast BPM = "); Serial.println(fastBPM);
      return fastBPM;
    }

    /*
      slow Pulse
         return a pulse withing a low range between 10-60BPM
         NOTE: run first to generate a random slow pulse then put into pulseByBPM which is running in a loop
    */
    int slowPulse() {
      int slowBPM;
      long randNum = random(10, 60);
      slowBPM = (int)randNum;
      Serial.println("Slow BPM = "); Serial.println(slowBPM);
      return slowBPM;
    }

    /*
      irregularBeat
        return a pulse withing with an irregular rhythm
        NOTE: needs to be repeatedly calculated for it to be irregular

    */
    int irregularPulse() {
      int randBPM;
      long randNum = random(30, 220);
      randBPM = (int)randNum;
      Serial.print("Random BPM = "); Serial.println(randBPM);
      return randBPM;
    }
};
