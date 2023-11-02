#include <MeMCore.h>

#define ULTRASONIC 12
#define ULTRASONIC_TIMEOUT 2000 // Max microseconds to wait; choose according to max distance of wall
#define SPEED_OF_SOUND 340 // Update according to your own experiment

MeDCMotor leftMotor(M1); // assigning leftMotor to port M1
MeDCMotor rightMotor(M2); // assigning RightMotor to port M2
MeLineFollower lineFollower(PORT_2); // assigning lineFollower to RJ25 port 2

int status = 0; // global status; 0 = do nothing, 1 = mBot runs

//High Low Pair used to specifically control each R, G and B LED by varying A and B's Analog Signal
struct LEDPair {
  uint8_t A_val;
  uint8_t B_val;
};

/** Start of LDR Related Definitions**/

// Define time delay before the next RGB colour turns ON to allow LDR to stabilize
#define RGBWait 200 //in milliseconds
// Define time delay before taking another LDR reading
#define LDRWait 10 //in milliseconds
#define LDR A2 //Using Port 3 S1 to read LDR Voltage
#define IRD A3 //Using Port 3 S2 to read IR Detector Voltage


//By varying the analog output of A and B (At Port 4), we can control each of the LEDs + IR Emitter
#define A A0 //S1 (Pin 2 of Decoder)
#define B A1 //S2 (Pin 3 of Decoder)


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


bool do_color_decode = false;

enum Colours {
  detectPurple,
  detectGreen,
  detectBlue,
  detectRed,
  detectOrange,
  detectWhite
};

// void celebrate() {// Code for playing celebratory tune}


void stopRobot() {
  leftMotor.stop();
  rightMotor.stop();
}

// 🟢 Called when GREEN detected at waypoint.
//Turns mBot to the right by 90 degrees (on the spot).
// TODO : Higher Delay (1500?)
void turnRight() {
  leftMotor.run(-255); // Positive: wheel turns anti-clockwise
  rightMotor.run(-255); // Positive: wheel turns anti-clockwise
  delay(310);
  leftMotor.stop();
  rightMotor.stop();

}

// 🔴 Called when RED detected at waypoint.
// Turns mBot to the left by 90 degrees (on the spot).
// TODO : Higher Delay (1500?)
void turnLeft() {
  leftMotor.run(255); // Positive: wheel turns anti-clockwise
  rightMotor.run(255); // Positive: wheel turns anti-clockwise
  delay(310);
  leftMotor.stop();
  rightMotor.stop();
}


// 🟣 Called when PURPLE detected at waypoint.
//Turns the mBot successively to the left twice
//TODO: Higher delay at 2000ms while moving straight?
void successiveLeft() {
  // 1. Turn mBot to the left by 90 degrees.
  turnLeft();

  // 2. Then, move mBot forward by one tile.
  leftMotor.run(-255);
  rightMotor.run(255);
  delay(700);  // TODO: EDIT VALUE BASED ON LAB VALUES. PREV VAL: 600

  // 3. After moving forward, stop motors and wait for a short duration for mBot to stabilise.
  leftMotor.stop();
  rightMotor.stop();
  delay(20);

  // 4. Finally, turn mBot to the left by 90 degrees again.
  turnLeft();
}

// 🔵 Called when BLUE detected at waypoint.
//Turns the mBot successively to the left twice
//TODO: Higher delay at 2000ms while moving straight?
void successiveRight() {
  // 1. Turn mBot to the right by 90 degrees.
  turnRight();

  // 2. Then, move mBot forward by one tile.
  leftMotor.run(-255);
  rightMotor.run(255);
  delay(700);  // TODO: EDIT VALUE BASED ON LAB VALUES. (Last: 600)

  // 3. After moving forward, stop motors and wait for a short duration for mBot to stabilise.
  leftMotor.stop();
  rightMotor.stop();
  delay(20);

  // 4. Finally, turn mBot to the right by 90 degrees again.
  turnRight();

}

