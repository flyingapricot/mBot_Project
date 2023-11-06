#include <MeMCore.h>

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

// change this to make the song slower or faster
int tempo = 114;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {
  NOTE_A4, 0, NOTE_A4, NOTE_A4,
  NOTE_C5, 0, NOTE_AS4, NOTE_A4, 
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5,
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, 
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, 
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, 
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, 
  NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_F4,
  NOTE_G4, 0, NOTE_G4, NOTE_D5,
  NOTE_C5, 0, NOTE_AS4, 0,
  NOTE_A4, 0, NOTE_A4, NOTE_A4,
  NOTE_C5, 0, NOTE_AS4, NOTE_A4, 
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4, 0, NOTE_G4, NOTE_D5,
  NOTE_C5, 0, NOTE_AS4, 0,
  NOTE_A4, 0, NOTE_A4, NOTE_A4,
  NOTE_C5, 0, NOTE_AS4, NOTE_A4, 
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5
 };
int noteDurations[] = {
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  };




MeDCMotor leftMotor(M1); // assigning leftMotor to port M1
MeDCMotor rightMotor(M2); // assigning RightMotor to port M2
MeLineFollower lineFollower(PORT_2); // assigning lineFollower to RJ25 port 2
MeBuzzer buzzer; // create the buzzer object

//By varying the analog output of A and B (At Port 4), we can control each of the LEDs + IR Emitter
#define A A0 //S1 (Pin 2 of Decoder)
#define B A1 //S2 (Pin 3 of Decoder)
#define LDR A2 //Using Port 3 S1 to read LDR Voltage
#define IRD A3 //Using Port 3 S2 to read IR Detector Voltage



int status = 0; // global status; 0 = do nothing, 1 = mBot runs

//High Low Pair used to specifically control each R, G and B LED by varying A and B's Analog Signal
struct LEDPair {
  uint8_t A_val;
  uint8_t B_val;
};


/** Start of mBot movement functions**/

// ⚪ Called when WHITE detected at waypoint.
// mBot stops and plays victory tune.
void stopRobot() {
  leftMotor.stop();
  rightMotor.stop();
}

// 🔴 Called when RED detected at waypoint.
// mBot turns to the Left by 90 degrees (on the spot).
void turnLeft() {
  leftMotor.run(255); // Positive: Left wheel revolves backwards
  rightMotor.run(255); // Positive: Right wheel revolves forwards
  delay(310);
  leftMotor.stop();
  rightMotor.stop();
}

// 🟢 Called when GREEN detected at waypoint.
//  mBot Turns to the Right by 90 degrees (on the spot).
void turnRight() {
  leftMotor.run(-255); // Negative: Left wheel revolves forwards
  rightMotor.run(-255); // Negative: Right wheel revolves backwards
  delay(310);
  leftMotor.stop();
  rightMotor.stop();
}

// 🟠 Called when ORANGE detected at waypoint.
// mBot does 180° turn within the same grid  
void uTurn() {
  // mBot turns to the Left by 180 degrees (on the spot).
  leftMotor.run(255); // Positive: Left wheel revolves backwards
  rightMotor.run(255); // Positive: Right wheel revolves forwards
  delay(575);  // Keep turning until turn is 180 degrees.

  // After turn, stop motors and wait for a short duration for mBot to stabilise.
  leftMotor.stop();
  rightMotor.stop();
  delay(300);
}

// 🟣 Called when PURPLE detected at waypoint.
// mbot does Two successive Left-turns in two grids
void successiveLeft() {
  // 1. Turn mBot to the left by 90 degrees.
  turnLeft();

  // 2. Then, move mBot forward by one tile.
  leftMotor.run(-255); // Negative: Left wheel revolves forwards
  rightMotor.run(255); // Positive: Right wheel revolves forwards
  delay(700);  // PREV VAL: 600

  // 3. After moving forward, stop motors and wait for a short duration for mBot to stabilise.
  leftMotor.stop();
  rightMotor.stop();
  delay(20);

  // 4. Finally, turn mBot to the left by 90 degrees again.
  turnLeft();
}

