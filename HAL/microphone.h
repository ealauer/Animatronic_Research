/**********************************************************************************

   Microphone Header file
      Has overall class definition and private and public functions
   Edited by Ethan Lauer on 2/19/20


 *********************************************************************************/
#include <Arduino.h>

// preliminary testing completed with poteniometer
// inherits analog sensor class
class Microphone {
  private:
    int micPin;
    int sensorReading;
    int state = 0;
    //    float millisecs = 3500;
    float timeNow = 0;
//    int trimmedVal;

  public:

    Microphone(int micPin) {
      micPin = micPin;
    }

    void setUp(int micPin) {
      micPin = micPin;
      pinMode(micPin, INPUT);
      Serial.println("Initializing Microphone");
    }

    int readRawAnalogInput(int micPin) {
      sensorReading = analogRead(micPin);
      return sensorReading;
    }


};
