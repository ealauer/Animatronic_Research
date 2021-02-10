/**********************************************************************************
  forceSensor.h

  This file contains the class definition, attributes, and methods of a ForceSensor.
  The functions in this class include basic "analogRead()" function as well as a function
  that produces an integer depending how big of a force is produced. This code is
  modified from the example code found at Adafruit's website for the FSRs
  (https://learn.adafruit.com/force-sensitive-resistor-fsr/using-an-fsr). The force
  sensor pin ends up being polled in the head.h file. Five force sensors are used
  in total (see head.h for details).

  Last Edited by Ethan Lauer on 3/29/20
 *********************************************************************************/
#include<Arduino.h>

class ForceSensor {
  private:
    int forceSensorPin; // Analog Input Pin
    int sensorReading;  // stores sensor reading
  public:
    //************************************************INITIALIZE********************************
    // Define the Sensor
    ForceSensor(int forceSensorPin) {
      forceSensorPin = forceSensorPin;
    }

    /*
       setUp - set the respective pin to be read and print to the Serial monitor
            int forceSensorPin - analog pin to be read
    */
    void setUp(int forceSensorPin) {
      forceSensorPin = forceSensorPin;
      Serial.println("Initializing Force Sensor");
    }

    //************************************************SENSOR READINGS********************************

    /*
       NOTE: This could be replaced with "analogRead()". OR can be changed to compute the force in Newtons
       readRawAnalogInput - read the raw force sensor pin
              int forceSensorPin - pin to be read
             return int sensorReading - the resulting raw data
    */
    int readRawAnalogInput(int forceSensorPin) {
      sensorReading = analogRead(forceSensorPin);
      return sensorReading;
    }

    /*
       forceRange - read the force sensor and return an approximate range of the amount of force applied to the FSR
                    NOTE: These values may have to change. Might have to simply detect changes in the values of 900 because the sensor
                              will be pressed agains the neck, head, and skin. The values could also change by having the reading in Newtons
                              instead of the raw input data.
                int forceSensorPin - analog pin to be read
           return int result = 0-no pressure, 1 - very light force, 2- light force, 3- med force, 4- big force
    */
    int forceRange(int forceSensorPin) {
      sensorReading = readRawAnalogInput(forceSensorPin); // read the input pin (potentially change to be in Newtons)
      int result;   // change the result value based on the sensor reading.
      if (sensorReading < 100) { 
        Serial.println("No pressure");
        result = 0;
      } else if (sensorReading < 300) {
        Serial.println("Very Light Force");
        result = 1;
      } else if (sensorReading < 500) {
        Serial.println("Light Force");
        result = 2;
      } else if (sensorReading < 800) {
        Serial.println("Medium Force");
        result = 3;
      } else {
        Serial.println("Big Force");
        result = 4;
      }
      return result;  // output can then be passed into a switch case to easily switch actions.
    }

};