// 🟠 Called when ORANGE detected at waypoint when closer to left wall.
//TODO: Higher Single Delay after UTurn? (3000ms)
void uTurn() {
  // Turn mBot 180 degrees by spinning to the left.
  // Spin left motor backward and right motor forward to turn mBot to the left.

  // leftMotor.run(100);
  // rightMotor.run(0);
  // delay(200);

  leftMotor.run(255);
  rightMotor.run(255);
  delay(575);  // Keep turning until turn is 180 degrees.

  // After turn, stop motors and wait for a short duration for mBot to stabilise.
  leftMotor.stop();
  rightMotor.stop();
  delay(300);

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

void shineRed() {    
  analogWrite(A,LED_Array[2].A_val); //Setting A0 to High/Low
  analogWrite(B,LED_Array[2].B_val); //Setting A0 to High/Low
}

void shineGreen() {
  analogWrite(A,LED_Array[0].A_val); //Setting A0 to High/Low
  analogWrite(B,LED_Array[0].B_val); //Setting A0 to High/Low
}

void shineBlue() {
  analogWrite(A,LED_Array[1].A_val); //Setting A0 to High/Low
  analogWrite(B,LED_Array[1].B_val); //Setting A0 to High/Low
}

int detectColour()
{

  //0 - Green, 1 - Blue, 2 - Red

  // Shine Red, read LDR after some delay
  shineRed();
  delay(RGBWait);
  colourArray[2] = getAvgReading(5);
  colourArray[2] = (colourArray[2] - blackArray[2])/(greyDiff[2])*255;
  delay(RGBWait);
  Serial.println(int(colourArray[2]));

  // Shine Green, read LDR after some delay
  shineGreen();
  delay(RGBWait);
  colourArray[0] = getAvgReading(5);
  colourArray[0] = (colourArray[0] - blackArray[0])/(greyDiff[0])*255;
  delay(RGBWait);
  Serial.println(int(colourArray[0]));

  // Shine Blue, read LDR after some delay
  shineBlue();
  delay(RGBWait);
  colourArray[1] = getAvgReading(5);
  colourArray[1] = (colourArray[1] - blackArray[1])/(greyDiff[1])*255;
  delay(RGBWait);
  Serial.println(int(colourArray[1]));
  analogWrite(A,LOW); //Setting A0 to High/Low
  analogWrite(B,LOW); //Setting A0 to High/Low

  //New Algo
  //0 - Green, 1 - Blue, 2 - Red
  // double gr = fabs(colourArray[0]/colourArray[2]); //Green/Red
  // double rb = fabs(colourArray[2]/colourArray[1]); //Red/Blue
  // double bg = fabs(colourArray[1]/colourArray[0]); //Blue/Green

  // if( (gr >= 0.6 && gr <=1) && (rb >= 1.95 && rb <= 2.35) && (bg >=0.35 && bg <= 0.75) ) {
  //   Serial.println("ORANGE DETECTED");
  //   return detectOrange;
  // }

  // if( (gr >= 1.05 && gr <=1.45) && (rb >= 0.55 && rb <= 0.95) && (bg >=0.8 && bg <= 1.2) ) {
  //   Serial.println("PURPLE DETECTED");
  //   return detectPurple;
  // }

  // if( (gr >= 3.8 && gr <=4.2) && (rb >= 0.2 && rb <= 0.6) && (bg >=0.35 && bg <= 0.75) ) {
  //   Serial.println("GREEN DETECTED");
  //   return detectGreen;
  // }
   
  // if( (gr >= 2.7 && gr <=3.1) && (rb >= 0.15 && rb <= 0.55) && (bg >=0.8 && bg <= 1.2) ) {
  //   Serial.println("BLUE DETECTED");
  //   return detectBlue;
  // }

  // if( (gr >= 0.5 && gr <=0.7) && (rb >= 1.8 && rb <= 2.0) && (bg >=0.7 && bg <= 0.9) ) {
  //   Serial.println("RED DETECTED");
  //   return detectRed;
  // }

  // Serial.println("FAILEDD!");
  // Run algorithm for colour decoding
  //🟢 GREEN Detected
  //0 - Green, 1 - Blue, 2 - Red
  // if (colourArray[0] > colourArray[1] && 
  // colourArray[0] > colourArray[2] && colourArray[1] > colourArray[2]) {
  //   if(colourArray[1]/colourArray[0] < 0.7) {
  //     Serial.println("ITS GREEN!!!");
  //     return detectGreen;
  //   }
  // }

  // //🔵 BLUE /🟣 PURPLE Detected 
  // //0 - Green, 1 - Blue, 2 - Red
  // if (colourArray[0] > colourArray[2] && colourArray[1] > colourArray[2] ) {
  //   if(colourArray[2]/colourArray[0] > 0.5) {
  //     Serial.println("ITS PURPLE!!!");
  //     return detectPurple;
  //   }
  //   Serial.println("ITS BLUE!!!");
  //   return detectBlue;
  // }

  // //🔵 ORANGE /🟣 RED Detected 
  // //0 - Green, 1 - Blue, 2 - Red
  // if (colourArray[2] > colourArray[0] && colourArray[2] > colourArray[1]) {
  //   if(colourArray[0]/colourArray[2] < 0.7) {
  //     Serial.println("ITS RED!!!");
  //     return detectRed;
  //   }
  //   Serial.println("ITS ORANGE!!!");
  //   return detectOrange;
  // }


  

  //0 - Green, 1 - Blue, 2 - Red
  // if(colourArray[1] > colourArray[2] && colourArray[1] > colourArray[0]) {
  //   //🔵 is the highest -> This is 🟣
  //   Serial.println("ITS PURPLE!!!");
  //   return detectPurple;
  if(colourArray[0] >= 180 && colourArray[1] >= 180 && colourArray[2] >= 180) {
    Serial.println("ITS WHITE!!!");
    return detectWhite;
  }
  /*}*/if(colourArray[0] > colourArray[2] && colourArray[0] > colourArray[1]) {
    double ratio = colourArray[0]/colourArray[1]; //Green/Blue ratio
    if(colourArray[2] >= 130) {Serial.println("ITS PURPLE!!!");return detectPurple;}
    else if(colourArray[2] >= 95) {
      Serial.println("ITS BLUE!!!");
      return detectBlue;
    }else {
      Serial.println("ITS GREEN!!!");
      return detectGreen;
    }
  }else if(colourArray[2] > colourArray[0] && colourArray[2] > colourArray[1]) {
      double ratio = colourArray[0]/colourArray[2]; //green/red ratio
      if(ratio < 0.9) {
        Serial.println("ITS RED!!!");
        return detectRed;
      }else {
        Serial.println("ITS ORANGE!!!");
        return detectOrange;
      }
  }
}

int getAvgReading(int times){
  //find the average reading for the requested number of times of scanning LDR
  int reading;
  int total =0;
  //take the reading as many times as requested and add them up
  for(int i = 0;i < times;i++){
    reading = analogRead(LDR);
    total = reading + total;
    delay(LDRWait);
  }
  //calculate the average and return it
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
    //Serial.print("distance(cm) = ");
    //Serial.println(dist);
  }
  else {
    dist = -1;
    //Serial.println("out of range");
  }

  //delay(500);
  return dist;
}