// 🔵 Called when BLUE detected at waypoint.
// mbot does Two successive Right-turns in two grids
void successiveRight() {
  // 1. Turn mBot to the right by 90 degrees.
  turnRight();

  // 2. Then, move mBot forward by one tile.
  leftMotor.run(-255); // Negative: Left wheel revolves forwards
  rightMotor.run(255); // Positive: Right wheel revolves forwards
  delay(700);  // PREV VAL: 600

  // 3. After moving forward, stop motors and wait for a short duration for mBot to stabilise.
  leftMotor.stop();
  rightMotor.stop();
  delay(20);

  // 4. Finally, turn mBot to the right by 90 degrees again.
  turnRight();
}

/** End of mBot movement functions**/


/** Start of LDR Related Definitions**/

// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 200 //in milliseconds
// Define time delay before taking another LDR reading
#define LDRWait 10 //in milliseconds

//Green LED(A - H, B - L) at Y1, Blue LED at Y2(A- L, B -H) , Red LED at Y3 (A- H, B -H)
LEDPair LED_Array[3];

//placeholders for colour detected
int red = 0;
int green = 0;
int blue = 0;

//floats to hold colour arrays
float colourArray[] = {0,0,0};
float whiteArray[] = {0,0,0};
float blackArray[] = {0,0,0};
float greyDiff[] = {0,0,0};

/** End of LDR Related Variables **/

enum Colours {
  detectPurple,
  detectGreen,
  detectBlue,
  detectRed,
  detectOrange,
  detectWhite
};

bool do_color_decode = false;


