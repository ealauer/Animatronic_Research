/**********************************************************************************
    camera.h

    This file contains the class definition, attributes, and methods For the camera.
    The Pixy2 camera uses the Arduino library that can be downloaded from the pixy
    website (https://pixycam.com/downloads-pixy2/). The camera settings (such as the
    signature setting, brightness, etc.) are set through the PixyMon software that is
    also a free download at the same website. The code is a modified version of the
    "cc_i2c_uart" example given in the Pixy2 library.

    Last Edited by Ethan Lauer 3/29/20
 *********************************************************************************/
#include <Arduino.h>
#include <Pixy2I2C.h> // include the Pixy iibrary available for download

class Camera {
  private:
    Pixy2I2C pixy;  // create the pixy camerea object
  public:
    // Maximum Pixel Values - these are public so they can be called in the
    // followHand() function in head.h.
    // Directions are assuming you are looking from the camera's point of view.
    // (0,0) point is in the top left corner of the camera's field of view.

    // X axis
    const int pixMaxLeft = 0;
    const int pixMaxRight = 316;
    //Y axis
    const int pixMaxUp = 0;
    const int pixMaxDown = 208;

    //************************************************INITIALIZE********************************
    // Define the camera
    Camera () {
    }

    /*
       setUp - run the init() function that is abstracted back into the pixy library
               print a statement so the user can see where in the setup the program is
    */
    void setUp() {
      Serial.println("Initializing Camera");
      pixy.init();
    }

    //**************************************** GET INFORMATION ************************************
    /*
      getBlockInfo - get any information from the detected blocks since (for now) HAL is only following
                     a single hand or block, only get the information if there was one block detected.
                     Note: this should be adjusted in the PixyMon software also so the camera is only
                           detecting one block maximum
            int state - an integer that corresponds to the requested information which can equal the following
              0 = x (0-316) and y (0-208) coordinates
              1= width (1 to 316)and height (1 to 208)
              2= angle (used for color code)(-180 to 180).
              3= index - tracking inted of block
              4= age- number of frames the bblock has been tracked  about 61 frames/sec (could be transfered to time)
              5= signature - what color block, what identified object (1-7)
          return array of integers result - if input state is not 0 or 1: the value is only in the first index[0] of the arrray
                                            see function for details
    */
    int *getBlockInfo(int state) {
      // create variables under a new name so it is easier
      // to read & understand in the switch case
      int requestedInfo = state;
      const int Coord = 0;
      const int Size = 1;
      const int Angle = 2;
      const int Index = 3;
      const int Age = 4;
      const int Signature = 5;

      static int result[2]; // result array
      pixy.ccc.getBlocks();   // get the block information
      if (pixy.ccc.numBlocks) {   // If there are detected blocks
        Serial.println("Detected ");
        if (pixy.ccc.numBlocks > 1) { // check that maximum of 1 block is detected otherwise print an error
          Serial.println("Too Many Blocks! Adjust Camera Settings or Eliminate Exessive Background");
        } else {
          switch (requestedInfo) {    // store the needed info in the result array
            case Coord:   // format: [x_position, y_position]
              result[0] = pixy.ccc.blocks[0].m_x;
              result[1] = pixy.ccc.blocks[0].m_y;
              break;

            case Size:// not used for this application. included only for future work.
              // format: [width, height]  Note: can be used to determine how close a standardized object is.
              result[0] = pixy.ccc.blocks[0].m_width;
              result[1] = pixy.ccc.blocks[0].m_height;
              break;

            case Angle: // not used for this application. included only for future work.
              result[0] = pixy.ccc.blocks[0].m_angle;
              result[1] = 0;
              break;

            case Index: // not used for this application. included only for future work.
              result[0] = pixy.ccc.blocks[0].m_index;
              result[1] = 0;
              break;

            case Age: // used to determine if the block is not longer detected - format: [age, 0]
              result[0] = pixy.ccc.blocks[0].m_age;
              result[1] = 0;
              break;

            case Signature: //not used for this application. included only for future work.
              // format: [signature, 0] - can be used to distinguish between
              // two different colored object (ex. blue gloved hand or a red
              // patch on a lab coat)
              result[0] = pixy.ccc.blocks[0].m_signature;
              result[1] = 0;
              break;
          }
        }
      }
      return result; // return the array
    }

};