void setup()
{
  // Configure pinMode for A0, A1, A2, A3
  //Setting LED_Array
  //0 - Green, 1 - Blue, 2 - Red
  LED_Array[0] = {255, 0};
  LED_Array[1] = {0, 255};
  LED_Array[2] = {255, 255};
  
  //Setting A0(A) and A1(B) to output to control the LEDs + IR Emitter
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);

  //Setting A2 and A3 to input to recieve input from IR Detector + LDR
  pinMode(IRD, INPUT); //A1 recieves input from IR Detector
  pinMode(LDR, INPUT); //A2 recieves input from LDR

  
  pinMode(ULTRASONIC, OUTPUT);
  pinMode(A7, INPUT); // Setup A7 as input for the push button

  Serial.begin(9600);
  whiteArray[0] = 993;
  whiteArray[1] = 995;
  whiteArray[2] = 991;

  blackArray[0] = 934;
  blackArray[1] = 926;
  blackArray[2] = 954;

  greyDiff[0] = 59;
  greyDiff[1] = 69;
  greyDiff[2] = 37;

  // whiteArray[0] = 1000;
  // whiteArray[1] = 1000;
  // whiteArray[2] = 1000;

  // blackArray[0] = 950;
  // blackArray[1] = 950;
  // blackArray[2] = 950;

  // greyDiff[0] = 50;
  // greyDiff[1] = 50;
  // greyDiff[2] = 50;


  //setBalance();
}


