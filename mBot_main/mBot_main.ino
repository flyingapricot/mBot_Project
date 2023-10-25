#include <MeMCore.h>

#define ULTRASONIC 12
#define ULTRASONIC_TIMEOUT 2000 // Max microseconds to wait; choose according to max distance of wall
#define SPEED_OF_SOUND 340 // Update according to your own experiment
int status = 0; // global status; 0 = do nothing, 1 = mBot runs

MeDCMotor leftMotor(M1); // assigning leftMotor to port M1
MeDCMotor rightMotor(M2); // assigning RightMotor to port M2

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

  // Run algorithm for colour decoding
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

    //the difference between the maximum and the minimum gives the range
    greyDiff[i] = whiteArray[i] - blackArray[i];
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
