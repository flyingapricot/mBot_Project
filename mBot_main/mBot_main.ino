#include <MeMCore.h>
#include "definitons.h"

/**Start of Music related Defintions **/
// change this to make the song slower or faster
int tempo = 150;
//Melody for Never gonna give you up
int melody[] = {
  NOTE_D5,16, NOTE_B4,16,
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,

  NOTE_E5,-8, NOTE_E5,-8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,-8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16, //18
  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,8, NOTE_A4,8, NOTE_A4,8, 
  NOTE_E5,4, NOTE_D5,2, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
 };

/**End of Music related Defintions **/


MeDCMotor leftMotor(M1); // assigning leftMotor to port M1
MeDCMotor rightMotor(M2); // assigning RightMotor to port M2
MeLineFollower lineFollower(PORT_2); // assigning lineFollower to RJ25 port 2
MeBuzzer buzzer; // create the buzzer object
MeRGBLed led(0,30); // Based on hardware connections on mCore; cannot change

bool adjustLeft;
bool adjustRight;


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

void moveForward() {
  leftMotor.run(-255); // Negative: wheel turns anti-clockwise
  rightMotor.run(255); // Positive: wheel turns clockwise
}

// 🔴 Called when RED detected at waypoint.
// mBot turns to the Left by 90 degrees (on the spot).
void turnLeft() {
  leftMotor.run(255); // Positive: Left wheel revolves backwards
  rightMotor.run(255); // Positive: Right wheel revolves forwards
  delay(300); //Previously 320/310
  leftMotor.stop();
  rightMotor.stop();
}

// 🟢 Called when GREEN detected at waypoint.
//  mBot Turns to the Right by 90 degrees (on the spot).
void turnRight() {
  leftMotor.run(-255); // Negative: Left wheel revolves forwards
  rightMotor.run(-255); // Negative: Right wheel revolves backwards
  delay(325); //Original: 315
}

// 🟠 Called when ORANGE detected at waypoint.
// mBot does 180° turn within the same grid  
void uTurn(double distance_right) {
  if(distance_right <= 8) {
    // mBot turns to the Left by 180 degrees (on the spot).
    leftMotor.run(255); // Positive: Left wheel revolves backwards
    rightMotor.run(255); // Positive: Right wheel revolves forwards
    delay(590);  // Keep turning until turn is 180 degrees. (previously 575)
  }else {
    // mBot turns to the Right by 180 degrees (on the spot).
    leftMotor.run(-255); // Positive: Left wheel revolves forwards
    rightMotor.run(-255); // Positive: Right wheel revolves backwards
    delay(590);  // Keep turning until turn is 180 degrees. (previously 575)
  }
}

// 🟣 Called when PURPLE detected at waypoint.
// mbot does Two successive Left-turns in two grids
void successiveLeft() {
  // 1. Turn mBot to the left by 90 degrees.
  turnLeft();

  // 2. Then, move mBot forward by one tile.
  moveForward();
  delay(700);  // PREV VAL:720/700

  // 3. After moving forward, stop motors and wait for a short duration for mBot to stabilise.
  leftMotor.stop();
  rightMotor.stop();
  delay(50);

  // 4. Finally, turn mBot to the left by 90 degrees again.
  turnLeft();
}

void successiveLeft2() {
  // 1. Turn mBot to the left by 90 degrees.
  turnLeft();

  // 2. Then, move mBot forward by one tile.
  moveForward();
  delay(710);  // PREV VAL:720/700

  // 3. After moving forward, stop motors and wait for a short duration for mBot to stabilise.
  leftMotor.stop();
  rightMotor.stop();
  delay(50);

  // 4. Finally, turn mBot to the left by 90 degrees again.
  turnLeft();
}




// 🔵 Called when BLUE detected at waypoint.
// mbot does Two successive Right-turns in two grids
void successiveRight() {
  // 1. Turn mBot to the right by 90 degrees.
  turnRight();

  // 2. Then, move mBot forward by one tile.
  moveForward();
  delay(800);  // PREV VAL: 600

  // 3. After moving forward, stop motors and wait for a short duration for mBot to stabilise.
  leftMotor.stop();
  rightMotor.stop();
  delay(50);

  // 4. Finally, turn mBot to the right by 90 degrees again.
  turnRight();
}
/** End of mBot movement functions**/


/** Start of LDR Related Definitions**/

//Green LED(A - H, B - L) at Y1, Blue LED at Y2(A- L, B -H) , Red LED at Y3 (A- H, B -H)
LEDPair LED_Array[3];

bool do_color_decode = false;

//placeholders for colour detected
int red = 0;
int green = 0;
int blue = 0;

//floats to hold colour arrays
float colourArray[] = {0,0,0};
float whiteArray[] = {0,0,0};
float blackArray[] = {0,0,0};
float greyDiff[] = {0,0,0};

enum Colours {
  detectPurple,
  detectGreen,
  detectBlue,
  detectRed,
  detectOrange,
  detectWhite
};


/** End of LDR Related Variables **/

void celebrate() {
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;

  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    buzzer.tone(melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    buzzer.noTone();
  }


}

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
    digitalWrite(A,LED_Array[i].A_val); 
    digitalWrite(B,LED_Array[i].B_val);
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
  analogWrite(A,LED_Array[0].A_val); 
  analogWrite(B,LED_Array[0].B_val);
}