void loop()
{
  if (analogRead(A7) < 100) { // If push button is pushed, the value will be very low
    status = 1 - status; // Toggle status
    //delay(500); // Delay 500ms so that a button push won't be counted multiple times.
  }

  //detectColour();
  // shineRed();
  //delay(1000);
  // shineGreen();
  // delay(1000);

  // shineBlue();
  // delay(1000);

  //delay(2000);

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

    // if(distance_right < 7.0 && distance_right > 0.1) {
    //   if(distance_right < 7.0 && distance_right > 0.1) {
    //     leftMotor.run(-140);
    //     rightMotor.run(255);
    //     delay(15);
    //   }else if(distance_right < 7.0) {
    //     leftMotor.run(-160);
    //     rightMotor.run(255);
    //     delay(15);
    //   }else if(distance_right < 7.5) {
    //     leftMotor.run(-190);
    //     rightMotor.run(255);
    //     delay(15);
    //   }else {
    //     leftMotor.run(-215);
    //     rightMotor.run(255);
    //     delay(15);
    //   }
    //}

    int sensorState = lineFollower.readSensors(); // read the line sensor's state
    if (sensorState != S1_OUT_S2_OUT) { // run colour sensor when mbot stops at blackline
      leftMotor.stop(); // Left wheel Stop
      rightMotor.stop(); // Right wheel stop
      delay(500);

      enum Colours colour = detectColour();
      switch(colour) {
        case detectPurple:
          Serial.println("PURPLE DETECTED!!!!");
          successiveLeft();
          break;
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
        case detectBlue:
          Serial.println("BLUE DETECTED!!!!");
          successiveRight();
          break;
        case detectWhite:
          leftMotor.stop();
          rightMotor.stop();
          delay(5000);
          break;
      };
    }
  }

  //     // delay(2000);
  //     // //Do turning right now
  //     // uTurn();
      
  //     // leftMotor.stop(); // Left wheel Stop
  //     // rightMotor.stop(); // Right wheel stop
  //     // delay(500);



  //   }

  // }

  // if(status == 1) {
  //   int sensorState = lineFollower.readSensors(); // read the line sensor's state
  //   if (sensorState != S1_OUT_S2_OUT) //when black line is reached
  //   {
  //     stopRobot();
  //     //do_color_decode = true;
  //     delay(50);
  //   }else if(!do_color_decode) {
  //     double right_dist = ultrasonic_dist() - 11;
  //     if(right_dist+11 == 0) {
  //       leftMotor.run(-255); // Left wheel goes forward (anti-clockwise)
  //       rightMotor.run(255); // Right wheel goes forward (clockwise)
  //     }else if(right_dist >= 0) {
  //       leftMotor.run(-255); // Left wheel stops
  //       rightMotor.run(255 + (right_dist)*15 - 8); // Right wheel go forward
  //     }else {
  //       leftMotor.run(-255 + (right_dist)*15 ); // Left wheel go forward
  //       rightMotor.run(255-8); // Right wheel stops
  //     }

  //   }
  // }


  // if(!do_color_decode && status == 1) {
  // double dist_from_left = ultrasonic_dist();
  // //Serial.println(dist_from_left);
  // if(dist_from_left < 11) {
  //   //Too close to right, move left
  //     leftMotor.run(-190); // Left wheel stops
  //     rightMotor.run(255); // Right wheel go forward
  //     delay(250);
  //     //dist_from_left = ultrasonic_dist();
  //   //}
  // }else if(dist_from_left > 12) {
  //   //To close to left, move to right
  //   //while(dist_from_left > 10) {
  //     leftMotor.run(-255); // Left wheel go forward
  //     rightMotor.run(190); // Right wheel stops
  //     delay(250);
  //     //dist_from_left = ultrasonic_dist();
  //   //}
  // }else {
  //   leftMotor.run(-255); // Left wheel goes forward (anti-clockwise)
  //   rightMotor.run(255); // Right wheel goes forward (clockwise)
    
  // }


// Read ultrasonic sensing distance (choose an appropriate timeout)
// Read IR sensing distance (turn off IR, read IR detector, turn on IR,
//read IR detector, estimate distance)
// if within black line, stop motor, detect colour, and take corresponding
//action
// else if too near to left wall, nudge right
// else if too near to right wall, nudge left
// else move forward
}
