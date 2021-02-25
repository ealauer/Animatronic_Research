/**********************************************************************************
   Voice
      Has overall class definition and private and public functions
   Modified from "WaveFilePlayer.ino" Example from Teensy Audio Library
   Edited by Ethan Lauer on 2/8/20

 *********************************************************************************/


class Voice {

  public:
    //************************************************************INITIALIZE AND SET UP********************************

    Voice() {

    }

    void setUp() {
      AudioMemory(8);
      while (!SD.begin(SDCARD_CS_PIN)) {
        Serial.println("Unable to access the SD card");
        delay(500);
      }
      // insert a "play file loop ( "setupNoise");
    }

    //************************************************************OTHER FUNCTIONS********************************

    /*
       playFileLoop - given a file name in ALL CAPS with a ".WAV" at the end, play the file until it is completely finished
                      note: this uses a while loop until the piece is done playing
                const char *filename - input of the file name
    */
    void playFileLoop(const char *filename) {
      Serial.print("Playing file: ");
      Serial.println(filename);

      // Start playing the file.  This sketch continues to
      // run while the file plays.
      playWav.play(filename);

      // A brief delay for the library read WAV info
      delay(5);

      // Simply wait for the file to finish playing.
      while (playWav.isPlaying()) {
        Serial.print("STILL Playing file: ");
//        Serial.println(filename);
//        delay(500);
      }
    }

    /*
       playFile - given a file name ALL CAPS with a ".WAV" at the end, play the file
                  check to see if the file is playing at the beginning of the function
                  if it is still playing, then do nothing.
              INPUT: const char *filename - input of the file name
    */
    void playFile(const char *filename) {
      Serial.print("Playing file: ");
      Serial.println(filename);
      // Start playing the file.  This sketch continues to
      // run while the file plays.
      playWav.play(filename);
      // A brief delay for the library read WAV info
      delay(5);
    }


    /*
        stillPlaying - return true if the audio is still playing, return false if it is not playing
    */
    boolean stillPlaying() {
      return playWav.isPlaying();
    }

};
