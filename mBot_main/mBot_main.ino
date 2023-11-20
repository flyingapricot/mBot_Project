#include <MeMCore.h>
#include "definitons.h"
/**
 * @file mBot_main.ino
 * @brief mBot Movement and Sensor Control
 *
 * This file defines functions for controlling the movement of an mBot robot and integrating sensor functionalities,
 * including line following, color detection, and ultrasonic distance measurement.
 *
 * The file implements functions for mBot movements such as moving forward, stopping, turning left or right, performing
 * u-turns, and executing specific maneuvers based on color detection. Additionally, it includes functions for reading
 * sensor values, controlling the RGB LED, and playing a celebratory melody.
 *
 * The robot's behavior is controlled based on a global status variable. When the status is set to 1, the mBot performs
 * various movements and sensor-based actions. The status can be toggled using a push button.
 *
 * @note This code assumes the use of the MeMCore library for mBot control.
 * @note The color detection is based on readings from an LDR and an RGB LED.
 * @note The ultrasonic sensor is used for distance measurement.
 * @note The line follower is used to detect black lines.
 * @note The buzzer is used for playing celebratory melodies.
 * @note The RGB LED is used for indicating detected colors.
 *
 * @see definitions.h Header file containing additional definitions.
 *
 * @author S3T4
 */


/** Start of mBot Movement functions**/

//Moves the mBot forward in a straight line
void moveForward() {
  leftMotor.run(-255); // Negative: Left wheel revolves forwards
  rightMotor.run(255); // Positive: Right wheel revolves forwards
}

// ⚪ Called when WHITE detected at waypoint
// mBot stops and plays victory tune
void stopRobot() {
  leftMotor.stop();
  rightMotor.stop();
}

// 🔴 Called when RED detected at waypoint
// mBot turns to the Left by 90 degrees (on the spot)
void turnLeft() {
  leftMotor.run(255); // Positive: Left wheel revolves backwards
  rightMotor.run(255); // Positive: Right wheel revolves forwards
  delay(300); 
  leftMotor.stop();
  rightMotor.stop();
}

// 🟢 Called when GREEN detected at waypoint
//  mBot Turns to the Right by 90 degrees (on the spot)
void turnRight() {
  leftMotor.run(-255); // Negative: Left wheel revolves forwards
  rightMotor.run(-255); // Negative: Right wheel revolves backwards
  delay(325); 
}

// 🟠 Called when ORANGE detected at waypoint
// mBot does 180° turn within the same grid  
void uTurn(double distance_right) {
  if(distance_right <= 8) {
    // mBot turns to the Left by 180 degrees (on the spot)
    leftMotor.run(255); // Positive: Left wheel revolves backwards
    rightMotor.run(255); // Positive: Right wheel revolves forwards
    delay(590); 
  } else {
    // mBot turns to the Right by 180 degrees (on the spot)
    leftMotor.run(-255); // Positive: Left wheel revolves forwards
    rightMotor.run(-255); // Positive: Right wheel revolves backwards
    delay(590); 
  }
}

// 🟣 Called when PURPLE detected at waypoint
// mbot does Two successive Left-turns in two grids
void successiveLeft() {
  // Turn mBot to the left by 90 degrees.
  turnLeft();

  // Then, move mBot forward by one tile.
  moveForward();
  delay(700); 

  // After moving forward, stop motors and wait for a short duration for mBot to stabilise.
  leftMotor.stop();
  rightMotor.stop();
  delay(50);

  // Finally, turn mBot to the left by 90 degrees again.
  turnLeft();
}

// 🔵 Called when BLUE detected at waypoint.
// mbot does Two successive Right-turns in two grids
void successiveRight() {
  // Turn mBot to the right by 90 degrees.
  turnRight();

  // Then, move mBot forward by one tile.
  moveForward();
  delay(800);  // PREV VAL: 600

  // After moving forward, stop motors and wait for a short duration for mBot to stabilise.
  leftMotor.stop();
  rightMotor.stop();
  delay(50);

  // Finally, turn mBot to the right by 90 degrees again.
  turnRight();
}
/** End of mBot Movement functions **/


float shineIR() {
  // Power on IR Emitter
  analogWrite(A,LOW); //Setting A0 to High/Low
  analogWrite(B,LOW); //Setting A1 to High/Low

  delay(500);

  float raw = analogRead(IRD);
  float ans = (raw - 6.525)/19.826;
  return ans;
}

