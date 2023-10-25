#include <MeMCore.h>
void celebrate() {// Code for playing celebratory tune}
void stopMotor() {// Code for stopping motor}
void moveForward() {// Code for moving forward for some short interval}
void turnRight() {// Code for turning right 90 deg}
void turnLeft() {// Code for turning left 90 deg}
void uTurn() {// Code for u-turn}
void doubleLeftTurn() {// Code for double left turn}
void doubleRightTurn() {// Code for double right turn}
void nudgeLeft() {// Code for nudging slightly to the left for some short
interval}
void nudgeRight() {// Code for nudging slightly to the right for some
short interval}
void shineIR() {// Code for turning on the IR emitter only}
void shineRed() {// Code for turning on the red LED only}
void shineGreen() {// Code for turning on the green LED only}
void shineBlue() {// Code for turning on the blue LED only}
int detectColour()
{
// Shine Red, read LDR after some delay
// Shine Green, read LDR after some delay
// Shine Blue, read LDR after some delay
// Run algorithm for colour decoding
}
void setup()
{
// Configure pinMode for A0, A1, A2, A3
}
void loop()
{
// Read ultrasonic sensing distance (choose an appropriate timeout)
// Read IR sensing distance (turn off IR, read IR detector, turn on IR,
read IR detector, estimate distance)
// if within black line, stop motor, detect colour, and take corresponding
action
// else if too near to left wall, nudge right
// else if too near to right wall, nudge left
// else move forward
}