void shineGreen() {
  analogWrite(A,LED_Array[1].A_val); 
  analogWrite(B,LED_Array[1].B_val);
}

void shineBlue() {
  analogWrite(A,LED_Array[2].A_val);
  analogWrite(B,LED_Array[2].B_val); 
}

int detectColour(){
  // Red [0], Green [1], Blue [2]
  // Shine Red, read LDR after some delay
  shineRed();
  delay(RGBWait);
  colourArray[0] = getAvgReading(6);
  shineGreen();
  delay(RGBWait);
  colourArray[1] = getAvgReading(6);
  shineBlue();
  delay(RGBWait);
  colourArray[2] = getAvgReading(6);

  analogWrite(A,LOW); 
  analogWrite(B,LOW);


  colourArray[0] = (colourArray[0] - blackArray[0])/(greyDiff[0])*255;
  Serial.println(int(colourArray[0]));

  colourArray[1] = (colourArray[1] - blackArray[1])/(greyDiff[1])*255;
  Serial.println(int(colourArray[1]));

  colourArray[2] = (colourArray[2] - blackArray[2])/(greyDiff[2])*255;
  Serial.println(int(colourArray[2]));


  // If R, G and B near 240 = WHITE
  if (colourArray[0] >= 240 && colourArray[1] >= 240 && colourArray[2] >= 240) {
    led.setColor(255, 255, 255); // set both LEDs to white colour
    led.show();
    return detectWhite;
  }
  // When Green is MAX
  if (colourArray[1] > colourArray[0] && colourArray[1] > colourArray[2]) {
        if(colourArray[2] <= 230) {
          led.setColor(0, 255, 0); // set both LEDs to green colour
          led.show();
          return detectGreen; 
        }else if(colourArray[0] >= 180) {
        led.setColor(128, 0, 128); // set both LEDs to purple colour
        led.show();
        return detectPurple;
    }else {
        led.setColor(0, 0, 255); // set both LEDs to blue colour
        led.show();
      return detectBlue;
    }
    //}
  }
  // When Red is MAX
  else if (colourArray[0] > colourArray[1] && colourArray[0] > colourArray[2]) {
      if (colourArray[1] <= 120) {
        led.setColor(255, 0, 0); // set both LEDs to red colour
        led.show();
        return detectRed;
      } 
      else {
        led.setColor(255, 165, 0); // set both LEDs to orange colour
        led.show();
        return detectOrange; // U-Turn
      }
  }else if(colourArray[2] > colourArray[0] && colourArray[2] > colourArray[1]) {
    if(colourArray[0] >= 180) {
        led.setColor(128, 0, 128); // set both LEDs to purple colour
        led.show(); // Must use .show() to make new colour take effect
      return detectPurple;
    }else {
        led.setColor(0, 0, 255); // set both LEDs to blue colour
        led.show();
      return detectBlue;
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


void setup(){

  led.setpin(13);

  // Setting LED_Array
  // Red [0], Green [1], Blue [2]
  LED_Array[0] = {255, 255};
  LED_Array[1] = {255, 0};
  LED_Array[2] = {0, 255};

  // Configure pinMode for A0, A1, A2, A3  
  //Setting A0(A) and A1(B) to output to control the LEDs + IR Emitter
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);

  //Setting A2 and A3 to input to recieve input from IR Detector + LDR
  pinMode(IRD, INPUT); //A1 recieves input from IR Detector
  pinMode(LDR, INPUT); //A2 recieves input from LDR
  
  pinMode(ULTRASONIC, OUTPUT);
  pinMode(A7, INPUT); // Setup A7 as input for the push button

  whiteArray[0] = 988;
  whiteArray[1] = 1009;
  whiteArray[2] = 992;

  blackArray[0] = 908;
  blackArray[1] = 987;
  blackArray[2] = 906;
  
  greyDiff[0] = 80;
  greyDiff[1] = 22;
  greyDiff[2] = 86;


}

void loop()
{

  if (analogRead(A7) < 100) { // If push button is pushed, the value will be very low
    status = 1 - status; // Toggle status
  }

  if(status == 1) {
    double distance_right = ultrasonic_dist();
    float dx = distance_right - 8;
     moveForward();

    if (distance_right < 9 && distance_right != -1) { // Left
      leftMotor.run(-255 - dx*15); // Negative: wheel turns anti-clockwise
      rightMotor.run(255); // Positive: wheel turns clockwise
      delay(50);
    }
    else if(distance_right > 11 && distance_right <= 16) { // Right
      leftMotor.run(-255); // Negative: wheel turns anti-clockwise
      rightMotor.run(255 - dx*15); // Positive: wheel turns clockwise
      delay(50);
    }

    int sensorState = lineFollower.readSensors(); // read the line sensor's state
    if (sensorState != S1_OUT_S2_OUT) { // run colour sensor when mbot stops at blackline
      leftMotor.stop(); // Left wheel Stop
      rightMotor.stop(); // Right wheel stop

      enum Colours colour = detectColour();
      switch(colour) {
        case detectRed:
          turnLeft();
          break;
        case detectGreen:
          turnRight();
          break;
        case detectOrange:
          uTurn(distance_right);
          break;
        case detectPurple:
          successiveLeft();
          break;
        case detectBlue:
          successiveRight();
          break;
        case detectWhite:
          leftMotor.stop();
          rightMotor.stop();
          celebrate(); // play "Never Gonna Give You Up"
          delay(100000);
          break;
      };
    }
  }
}