int getAvgReading(int times) {
  // Find the average reading for the requested number of times of scanning LDR
  int reading;
  int total = 0;
  // Take the reading as many times as requested and add them up
  for(int i = 0;i < times;i++){
    reading = analogRead(LDR);
    total = reading + total;
    delay(LDRWait);
  }
  // Calculate the average and return it
  return total/times;
}

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


void celebrate() {
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;

  // Calculates the duration of a whole note in milliseconds
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    // Calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // Regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // Dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // Increases the duration in half for dotted notes
    }

    // Only play the note for 90% of the duration, leaving remaining 10% as a pause
    buzzer.tone(melody[thisNote], noteDuration * 0.9);

    // Wait for the specified duration before playing the next note.
    delay(noteDuration);

    // Stop the waveform generation before the next note.
    buzzer.noTone();
  }
}

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

// Red [0], Green [1], Blue [2]
int detectColour() {
  
  // Shine Red, read LDR after some delay
  shineRed();
  delay(RGBWait);
  colourArray[0] = getAvgReading(5);
  
  // Shine Green, read LDR after some delay
  shineGreen();
  delay(RGBWait);
  colourArray[1] = getAvgReading(5);
  
  // Shine Blue, read LDR after some delay  
  shineBlue();
  delay(RGBWait);
  colourArray[2] = getAvgReading(5);

  //Turn the IR back on
  analogWrite(A,LOW); 
  analogWrite(B,LOW);

  // Red Array values
  colourArray[0] = (colourArray[0] - blackArray[0])/(greyDiff[0])*255;
  Serial.println(int(colourArray[0]));
  
  // Green Array values
  colourArray[1] = (colourArray[1] - blackArray[1])/(greyDiff[1])*255;
  Serial.println(int(colourArray[1]));
  
  // Blue Array values
  colourArray[2] = (colourArray[2] - blackArray[2])/(greyDiff[2])*255;
  Serial.println(int(colourArray[2]));


  // If R, G and B near 240 = ⚪
  if (colourArray[0] >= 240 && colourArray[1] >= 240 && colourArray[2] >= 240) {
    led.setColor(255, 255, 255); // set both LEDs to ⚪
    led.show();
    return detectWhite;
  }
  // When 🟢 is MAX
  if (colourArray[1] > colourArray[0] && colourArray[1] > colourArray[2]) {
        if(colourArray[2] <= 230) {
          led.setColor(0, 255, 0); // set both LEDs to 🟢
          led.show();
          return detectGreen; 
        }else if(colourArray[0] >= 180) {
        led.setColor(128, 0, 128); // set both LEDs to 🟣
        led.show();
        return detectPurple;
    }else {
        led.setColor(0, 0, 255); // set both LEDs to 🔵
        led.show();
      return detectBlue;
    }
    //}
  }
  // When 🔴 is MAX
  else if (colourArray[0] > colourArray[1] && colourArray[0] > colourArray[2]) {
      if (colourArray[1] <= 120) {
        led.setColor(255, 0, 0); // set both LEDs to 🔴
        led.show();
        return detectRed;
      } 
      else {
        led.setColor(255, 165, 0); // set both LEDs to 🟠
        led.show();
        return detectOrange;
      }
  }else if(colourArray[2] > colourArray[0] && colourArray[2] > colourArray[1]) {
    if(colourArray[0] >= 180) {
        led.setColor(128, 0, 128); // set both LEDs to 🟣
        led.show(); 
      return detectPurple;
    }else {
        led.setColor(0, 0, 255); // set both LEDs to 🔵
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


}

void loop(){

  if (analogRead(A7) < 100) { // If push button is pushed, the value will be very low
    status = 1 - status; // Toggle status
  }

  if(status == 1) {
    double distance_right = ultrasonic_dist();
    float distance_left = shineIR(); 
    float dx = distance_right - 8;
    moveForward();

    if (distance_right < 9 && distance_right != -1) { // Left
      leftMotor.run(-255 - dx*15); // Negative: wheel turns anti-clockwise
      rightMotor.run(255); // Positive: wheel turns clockwise
      delay(50);
    }
    else if(distance_left < 9) { // Right
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
          while(1) {
            celebrate(); // play "Never Gonna Give You Up"
          }
          break;
      };
    }
  }
}