void celebrate() {
    for (int thisNote = 0; thisNote < 100; thisNote++) {
    int noteDuration = 750 / noteDurations[thisNote];
    buzzer.tone(melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    
    buzzer.noTone();
  }
}

// void moveForward() {// Code for moving forward for some short interval}
// void nudgeLeft() {// Code for nudging slightly to the left for some short
// interval}
// void nudgeRight() {// Code for nudging slightly to the right for some
// short interval}
int shineIR() {
  //Power on IR Emitter
  analogWrite(A,LOW); //Setting A0 to High/Low
  analogWrite(B,LOW); //Setting A1 to High/Low

  delay(500);

  int ans = analogRead(IRD);

  return ans;

}


int getAvgReading(int times){
  //find the average reading for the requested number of times of scanning LDR
  int reading;
  int total = 0;
  //take the reading as many times as requested and add them up
  for(int i = 0;i < times;i++){
    reading = analogRead(LDR);
    total = reading + total;
    delay(LDRWait);
  }
  //calculate the average and return it
  return total/times;
}

/** Start of LDR Colour Calibration**/


void setBalance() {
  //Set white balance
  Serial.println("Put White Sample For Calibration ...");
  delay(5000);

  //scan the white sample.
  //go through one colour at a time, set the maximum reading for each colour
  //--red, green and blue to the white array
  for(int i =0;i<3;i++) {
    digitalWrite(A,LED_Array[i].A_val); //Setting A0 to High/Low
    digitalWrite(B,LED_Array[i].B_val); //Setting A0 to High/Low
    delay(RGBWait);
    whiteArray[i] = getAvgReading(5); //Get average of 5 readings and store in white
    Serial.print("White Array ");
    Serial.print(i);
    Serial.print(" Value: ");
    Serial.println(whiteArray[i]);
  }

  Serial.println("Put Black Sample For Calibration ...");
  delay(5000); //delay for five seconds for getting sample ready
  
  //Next, scan black sample
  //go through one colour at a time, set the maximum reading for each colour
  //--red, green and blue to the black array
  for(int i =0;i<3;i++) {
    digitalWrite(A,LED_Array[i].A_val); //Setting A0 to High/Low
    digitalWrite(B,LED_Array[i].B_val); //Setting A0 to High/Low
    delay(RGBWait);

    blackArray[i] = getAvgReading(5); //Get average of 5 readings and store in white
    Serial.print("Black Array ");
    Serial.print(i);
    Serial.print(" Value: ");
    Serial.println(blackArray[i]);

    //the difference between the maximum and the minimum gives the range
    greyDiff[i] = whiteArray[i] - blackArray[i];
    Serial.print("Grey Array ");
    Serial.print(i);
    Serial.print(" Value: ");
    Serial.println(greyDiff[i]);
  }

  Serial.println("Colour Sensor Is Ready.");
}

/** End of LDR Colour Calibration**/



/** Start of LDR Colour Detection**/

void shineRed() {    
  analogWrite(A,LED_Array[0].A_val); //Setting A0 to High/Low
  analogWrite(B,LED_Array[0].B_val); //Setting A0 to High/Low
}

void shineGreen() {
  analogWrite(A,LED_Array[1].A_val); //Setting A0 to High/Low
  analogWrite(B,LED_Array[1].B_val); //Setting A0 to High/Low
}

void shineBlue() {
  analogWrite(A,LED_Array[2].A_val); //Setting A0 to High/Low
  analogWrite(B,LED_Array[2].B_val); //Setting A0 to High/Low
}

// Red [0], Green [1], Blue [2]
int detectColour()
{
  // Shine Red, read LDR after some delay
  shineRed();
  delay(RGBWait);
  colourArray[0] = getAvgReading(5);
  colourArray[0] = (colourArray[0] - blackArray[0])/(greyDiff[0])*255;
  delay(RGBWait);
  Serial.println(int(colourArray[0]));

  // Shine Green, read LDR after some delay
  shineGreen();
  delay(RGBWait);
  colourArray[1] = getAvgReading(5);
  colourArray[1] = (colourArray[1] - blackArray[1])/(greyDiff[1])*255;
  delay(RGBWait);
  Serial.println(int(colourArray[1]));

  // Shine Blue, read LDR after some delay
  shineBlue();
  delay(RGBWait);
  colourArray[2] = getAvgReading(5);
  colourArray[2] = (colourArray[2] - blackArray[2])/(greyDiff[2])*255;
  delay(RGBWait);
  Serial.println(int(colourArray[2]));
  analogWrite(A,LOW); //Setting A0 to High/Low
  analogWrite(B,LOW); //Setting A0 to High/Low

  // Red [0], Green [1], Blue [2]

  // If R, G and B near 255 = WHITE
  if (colourArray[0] >= 180 && colourArray[1] >= 180 && colourArray[2] >= 180) {
    Serial.println("ITS WHITE!!!");
    return detectWhite; // Stops
  }
  // When Green is MAX
  if (colourArray[1] > colourArray[0] && colourArray[1] > colourArray[2]) {
    double ratio = colourArray[1]/colourArray[2]; // Green/Blue ratio
    // Using Red as differentiator
    if (colourArray[0] >= 130) {
      Serial.println("ITS PURPLE!!!");
      return detectPurple; // Double Left turn
    }
    else if (colourArray[0] >= 95) {
      Serial.println("ITS BLUE!!!");
      return detectBlue; // Double Right turn
    } 
    else {
      Serial.println("ITS GREEN!!!");
      return detectGreen; // Right turn
    }
  }
  // When Red is MAX
  else if (colourArray[0] > colourArray[1] && colourArray[0] > colourArray[2]) {
      double ratio = colourArray[1]/colourArray[0]; // Green/Red ratio
      if (ratio < 0.9) {
        Serial.println("ITS RED!!!");
        return detectRed; // Left turn
      } 
      else {
        Serial.println("ITS ORANGE!!!");
        return detectOrange; // U-Turn
      }
  }
}

/** End of LDR Colour Detection**/


double ultrasonic_dist() {
  
  pinMode(ULTRASONIC, OUTPUT);
  digitalWrite(ULTRASONIC, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, LOW);
  pinMode(ULTRASONIC, INPUT);
  float duration = pulseIn(ULTRASONIC, HIGH, ULTRASONIC_TIMEOUT);

  float dist = -1;
  
  if (duration > 0) {
    dist = duration / 2.0 / 1000000 * SPEED_OF_SOUND * 100;
  }
  else {
    dist = -1;
  }
  return dist;
}


void setup()
{
  // setBalance();
  
  // Configure pinMode for A0, A1, A2, A3
  // Setting LED_Array
  // Red [0], Green [1], Blue [2]
  LED_Array[0] = {255, 255};
  LED_Array[1] = {255, 0};
  LED_Array[2] = {0, 255};
  
  //Setting A0(A) and A1(B) to output to control the LEDs + IR Emitter
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);

  //Setting A2 and A3 to input to recieve input from IR Detector + LDR
  pinMode(IRD, INPUT); //A1 recieves input from IR Detector
  pinMode(LDR, INPUT); //A2 recieves input from LDR
  
  pinMode(ULTRASONIC, OUTPUT);
  pinMode(A7, INPUT); // Setup A7 as input for the push button

  Serial.begin(9600);
  whiteArray[0] = 991;
  whiteArray[1] = 993;
  whiteArray[2] = 995;

  blackArray[0] = 954;
  blackArray[1] = 934;
  blackArray[2] = 926;
  
  greyDiff[0] = 37;
  greyDiff[1] = 59;
  greyDiff[2] = 69;

  celebrate();
}


