#include <MeMCore.h>
#include "definitons.h"

/**Start of Music related Defintions **/
// change this to make the song slower or faster
int tempo = 150;
// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
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

MeEncoderMotor Encoder;

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
  //leftMotor.stop();
  //rightMotor.stop();
}

// 🟠 Called when ORANGE detected at waypoint.
// mBot does 180° turn within the same grid  
void uTurn(double distance_right) {
  if(distance_right <= 8) {
    // mBot turns to the Left by 180 degrees (on the spot).
    leftMotor.run(255); // Positive: Left wheel revolves backwards
    rightMotor.run(255); // Positive: Right wheel revolves forwards
    delay(590);  // Keep turning until turn is 180 degrees. (previously 575)

    // After turn, stop motors and wait for a short duration for mBot to stabilise.
    // leftMotor.stop();
    // rightMotor.stop();

    // leftMotor.run(-255); // Negative: wheel turns anti-clockwise
    // rightMotor.run(255); // Positive: wheel turns clockwise
    // delay(30);
  }else {
    // mBot turns to the Right by 180 degrees (on the spot).
    leftMotor.run(-255); // Positive: Left wheel revolves forwards
    rightMotor.run(-255); // Positive: Right wheel revolves backwards
    delay(590);  // Keep turning until turn is 180 degrees. (previously 575)

    // After turn, stop motors and wait for a short duration for mBot to stabilise.
    // leftMotor.stop();
    // rightMotor.stop();

    // leftMotor.run(-255); // Negative: wheel turns anti-clockwise
    // rightMotor.run(255); // Positive: wheel turns clockwise
    // delay(30);
  }

  //delay(300);
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
  //   for (int thisNote = 0; thisNote < 100; thisNote++) {
  //   int noteDuration = 750 / noteDurations[thisNote];
  //   buzzer.tone(melody[thisNote], noteDuration);
  //   int pauseBetweenNotes = noteDuration * 1.30;
  //   delay(pauseBetweenNotes);
    
  //   buzzer.noTone();
  // }
  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
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
int detectColour(){
  // Shine Red, read LDR after some delay
  shineRed();
  delay(RGBWait);
  colourArray[0] = getAvgReading(6); //originally 5
  shineGreen();
  delay(RGBWait);
  colourArray[1] = getAvgReading(6);
  shineBlue();
  delay(RGBWait);
  colourArray[2] = getAvgReading(6);
  //delay(RGBWait);
  analogWrite(A,LOW); //Setting A0 to High/Low
  analogWrite(B,LOW); //Setting A0 to High/Low


  colourArray[0] = (colourArray[0] - blackArray[0])/(greyDiff[0])*255;
  //delay(RGBWait);
  Serial.println(int(colourArray[0]));

  // Shine Green, read LDR after some delay

  colourArray[1] = (colourArray[1] - blackArray[1])/(greyDiff[1])*255;
  //delay(RGBWait);
  Serial.println(int(colourArray[1]));

  // Shine Blue, read LDR after some delay
  colourArray[2] = (colourArray[2] - blackArray[2])/(greyDiff[2])*255;
  //delay(RGBWait);
  Serial.println(int(colourArray[2]));

  // Red [0], Green [1], Blue [2]

  // If R, G and B near 255 = WHITE (prev 190)
  if (colourArray[0] >= 240 && colourArray[1] >= 240 && colourArray[2] >= 240) {
    //Serial.println("ITS WHITE!!!");
    led.setColor(255, 255, 255); // set both LEDs to white colour
    led.show(); // Must use .show() to make new colour take effect
    return detectWhite; // Stops
  }
  // When Green is MAX
  if (colourArray[1] > colourArray[0] && colourArray[1] > colourArray[2]) {
    // Using Red as differentiator
    // if (colourArray[0] >= 130) {
    //   Serial.println("ITS PURPLE!!!");
    //   return detectPurple; // Double Left turn
    // }
    // if (colourArray[1] >= 150) {
    //   Serial.println("ITS BLUE!!!");
    //   return detectBlue; // Double Right turn
    // } 
    //else {
      //Serial.println("ITS GREEN!!!");
        if(colourArray[2] <= 230) {
          led.setColor(0, 255, 0); // set both LEDs to white colour
          led.show(); // Must use .show() to make new colour take effect
          return detectGreen; // Right turn
        }else if(colourArray[0] >= 180) { //120 prev 280
      //Serial.println("ITS PURPLE!!!");
        led.setColor(128, 0, 128); // set both LEDs to white colour
        led.show(); // Must use .show() to make new colour take effect
        return detectPurple;
    }else {
        led.setColor(0, 0, 255); // set both LEDs to white colour
        led.show(); // Must use .show() to make new colour take effect
      //Serial.println("ITS BLUE!!!");
      return detectBlue;
    }
    //}
  }
  // When Red is MAX
  else if (colourArray[0] > colourArray[1] && colourArray[0] > colourArray[2]) {
      //Use blue/green ratio instead because green/red ratio is too close to compare
      double ratio = colourArray[2]/colourArray[1]; //  ̶G̶r̶e̶e̶n̶/̶R̶e̶d̶ ̶r̶a̶t̶i̶o̶  blue/green ratio
      if (colourArray[1] <= 120) { //Originally 165
        //Serial.println("ITS RED!!!");
        led.setColor(255, 0, 0); // set both LEDs to white colour
        led.show(); // Must use .show() to make new colour take effect
        return detectRed; // Left turn
      } 
      else {
        //Serial.println("ITS ORANGE!!!");
        led.setColor(255, 165, 0); // set both LEDs to white colour
        led.show(); // Must use .show() to make new colour take effect
        return detectOrange; // U-Turn
      }
  }else if(colourArray[2] > colourArray[0] && colourArray[2] > colourArray[1]) {
    if(colourArray[0] >= 180) { //120 prev 280
      //Serial.println("ITS PURPLE!!!");
        led.setColor(128, 0, 128); // set both LEDs to white colour
        led.show(); // Must use .show() to make new colour take effect
      return detectPurple;
    }else {
        led.setColor(0, 0, 255); // set both LEDs to white colour
        led.show(); // Must use .show() to make new colour take effect
      //Serial.println("ITS BLUE!!!");
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
  delayMicroseconds(10); //originally 10
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
  //adjustLeft = false;
  //adjustRight = false;
  // Configure pinMode for A0, A1, A2, A3
  // Setting LED_Array
  // Red [0], Green [1], Blue [2]
  led.setpin(13);
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
  //celebrate();

  Serial.begin(9600);
  //setBalance();
  // whiteArray[0] = 991;
  // whiteArray[1] = 993;  // whiteArray[2] = 995;

  // blackArray[0] = 954;
  // blackArray[1] = 934;
  // blackArray[2] = 926;
  
  // greyDiff[0] = 37;
  // greyDiff[1] = 59;
  // greyDiff[2] = 69;


  // whiteArray[0] = 989;
  // whiteArray[1] = 998;
  // whiteArray[2] = 946;

  // blackArray[0] = 966;
  // blackArray[1] = 907;
  // blackArray[2] = 709;
  
  // greyDiff[0] = 23;
  // greyDiff[1] = 91;
  // greyDiff[2] = 237;


  // whiteArray[0] = 984;
  // whiteArray[1] = 999;
  // whiteArray[2] = 997;

  // blackArray[0] = 957;
  // blackArray[1] = 917;
  // blackArray[2] = 943;
  
  // greyDiff[0] = 27;
  // greyDiff[1] = 82;
  // greyDiff[2] = 54;

  //Calibration 15 Nov
  whiteArray[0] = 988;
  whiteArray[1] = 1009;
  whiteArray[2] = 992;

  blackArray[0] = 908;
  blackArray[1] = 987;
  blackArray[2] = 906;
  
  greyDiff[0] = 80;
  greyDiff[1] = 22;
  greyDiff[2] = 86;

  //celebrate();

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
    int val = analogRead(LIGHTSENSOR);
    //Serial.println(val);
    //Check the Light sensor
    //If light sensor is covered, then switch to other mode, otherwise carry on current
    //value ranges from 0 to 1023
    if(val < 700) {
      //Do pre-loaded stuff here
      //Set led colour
      led.setColor(255, 0, 0); // set both LEDs to white colour
      led.show(); // Must use .show() to make new colour take effect
      int count = 0;
      while(count < 8) {
        double distance_right = ultrasonic_dist();
        float dx = distance_right - 8;
        moveForward();

        if (distance_right < 9 && distance_right != -1) { // Left (11)
          leftMotor.run(-255 - dx*15); // Negative: wheel turns anti-clockwise
          rightMotor.run(255); // Positive: wheel turns clockwise
          delay(50); // 5
        }
        else if(distance_right > 11 && distance_right <= 16) { // Right (original:20 -> 16)
          leftMotor.run(-255); // Negative: wheel turns anti-clockwise
          rightMotor.run(255 - dx*15); // Positive: wheel turns clockwise
          delay(50);
        }

        int sensorState = lineFollower.readSensors(); 
        if(sensorState != S1_OUT_S2_OUT) {
          leftMotor.stop(); // Left wheel Stop
          rightMotor.stop(); // Right wheel stop
          if(count == 7) {
            //White
            delay(100);
            led.setColor(255,255,255);
            led.show();
            leftMotor.stop();
            rightMotor.stop();
            celebrate(); // play "Never Gonna Give You Up"
            delay(100000);
          }

          if(count == 0) {delay(100);led.setColor(255,0,0);led.show();turnLeft();count++;continue;}
          else if(count == 1) {delay(100);led.setColor(255,0,0);led.show();turnLeft();count++;continue;}
          else if(count == 2) {delay(100);led.setColor(0,0,255);led.show();successiveRight();count++;continue;}
          else if(count == 3) {delay(100);led.setColor(255, 165, 0);led.show();uTurn(distance_right);count++;continue;}
          else if(count == 4) {delay(100);led.setColor(128, 0, 128);led.show();successiveLeft2();count++;continue;}
          else if(count == 5) {delay(100);led.setColor(0,255,0);led.show();turnRight();count++;continue;}
          else if(count == 6) {delay(100);led.setColor(0,255,0);led.show();turnRight();count++;continue;}
        }
      }

    }
    //delay(500); // Delay 500ms so that a button push won't be counted multiple times
  }

  if(status == 1) {
    double distance_right = ultrasonic_dist();
    float dx = distance_right - 8;
     moveForward();

    if (distance_right < 9 && distance_right != -1) { // Left (11)
      leftMotor.run(-255 - dx*15); // Negative: wheel turns anti-clockwise
      rightMotor.run(255); // Positive: wheel turns clockwise
      delay(50); // 5
    }
    else if(distance_right > 11 && distance_right <= 16) { // Right (original:20 -> 16)
      leftMotor.run(-255); // Negative: wheel turns anti-clockwise
      rightMotor.run(255 - dx*15); // Positive: wheel turns clockwise
      delay(50);
    }

    /*if(distance_right < 10 && distance_right != -1) {
      //Too close to right, move left
      leftMotor.run(-170); // Left wheel stops
      rightMotor.run(255); // Right wheel go forward
      delay(5);
      //dist_from_left = ultrasonic_dist();
    //}
  }else if(distance_right > 11 && distance_right <= 20) {
    //To close to left, move to right
    //while(dist_from_left > 10) {
      leftMotor.run(-255); // Left wheel go forward
      rightMotor.run(170); // Right wheel stops
      delay(5);
      //dist_from_left = ultrasonic_dist();
  }*/


  //   if(distance_right < 8.0 && distance_right > 0.1) { /** too close to right correct to the left, corrects more if the robot is closer to the wall */
  //   if(distance_right < 6.0 && distance_right > 0.1) {
  //     leftMotor.run(-140);
  //     rightMotor.run(255);
  //     delay(15);
  //   }
  //   else if(distance_right < 7.0) {
  //     leftMotor.run(-160);
  //     rightMotor.run(255);
  //     delay(15);
  //   }
  //   else if(distance_right < 7.5) {
  //     leftMotor.run(-190);
  //     rightMotor.run(255);
  //     delay(15);
  //   }
  //   else {
  //     leftMotor.run(-215);
  //     rightMotor.run(255);
  //     delay(15);
  //   }
  // }

  // if(distance_right > 9.0 && distance_right < 15.0) { /** too close to right correct to the left, corrects more if the robot is closer to the wall */
  //   if(distance_right >= 13.0) {
  //     leftMotor.run(-140);
  //     rightMotor.run(255);
  //     delay(15);
  //   }
  //   else if(distance_right < 12.0) {
  //     leftMotor.run(-160);
  //     rightMotor.run(255);
  //     delay(15);
  //   }
  //   else if(distance_right < 11) {
  //     leftMotor.run(-190);
  //     rightMotor.run(255);
  //     delay(15);
  //   }
  //   else {
  //     leftMotor.run(-215);
  //     rightMotor.run(255);
  //     delay(15);
  //   }
  // }

    /*if( (int)distance_right == -1) {
      if(distance_right <= 11) {
        leftMotor.run(-190); // Left wheel stops
        rightMotor.run(255); // Right wheel go forward
        delay(15);
      }
      if(distance_right >= 12 && distance_right <= 16) {
        leftMotor.run(-255); // Left wheel go forward
        rightMotor.run(190); // Right wheel stops
        delay(15);
      }
  //   }else*//*else {
    //Before moving straight,check if aligned to the left or the right before moving
    if(adjustLeft) {
      //Then veer slightly to the right
      leftMotor.run(-255); // Left wheel go forward
      rightMotor.run(170); // Right wheel stops
      delay(10);
    }else if(adjustRight) {
      //Then veer slightly to the left
      leftMotor.run(-190); //Left wheel stops
      rightMotor.run(255); // Right wheel go forward
      delay(10);
    }
    leftMotor.run(-255); // Negative: wheel turns anti-clockwise
    rightMotor.run(255); // Positive: wheel turns clockwise (235)
    adjustLeft = false;
    adjustRight = false;
  }*/


    int sensorState = lineFollower.readSensors(); // read the line sensor's state
    if (sensorState != S1_OUT_S2_OUT) { // run colour sensor when mbot stops at blackline
      leftMotor.stop(); // Left wheel Stop
      rightMotor.stop(); // Right wheel stop
      //delay(200); //Originally 500

      enum Colours colour = detectColour();
      switch(colour) {
        case detectRed:
          //Serial.println("RED DETECTED!!!!");
          turnLeft();
          break;
        case detectGreen:
          //Serial.println("GREEN DETECTED!!!!");
          turnRight();
          break;
        case detectOrange:
          //Serial.println("ORANGE DETECTED!!!!");
          uTurn(distance_right);
          break;
        case detectPurple:
          //Serial.println("PURPLE DETECTED!!!!");
          successiveLeft();
          break;
        case detectBlue:
          //Serial.println("BLUE DETECTED!!!!");
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
