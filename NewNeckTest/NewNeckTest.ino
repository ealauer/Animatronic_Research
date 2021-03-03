/**********************************************************************************

  Neck test file for stewart platform
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



// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm0 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);


//NECK SERVO PINS ON DRIVERS
const int Leg1 = 16;
const int Leg2 = 17;
const int Leg3 = 18;
const int Leg4 = 19;
const int Leg5 = 20;
const int Leg6 = 21;



void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
  Serial.println("Initializing HAL's Brain...");

  //Start the PWM servos
  pwm0.begin();
  pwm1.begin();

  //this is the mx PWM freq
  pwm0.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  pwm1.setPWMFreq(60);

}

void loop() {
  // put your main code here, to run repeatedly:

}
