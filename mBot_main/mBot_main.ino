#include <MeMCore.h>
#include "definitons.h"

void setup() {

  led.setpin(13);

  // Setting LED_Array
  // Red [0], Green [1], Blue [2]
  LED_Array[0] = {255, 255};
  LED_Array[1] = {255, 0};
  LED_Array[2] = {0, 255};

  // Configure pinMode for A0, A1, A2, A3  
  // Setting A0(A) and A1(B) to output to control the LEDs + IR Emitter
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);

  //Setting A2 and A3 to input to recieve input from IR Detector + LDR
  pinMode(IRD, INPUT); // A1 recieves input from IR Detector
  pinMode(LDR, INPUT); // A2 recieves input from LDR
  
  pinMode(ULTRASONIC, OUTPUT);
  pinMode(A7, INPUT); // Setup A7 as input for the push button
}


/** Start of mBot Movement functions**/

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


/** Start of LDR Colour Calibration**/

int getAvgReading(int times) {
  // Find the average reading for the requested number of times of scanning LDR
  int reading;
  int total = 0;
  // Take the reading as many times as requested and add them up
  for (int i = 0; i < times; i++){
    reading = analogRead(LDR);
    total = reading + total;
    delay(LDRWait);
  }
  // Calculate the average and return it
  return total/times;
}

void setBalance() {
  // Set white balance
  Serial.println("Put White Sample For Calibration ...");
  delay(5000); // 5 second delay to get sample ready

  // Scan the White sample
  // Go through Red, Green and Blue colour one at a time
  // Set the Maximum reading for each colour into the White array
  for (int i = 0; i < 3; i++) {
    digitalWrite(A,LED_Array[i].A_val); // Setting A0 to High/Low
    digitalWrite(B,LED_Array[i].B_val); // Setting A1 to High/Low
    delay(RGBWait);
    
    whiteArray[i] = getAvgReading(5); // Get average of 5 readings
    Serial.print("White Array ");
    Serial.print(i);
    Serial.print(" Value: ");
    Serial.println(whiteArray[i]);
  }

  Serial.println("Put Black Sample For Calibration ...");
  delay(5000); // 5 second delay to get sample ready
  
  // Scan the Black sample
  // Go through Red, Green and Blue colour one at a time
  // Set the Minimum reading for each colour into the Black array
  for (int i = 0; i < 3; i++) {
    digitalWrite(A,LED_Array[i].A_val); // Setting A0 to High/Low
    digitalWrite(B,LED_Array[i].B_val); // Setting A1 to High/Low
    delay(RGBWait);

    blackArray[i] = getAvgReading(5); // Get average of 5 readings
    Serial.print("Black Array ");
    Serial.print(i);
    Serial.print(" Value: ");
    Serial.println(blackArray[i]);

    // Range is derived from difference between Maximum and Minimum values
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

  // If R, G and B values above 240 = ⚪
  if (colourArray[0] >= 240 && colourArray[1] >= 240 && colourArray[2] >= 240) {
    led.setColor(255, 255, 255); // Set both LEDs to ⚪
    led.show();
    return detectWhite;
  }
  
  // When Green value is MAX
  if (colourArray[1] > colourArray[0] && colourArray[1] > colourArray[2]) {
        if(colourArray[2] <= 230) { // Compare Blue values
          led.setColor(0, 255, 0); // set both LEDs to 🟢
          led.show();
          return detectGreen; 
        } 
        else if (colourArray[0] >= 180) { // Compare Red values
        led.setColor(128, 0, 128); // set both LEDs to 🟣
        led.show();
        return detectPurple;
    } else {
        led.setColor(0, 0, 255); // set both LEDs to 🔵
        led.show();
      return detectBlue;
    }
  }
    
  // When Red value is MAX
  else if (colourArray[0] > colourArray[1] && colourArray[0] > colourArray[2]) {
      if (colourArray[1] <= 120) { // Compare Green values
        led.setColor(255, 0, 0); // set both LEDs to 🔴
        led.show();
        return detectRed;
      } 
      else {
        led.setColor(255, 165, 0); // set both LEDs to 🟠
        led.show();
        return detectOrange;
      }
  }
    
  // When Blue value is MAX
  else if (colourArray[2] > colourArray[0] && colourArray[2] > colourArray[1]) {
    if (colourArray[0] >= 180) {
        led.setColor(128, 0, 128); // set both LEDs to 🟣
        led.show(); 
      return detectPurple;
    }
    else {
        led.setColor(0, 0, 255); // set both LEDs to 🔵
        led.show();
      return detectBlue;
    }
  }
}

/** End of LDR Colour Detection**/


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

int shineIR() {
  // Power on IR Emitter
  analogWrite(A,LOW); // Setting A0 to High/Low
  analogWrite(B,LOW); // Setting A1 to High/Low

  delay(500);

  int ans = analogRead(IRD);

  return ans;
}


void loop() {
  
  if (analogRead(A7) < 100) { // If push button is pushed, the value will be very low
    status = 1 - status; // Toggle status
  }

  if (status == 1) {
    double distance_right = ultrasonic_dist();
    int distance_left = shineIR(); 
    float dx = distance_right - 8;
    moveForward();

    // Too close to Right wall, Veers Left (Dynamic)
    if (distance_right < 9 && distance_right != -1) { 
      leftMotor.run(-255 - dx*15); // Negative: Left wheel revolves forwards
      rightMotor.run(255); // Positive: Right wheel revolved backwards
      delay(50);
    }
    // Too close to Left wall, Veers Right (Dynamic)
    else if(distance_left < 9) { 
      leftMotor.run(-255); // Negative: Left wheel revolves forwards
      rightMotor.run(255 - dx*15); // Positive: Right wheel revolved backwards
      delay(50);
    }

    int sensorState = lineFollower.readSensors(); // Reads the line sensor's state
    if (sensorState != S1_OUT_S2_OUT) { // Runs colour sensor when mbot stops at blackline
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
          uTurn(distance_right); // Turn direction depends on distance_right
          break;
        case detectPurple:
          successiveLeft();
          break;
        case detectBlue:
          successiveRight();
          break;
        case detectWhite:
          leftMotor.stop(); // Left wheel Stop
          rightMotor.stop(); // Right wheel stop
          while (1) {
            celebrate(); // play "Never Gonna Give You Up"
          }
          break;
      };
    }
  }
}