// void celebrate() Code for playing celebratory tune
void rickroll() {
  int rickrollMelody[] = { 392, 587, 587, 659, 587, 523, 494, 440,
    392, 440, 494, 523, 494, 523, 587, 659, 587, 523, 587, 659,
    587, 659, 698, 784, 698, 659, 523, 587, 659, 587, 523, 587, 659
  };
  int rickrollNoteDuration[] = { 375, 375, 375, 375, 375, 750, 375, 375,
    375, 375, 375, 375, 375, 375, 375, 750, 375, 375, 375, 375,
    375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 750
  };

  for (int i = 0; i < sizeof(rickrollMelody) / sizeof(rickrollMelody[0]); i++) {
    if (rickrollMelody[i] == 0) {
      buzzer.noTone();
    } else {
      buzzer.tone(rickrollMelody[i], rickrollNoteDuration[i]);
    }
    delay(rickrollNoteDuration[i] * 1.30); // add a small delay between notes
  }
  buzzer.noTone();
}


void loop()
{

  if (analogRead(A7) < 100) { // If push button is pushed, the value will be very low
    status = 1 - status; // Toggle status
    //delay(500); // Delay 500ms so that a button push won't be counted multiple times.
  }

  if(status == 1) {
    leftMotor.run(-255); // Negative: wheel turns anti-clockwise
    rightMotor.run(255); // Positive: wheel turns clockwise
    double distance_right = ultrasonic_dist();
    if(distance_right < 11 && distance_right != -1) {
    //Too close to right, move left
      leftMotor.run(-190); // Left wheel stops
      rightMotor.run(255); // Right wheel go forward
      delay(15);
      //dist_from_left = ultrasonic_dist();
    //}
  }else if(distance_right > 12 && distance_right <= 20) {
    //To close to left, move to right
    //while(dist_from_left > 10) {
      leftMotor.run(-255); // Left wheel go forward
      rightMotor.run(190); // Right wheel stops
      delay(15);
      //dist_from_left = ultrasonic_dist();
  }

    int sensorState = lineFollower.readSensors(); // read the line sensor's state
    if (sensorState != S1_OUT_S2_OUT) { // run colour sensor when mbot stops at blackline
      leftMotor.stop(); // Left wheel Stop
      rightMotor.stop(); // Right wheel stop
      delay(500);

      enum Colours colour = detectColour();
      switch(colour) {
        case detectRed:
          Serial.println("RED DETECTED!!!!");
          turnLeft();
          break;
        case detectGreen:
          Serial.println("GREEN DETECTED!!!!");
          turnRight();
          break;
        case detectOrange:
          Serial.println("ORANGE DETECTED!!!!");
          uTurn();
          break;
        case detectPurple:
          Serial.println("PURPLE DETECTED!!!!");
          successiveLeft();
          break;
        case detectBlue:
          Serial.println("BLUE DETECTED!!!!");
          successiveRight();
          break;
        case detectWhite:
          leftMotor.stop();
          rightMotor.stop();
          celebrate(); // play "Never Gonna Give You Up"
          delay(5000);
          break;
      };
    }
  }
}
