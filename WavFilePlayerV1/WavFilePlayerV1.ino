/***************************************************

  This is an example code from the teensy audio library.

*************************************************************/

/***************************************************
  // Simple WAV file player example
  //
  // Three types of output may be used, by configuring the code below.
  //
  //   1: Digital I2S - Normally used with the audio shield:
  //         http://www.pjrc.com/store/teensy3_audio.html
  //
  //   2: Digital S/PDIF - Connect pin 22 to a S/PDIF transmitter
  //         https://www.oshpark.com/shared_projects/KcDBKHta
  //
  //   3: Analog DAC - Connect the DAC pin to an amplified speaker
  //         http://www.pjrc.com/teensy/gui/?info=AudioOutputAnalog
  //
  // To configure the output type, first uncomment one of the three
  // output objects.  If not using the audio shield, comment out
  // the sgtl5000_1 lines in setup(), so it does not wait forever
  // trying to configure the SGTL5000 codec chip.
  //
  // The SD card may connect to different pins, depending on the
  // hardware you are using.  Uncomment or configure the SD card
  // pins to match your hardware.
  //
  // Data files to put on your SD card can be downloaded here:
  //   http://www.pjrc.com/teensy/td_libs_AudioDataFiles.html
  //
  // This example code is in the public domain.

*************************************************************/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioPlaySdWav playWav;
// Use one of these 3 output types: Digital I2S, Digital S/PDIF, or Analog DAC
AudioOutputAnalogStereo dacs1;           //xy=329,47
//AudioOutputAnalog dacs2;     /      //xy=329,47

//AudioOutputAnalog      audioOutput;

AudioConnection patchCord1(playWav, dacs1);
//AudioConnection patchCor21(playWav, dacs2);

AudioControlSGTL5000 sgtl5000_1;

// Use these with the Teensy 3.5 & 3.6 SD card
#define SDCARD_CS_PIN BUILTIN_SDCARD


int state = 0;
boolean wasPlaying = false;
int playState = 0;

void setup() {
  Serial.begin(9600);

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(8);

  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
}

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
        Serial.println("Still playing");

    // uncomment these lines if you audio shield
    // has the optional volume pot soldered
    //    float vol = analogRead(15);
    //    vol = vol / 1024;
    //     sgtl5000_1.volume(vol);
  }
}


void playFile(const char *filename) {
  Serial.print("Playing file: ");
  Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playWav.play(filename);

  // A brief delay for the library read WAV info
  delay(5);
}

boolean stillPlaying() {
  return playWav.isPlaying();
}

boolean playFileBool(const char *filename) {
  boolean result;
  if (playWav.isPlaying()) {
    result = true;
  } else {
    Serial.print("Playing file: ");
    Serial.println(filename);

    // Start playing the file.  This sketch continues to
    // run while the file plays.
    playWav.play(filename);

    // A brief delay for the library read WAV info
    delay(5);
    result = false;
  }
  return result;
}



void loop() {
  playFileLoop("HELLO.WAV");
  //  Serial.print("start of loop...... state =");  Serial.println(state);
  //  switch (state) {
  //    case 0:
  //      playFile("HELLO.WAV");
  //      state = 1;
  //      break;
  //
  //    case 1:
  //      boolean isDonePlaying = stillPlaying();
  //      Serial.print("Is the song Still playing = ");      Serial.println(isDonePlaying);
  //
  //      if (!isDonePlaying) {
  //        state = 3;
  //      } else {
  //        state = 1;
  //      }
  //      break;
  //
  //    case 3:
  //      playFile("GHOST.WAV");
  //      Serial.println("FINISHED THE SONG! GOOD FOR YOU!");
  //      delay(5000);
  //      break;
  //  }
  //  Serial.print("END of loop...... state =");  Serial.println(state);

}
