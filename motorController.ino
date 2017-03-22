#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


//this is how many times a second the motors update
//a higher value will make movements smoother
const int updateRate = 20;
const int motorLowerBounds = 150;
const int motorUpperBounds = 600;
const int motorBoundRange = motorUpperBounds - motorLowerBounds;

int lastPos[16];

Adafruit_PWMServoDriver servoControl = Adafruit_PWMServoDriver();

void setup() {
  Serial.begin(9600);
  // initialize servos
  servoControl.begin();

  //set servo communication rates
  servoControl.setPWMFreq(60);

  //put all servos in start pos
  servoControl.setPWM(0, 0, 300);
  lastPos[0] = 300;
  servoControl.setPWM(1, 0, 500);
  lastPos[1] = 500;

  delay(3000);
}

void loop() {

  int waistSideMotors[2] = {0, 1};
  int pos1[2] = {400, 600};
  int pos2[2] = {200, 350};
  moveMotor(waistSideMotors, pos1, 1, 2);
  delay(2000);
  moveMotor(waistSideMotors, pos2, 1, 2);
  delay(2000);
  
}

//this function will move all given motors to their given positions in such a way that they arrive at the same time.
//accepts an array of servos id's, an array of the desired end positions, the time in milliseconds that it should take to get there, and the number of motors it will move
void moveMotor(int servos[], int pos[], float dur, int numMotors){
  int num = numMotors;
  int moveRange[num];
  int movePerUpdate[num];
  int reps = dur * updateRate;

  for(int i=0;i<num;i++){
    //startPos[i] = servos[i].read();  //defunct with special servo library
    moveRange[i] = pos[i]-lastPos[servos[i]];
    movePerUpdate[i] = moveRange[i] / reps;
  }


  //move motors with computed values
  for(int i=0;i<reps;i++){
    for(int j=0;j<num;j++){
       servoControl.setPWM(servos[j], 0, movePerUpdate[j]+lastPos[j]); //second arguement should always be zero
       lastPos[servos[j]] += movePerUpdate[j];
    }
    delay(1000/updateRate);
  }
}

