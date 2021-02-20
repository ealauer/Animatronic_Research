/**********************************************************************************
   Pin Set Up header file
   Created by Ethan Lauer on 8/29/2019
      Edited by Ethan Lauer on 2/19/20

 *********************************************************************************/

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm0 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x42);
Adafruit_PWMServoDriver pwm3 = Adafruit_PWMServoDriver(0x43);
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(&Wire, 0x40);

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
//#define TOWERPROMIN  100 // this is the 'minimum' pulse length count (out of 4096)
//#define TOWERPROAVE  350
//#define TOWERPROMAX  600 // this is the 'maximum' pulse length count (out of 4096)

#define TOWERPROMIN  250 // this is the 'minimum' pulse length count (out of 4096)
#define TOWERPROAVE  300
#define TOWERPROMAX  450 // this is the 'maximum' pulse length count (out of 4096)

// tower pro min 150 and max is 600
// servo freq 50 (about 50 hz updates)

#define TURNIGYMIN  100 // this is the 'minimum' pulse length count (out of 4096)
#define TURNIGYAVE  350
#define TURNIGYMAX  600 // this is the 'maximum' pulse length count (out of 4096)


#define DATANMIN  90 // This is the 'minimum' pulse length count (out of 4096)
#define DATANMAX  500 // This is the 'maximum' pulse length count (out of 4096)


// *************************SERVO PINS*************
//EYE LIDS
const int botEyelidL = 0;
const int topEyelidL = 1;
const int botEyelidR = 2;
const int topEyelidR = 3;
//EYE BALLS
const int horEyeL = 4;
const int vertEyeL = 5;
const int horEyeR = 6;
const int vertEyeR = 7;
//EYEBROWS
const int medialLeft = 8;
const int lateralLeft = 9;
const int medialRight = 10;
const int lateralRight = 11;
//JAWS
const int jawLVert = 12;// test angles are 20 and 120 (based on test linkage and where servo horn connnected)
const int jawRVert = 13;
const int jawLHor = 14;
const int jawRHor = 15;

//NECK
const int rightTiltPin = 16;//20
const int leftTiltPin = 17;//21
const int rotPin = 18;//22

//MISC VARIABLES
int motorCommandPause = 5;
int potpin = A19;  // analog pin used to connect the potentiometer


// SERVO TYPES
int turnigy = 0;
int towerpro = 1;
int datan = 2;


//// SOLENIOD PULSE
const int leftPulsePin = 24;
const int rightPulsePin = 25;
unsigned long timeNow = 0;

//SPEAKER
AudioPlaySdWav    playWav;
AudioOutputAnalog   dacs1;
AudioConnection   patchCord1(playWav, dacs1);
#define SDCARD_CS_PIN BUILTIN_SDCARD

// LCD DEFINITIONS
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

// STATE MACHINE CONSTS
const int PreSimulationState = 0;
const int StartMenuState = 1;
const int LOCSettingState = 2;
const int AirwaySettingState = 3;
const int PulseSettingState = 4;
const int CheckStartSimState = 5;
const int StartSimulationState = 6;

//*********************************************UI CONSTS***************************
//Setting Changes
int changeSetYesCell = 0;
int changeSetNoCell = 5;

//OVERALL
int LOCCell = 0;
int airwayCell = 4;
int pulseCell = 8;
int startSim = 2;   // 2 if you are still changing settings, 0 if want to not do simulation, 1 if you want to start simulation


//LOC
int alertCell = 0;
int verbalCell = 2;
int painCell = 4;
int unrespCell = 6;

// AIRWAY
int yesCol = 0;
int noCol = 5;
int normAirState = 0;
int lockJawState = 1;
int irrBreathState = 2;
int stridBreathState = 3;
int agonGaspState = 4;
int airSetState = 5;


//PULSE
int certOrRangeState = 0;
int certState = 1;
int rangeState = 2;
int pulseSetState = 3;

int certainBPMYes[] = {0, 13};
int rangeBPMYes[] = {1, 11};
int normRangeCell = 0;
int fastRangeCell = 5;
int slowRangeCell = 10;

//****************************************************************SERVO CONSTANTS************************************************


// LEFT EYEBALL
//Up Down Servo Positions
const int leftUpVertEye = 100;
const int leftDownVertEye = 55;
//Left Right Servo Positions
const int leftLeftHorEye = 65;
const int leftRightHorEye = 124;


// RIGHT EYEBALL
//Up Down Servo Positions
const int rightUpVertEye = 30;
const int rightDownVertEye = 75;
//Left Right Servo Positions
const int rightLeftHorEye = 14;
const int rightRightHorEye = 53;
