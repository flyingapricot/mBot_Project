#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// This file contains all definitions and constant arrays/global values

int status = 0; // Global status; 0 = Do nothing, 1 = mBot runs

// By varying the analog output of A and B (At Port 4), we can control each of the LEDs + IR Emitter
#define A A0 // S1 (Pin 2 of Decoder)
#define B A1 // S2 (Pin 3 of Decoder)
#define LDR A2 // Using Port 3 S1 to read LDR Voltage
#define IRD A3 // Using Port 3 S2 to read IR Detector Voltage

#define LIGHTSENSOR A6 // internally connected to analog pin A6 in mCore

MeDCMotor leftMotor(M1); // Assigning leftMotor to port M1
MeDCMotor rightMotor(M2); // Assigning RightMotor to port M2
MeLineFollower lineFollower(PORT_2); // Assigning lineFollower to RJ25 port 2
MeBuzzer buzzer; // Create the buzzer object
MeRGBLed led(0,30); // Based on hardware connections on mCore; cannot change


/** Start of LDR Related Definitions **/

// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 50 // in milliseconds
// Define time delay before taking another LDR reading
#define LDRWait 15 // in milliseconds

// Red LED at Y3 (A- H, B -H)
// Green LED Y1 (A - H, B - L)
// Blue LED at Y2 (A- L, B -H)

// High Low Pair used to specifically control each R, G and B LED by varying A and B's Analog Signal
struct LEDPair {
  uint8_t A_val;
  uint8_t B_val;
};

LEDPair LED_Array[3];
bool do_color_decode = false;

// Placeholders for colour detected
int red = 0;
int green = 0;
int blue = 0;

// Floats to hold colour arrays
float colourArray[] = {0,0,0};
// White and Black array values obtained experimentally
float whiteArray[] = {988,1009,992};
float blackArray[] = {908,987,906};
float greyDiff[] = {80,22,86};

enum Colours {
  detectPurple,
  detectGreen,
  detectBlue,
  detectRed,
  detectOrange,
  detectWhite
};

/** End of LDR Related Definitions **/


/** Start of Music related Defintions **/

#define ULTRASONIC 12
#define ULTRASONIC_TIMEOUT 2000 // Max microseconds to wait; choose according to max distance of wall
#define SPEED_OF_SOUND 340 // Update according to your own experiment
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

// Increasing tempo makes the song faster (BPM)
int tempo = 150;

// Melody for "Never gonna give you up"
int melody[] = {
  NOTE_D5,16, NOTE_B4,16,
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_E5,-8, NOTE_E5,-8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,-8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16, // 18
  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,8, NOTE_A4,8, NOTE_A4,8, 
  NOTE_E5,4, NOTE_D5,2, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
 };

/** End of Music related Defintions **/


#endif
