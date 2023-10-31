#include <MeMCore.h>

#define ULTRASONIC 12
#define ULTRASONIC_TIMEOUT 2000 // Max microseconds to wait; choose according to max distance of wall
#define SPEED_OF_SOUND 340 // Update according to your own experiment
MeDCMotor leftMotor(M1); // assigning leftMotor to port M1
MeDCMotor rightMotor(M2); // assigning RightMotor to port M2
MeLineFollower lineFollower(PORT_2); // assigning lineFollower to RJ25 port 2

int status = 0; // global status; 0 = do nothing, 1 = mBot runs,
bool do_color_decode = false;

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
  delay(330);
  leftMotor.stop();
  rightMotor.stop();

}

// 🔴 Called when RED detected at waypoint.
// Turns mBot to the left by 90 degrees (on the spot).
// TODO : Higher Delay (1500?)
void turnLeft() {
  leftMotor.run(255); // Positive: wheel turns anti-clockwise
  rightMotor.run(255); // Positive: wheel turns anti-clockwise
  delay(300);
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
  delay(550);  // TODO: EDIT VALUE BASED ON LAB VALUES.

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
  delay(550);  // TODO: EDIT VALUE BASED ON LAB VALUES.

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
>>>>>>> Motor
// void nudgeLeft() {// Code for nudging slightly to the left for some short
// interval}
// void nudgeRight() {// Code for nudging slightly to the right for some
// short interval}
// void shineIR() {// Code for turning on the IR emitter only}
// void shineRed() {// Code for turning on the red LED only}
// void shineGreen() {// Code for turning on the green LED only}
// void shineBlue() {// Code for turning on the blue LED only}

int detectColour()
{
// Shine Red, read LDR after some delay
// Shine Green, read LDR after some delay
// Shine Blue, read LDR after some delay
// Run algorithm for colour decoding
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
  
  pinMode(ULTRASONIC, OUTPUT);
  pinMode(A7, INPUT); // Setup A7 as input for the push button

  //Serial.begin(9600);

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
    //}

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
    }

    int sensorState = lineFollower.readSensors(); // read the line sensor's state
    if (sensorState != S1_OUT_S2_OUT) { // run colour sensor when mbot stops at blackline
      leftMotor.stop(); // Left wheel Stop
      rightMotor.stop(); // Right wheel stop
      delay(500);

      //Do turning right now
      uTurn();
      // leftMotor.stop(); // Left wheel Stop
      // rightMotor.stop(); // Right wheel stop
      // delay(500);



    }

  }

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
