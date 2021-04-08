/**********************************************************************************
   helperFunction.h

   This file includes additional functions that help with driving the servos and 
   testing their range of motion.
   
   Last Edited by Ethan Lauer on 4/8/20
 *********************************************************************************/

/* you can use this function if you'd like to set the pulse length in seconds
  e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!

    uint8_t n - unsigned integer of length 8 bits
    double pulse - the current pulse
*/
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;

  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period");
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit");
  pulse *= 1000000;  // convert to us
  pulse /= pulselength;
  Serial.println(pulse);
  pwm0.setPWM(n, 0, pulse);
  Serial.begin(9600); // open the serial port at 9600 bps:

}

/*
   degree to pulse
      int deg - integer that is a degree for the servo
      int servoType - integer that indicates the type of servo being used
                      servoType 0 corresponds to the small turnigy (small black) motors
                      servoType 1 corresponds to the towerpro (blue) motors
                      servoType 2 corresponds to the (black and red) jx motors
          take a degree and convert it to pulse based on servoType
          the pulse length varies based on the type of servo so it is mapped from 0-180 to other ranges
*/
uint16_t deg2Pulse(int deg, int servoType) {
  uint16_t pulse = 0;
  switch (servoType) {
    case (0):
      pulse = (uint16_t) map(deg, 0, 180, TURNIGYMIN, TURNIGYMAX);
      break;
    case (1):
      pulse = (uint16_t) map(deg, 0, 180, TOWERPROMIN, TOWERPROMAX);
      break;
    case (2):
      pulse = (uint16_t) map(deg, 0, 180, DATANMIN, DATANMAX);
      break;
  }
  return pulse;
}

/*
   testing a given servo
      int servo - integer indicated the servo type
                   servoType 0 corresponds to the small turnigy (small black) motors
                   servoType 1 corresponds to the towerpro (blue) motors
                   servoType 2 corresponds to the (black and red) jx motors
            this moves the servo to the maximums and minimum range of motion (0-180 deg)
            by converting the degree to a pulse then seting the PWM of the pin
*/
void servoTest(int servo) {
  uint16_t mini = deg2Pulse(0, servo);
  uint16_t maxi = deg2Pulse(180, servo);

  //  channel: The channel that should be updated with the new values (0..15)
  //  on: The tick (between 0..4095) when the signal should transition from low to high
  //  off:the tick (between 0..4095) when the signal should transition from high to low
  pwm0.setPWM(0, 0, mini);
  delay(1000);
  pwm0.setPWM(0, 0, maxi);
  delay(1000);
}

/*
   Drive a given servo
       int servoNum - Server number you want to move
       int deg - degree to move servo to
       int type - type of servo
           based on the servoNumber/pin number, it will select which
           driver to select and then set the pwm to that pin
*/
void driveServo(int servoNum, int deg, int type) {
  int servoNumAdjusted = servoNum % 16;       //why do you need to use modulo?
  switch (servoNum) {
    case 0 ... 15:
      pwm0.setPWM(servoNumAdjusted, 0, deg2Pulse(deg, type));
      break;
    case 16 ... 31:
      pwm1.setPWM(servoNumAdjusted, 0, deg2Pulse(deg, type));
      break;
    case 32 ... 47:
      pwm2.setPWM(servoNumAdjusted, 0, deg2Pulse(deg, type));
      break;
    case 48 ... 63:
      pwm3.setPWM(servoNumAdjusted, 0, deg2Pulse(deg, type));
      break;
  }
  delay(motorCommandPause);
}

/*manually adjust servo
     int servoid - the integer saying which servo to adjust
     int servotype - integer indicates the servo type
         The potentiometer attached to pin A14 is read,the current angle is mapped to a range of 0-180
         This angle is printed to the Serial monitor and the servo is moved to the new angle.
         Potentiometer is used to manually adjust the servo position
*/
void manualAdjustServo(int servoid, int servotype) {
  int potReading = analogRead(potpin);
  int angle;

    angle = map(potReading, 0, 1023, 0, 180);
 
  Serial.println(angle);
  driveServo(servoid, angle, servotype);
  delay(50);
}



//*******************************************SERIAL PRINTING**************************
void printColumnRow(int column, int row) {
  Serial.print("column = "); Serial.println(column);
  Serial.print("row = "); Serial.println(row);
}

void printIntOneLine(char text, int integer) {
  Serial.print(text); Serial.println(integer);
}
