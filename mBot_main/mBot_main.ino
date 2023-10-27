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

// black
void stopRobot() {
  leftMotor.stop();
  rightMotor.stop();
}

// white
// void moveForward() {// Code for moving forward for some short interval}

// green - turning right 90 deg
// void turnRight() {
// spin left motor forwards and right motor backwards for t
// stop both left and right motor for stabilising b4 next action 
  leftMotor.stop();
  rightMotor.stop();
//}

// red - turning left 90 deg
// void turnLeft() {
// spin left motor backwards and right motor forwards for t
// stop both left and right motor for stabilising b4 next action 
  leftMotor.stop();
  rightMotor.stop();
// }

// orange - uturn
// void uTurn() {
// spin left motor backwards and right motor forwards for 2t
// stop both left and right motor for stabilising b4 next action 
  leftMotor.stop();
  rightMotor.stop();
// }

// purple - double left turn
// void doubleLeftTurn() {
// spin left motor backwards and right motor forwards for t
// stop both left and right motor for stabilising b4 next action
  leftMotor.stop();
  rightMotor.stop();
// both motor moves forward for t
// spin left motor backwards and right motor forwards for t
// stop both left and right motor for stabilising b4 next action
  leftMotor.stop();
  rightMotor.stop();
//}

// blue
// void doubleRightTurn() {
// spin left motor backwards and right motor forwards for t
// stop both left and right motor for stabilising b4 next action
  leftMotor.stop();
  rightMotor.stop();
// both motor moves forward for t
// spin left motor backwards and right motor forwards for t
// stop both left and right motor for stabilising b4 next action 
  leftMotor.stop();
  rightMotor.stop();
}


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

  Serial.begin(9600);

}


void loop()
{
  if (analogRead(A7) < 100) { // If push button is pushed, the value will be very low
    status = 1 - status; // Toggle status
    //delay(500); // Delay 500ms so that a button push won't be counted multiple times.
  }

  if(status == 1) {
    int sensorState = lineFollower.readSensors(); // read the line sensor's state
    if (sensorState == S1_IN_S2_IN || sensorState == S1_IN_S2_OUT || sensorState == S1_OUT_S2_IN) //when black line is reached
    {
      stopRobot();
      //Serial.println("DETECTED");
      do_color_decode = true;
      delay(50);
    }else if(!do_color_decode) {
      double dist_from_right = ultrasonic_dist();
      if(dist_from_right > 15 || dist_from_right == -1) {
        leftMotor.run(-255); // Left wheel goes forward (anti-clockwise)
        rightMotor.run(255); // Right wheel goes forward (clockwise)
        return;
      }else if(dist_from_right < 11) {
        leftMotor.run(-190); // Left wheel stops
        rightMotor.run(255); // Right wheel go forward
        delay(250);
      }else if(dist_from_right > 12) {
        leftMotor.run(-255); // Left wheel go forward
        rightMotor.run(190); // Right wheel stops
        delay(250);
      }else {
        leftMotor.run(-255); // Left wheel goes forward (anti-clockwise)
        rightMotor.run(255); // Right wheel goes forward (clockwise)
      }

    }
  }


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
