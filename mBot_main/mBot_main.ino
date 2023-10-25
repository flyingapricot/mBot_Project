#include <MeMCore.h>

#define ULTRASONIC 12
#define ULTRASONIC_TIMEOUT 2000 // Max microseconds to wait; choose according to max distance of wall
#define SPEED_OF_SOUND 340 // Update according to your own experiment
int status = 0; // global status; 0 = do nothing, 1 = mBot runs



// void celebrate() {// Code for playing celebratory tune}
// void stopMotor() {// Code for stopping motor}
// void moveForward() {// Code for moving forward for some short interval}
// void turnRight() {// Code for turning right 90 deg}
// void turnLeft() {// Code for turning left 90 deg}
// void uTurn() {// Code for u-turn}
// void doubleLeftTurn() {// Code for double left turn}
// void doubleRightTurn() {// Code for double right turn}
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

}
void loop()
{
  if (analogRead(A7) < 100) { // If push button is pushed, the value will be very low
    status = 1 - status; // Toggle status
    delay(500); // Delay 500ms so that a button push won't be counted multiple times.
  }
// Read ultrasonic sensing distance (choose an appropriate timeout)
// Read IR sensing distance (turn off IR, read IR detector, turn on IR,
//read IR detector, estimate distance)
// if within black line, stop motor, detect colour, and take corresponding
//action
// else if too near to left wall, nudge right
// else if too near to right wall, nudge left
// else move forward
}
