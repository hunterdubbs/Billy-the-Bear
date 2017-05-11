#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>


//this is how many times a second the motors update
//a higher value will make movements smoother
const int updateRate = 20;
const int motorLowerBounds = 150;
const int motorUpperBounds = 600;
const int motorBoundRange = motorUpperBounds - motorLowerBounds;

bool firstTime = true;

int soundLengths[8] = {33000, 13000, 13000, 8000, 9000, 10000, 13000, 10000};
int turn = 1;
int targetButton;
const int signalPin = A0;

int lastPos[16];
unsigned int soundLength = 5; //this should be set to the duration that billy is talking in seconds, should be changed as new lines are triggered
int nodReps = 3; //how many times billy should nod his head up and down
char input;

Adafruit_PWMServoDriver servoControl = Adafruit_PWMServoDriver();

void setup() {
  Serial.begin(9600);
  // initialize servos
  servoControl.begin();

  //set servo communication rates
  servoControl.setPWMFreq(60);

  //put all servos in start pos

  //waist
  servoControl.setPWM(0, 0, 320);
  lastPos[0] = 320;
  servoControl.setPWM(1, 0, 550);
  lastPos[1] = 550;
  //right arm
  servoControl.setPWM(4, 0, 200);
  servoControl.setPWM(5, 0, 600);
  servoControl.setPWM(6, 0, 400);
  lastPos[4] = 200;
  lastPos[5] = 600;
  lastPos[6] = 400;
  //left arm
  servoControl.setPWM(8, 0, 350);
  servoControl.setPWM(9, 0, 600);
  servoControl.setPWM(10, 0, 400);
  lastPos[8] = 350;
  lastPos[9] = 600;
  lastPos[10] = 400;
  //fluid power - neck
  servoControl.setPWM(12, 0, 280); //left right
  servoControl.setPWM(13, 0, 600); //front back
  lastPos[12] = 280;
  lastPos[13] = 600;
  //mouth - can kill small animals
  servoControl.setPWM(14, 0, 290);
  lastPos[14] = 290;
  
  /*
  //testing positions
  delay(500);
  int armWaistMotors[5] = {8, 9, 10, 0, 1};
  int pos3[5] = {200, 600, 400, 520, 580};
  int pos2[5] = {120, 400, 400, 320, 550};
  moveMotor(armWaistMotors, pos2, 0.5, 5);
  delay(200);
  moveMotor(armWaistMotors, pos3, 0.5, 5);*/

  delay(3000);
  input = 'Z';
}

void loop() {

  if (input != 'Z') {
    Serial.println(input);
  }


  if (input == 'B') {
    //turn waist
    int waistSideMotors[2] = {0, 1};
    int pos1[2] = {400, 600};
    int pos2[2] = {200, 350};
    moveMotor(waistSideMotors, pos1, 1, 2);
    delay(2000);
    moveMotor(waistSideMotors, pos2, 1, 2);
    delay(2000);
    input = 'Z';
  } else if (input == 'C') {
    //wave
    turn = 1;
    int mouthMotor[1] = {14};
    int openPos[1] = {200};
    int closePos[1] = {290};
    int armMotors[3] = {4, 5, 6};
    int RApos1[3] = {200, 600, 400};
    int RApos2[3] = {600, 600, 400};
    int RApos3[3] = {600, 450, 400};
    moveMotor(armMotors, RApos2, 1, 3);
    moveMotor(mouthMotor, openPos, 0.25, 1);
    moveMotor(mouthMotor, closePos, 0.25, 1);
    for (int i = 0; i < 3; i++) {
      moveMotor(armMotors, RApos3, 0.1, 3);
      moveMotor(mouthMotor, openPos, 0.25, 1);
      moveMotor(armMotors, RApos2, 0.1, 3);
      moveMotor(mouthMotor, closePos, 0.25, 1);
    }
    moveMotor(mouthMotor, openPos, 0.25, 1);
    moveMotor(armMotors, RApos1, 1, 3);
    moveMotor(mouthMotor, closePos, 0.25, 1);
    input = 'Z';
  } else if (input == 'D') {
    //do tic tac toe turn
    turn++;
    int armMotors[3] = {8, 9, 10};
    int armWaistMotors[5] = {8, 9, 10, 0, 1};
    //int pos1[3] = {200, 300, 600}; //paw to chin
    int pos2[5] = {120, 400, 400, 320, 550}; // side of honey pot
    int pos3[5];
    //this pos changes depending upon the targetButton var
    switch(targetButton){
      Serial.println("calc motors positions");
      case 1:
        fillArray(pos3, 50, 600, 400, 520, 500); //good
        break;

      case 2:
        fillArray(pos3, 50, 600, 400, 490, 500); //good
        break;

      case 3:
       fillArray(pos3, 50, 600, 400, 490, 450); //good
        break;

      case 4:
        fillArray(pos3, 50, 600, 400, 470, 500); //good
        break;

      case 5:
        fillArray(pos3, 50, 600, 400, 440, 500); //good
        break;

      case 6:
        fillArray(pos3, 50, 600, 400, 450, 450); //good
        break;

      case 7:
        fillArray(pos3, 200, 600, 400, 420, 580); //good
        break;

      case 8:
        fillArray(pos3, 50, 600, 400, 410, 500); //good
        break;

      case 9:
        fillArray(pos3, 50, 600, 400, 410, 450); //good
        break;
       
    }

    //returns to generic code
    
    int pos4[3] = {350, 600, 400}; //home pos
    //moveMotor(armMotors, pos1, 1, 3);
    //should say line here
    //delay(2000);
    int mouthMotor[1] = {14};
    int openPos[1] = {200};
    int closePos[1] = {290};
    moveMotor(mouthMotor, openPos, 0.25, 1);
    moveMotor(armWaistMotors, pos2, 0.5, 5);
    moveMotor(mouthMotor, closePos, 0.25, 1);
    moveMotor(armWaistMotors, pos3, 0.5, 5);
    moveMotor(mouthMotor, openPos, 0.25, 1);
    moveMotor(mouthMotor, closePos, 0.25, 1);
    moveMotor(armWaistMotors, pos2, 0.5, 5);
    moveMotor(mouthMotor, openPos, 0.25, 1);
    moveMotor(armMotors, pos4, 0.5, 3);
    moveMotor(mouthMotor, closePos, 0.25, 1);
    input = 'Z';
  } else if (input == 'S') {
    //moves the mouth
    if (firstTime) {
      soundLength = 25;
      firstTime = false;
      int mouthMotor[3] = {14, 13, 12};
      int openPos[3] = {200, 200, 100};
      delay(750);
      int closePos[3] = {290, 600, 280};
      moveMotor(mouthMotor, openPos, 0.5, 3);
      moveMotor(mouthMotor, closePos, 0.5, 3);
    } else {
      if (turn < 5) {
        soundLength = soundLengths[turn] / 1000 - 4000;
      } else {
        soundLength = 9;
      }
    }
    int mouthMotor[1] = {14};
    int openPos[1] = {200};
    int closePos[1] = {290};
    int speakTime = 0;
    while (speakTime < soundLength * 1000) {
      int pause = floor(random(0, 300));
      int pause2 = floor(random(0, 300));
      openPos[0] = floor(random(200, 245));
      moveMotor(mouthMotor, openPos, 0.25, 1);
      delay(pause);
      moveMotor(mouthMotor, closePos, 0.25, 1);
      delay(pause2);
      speakTime += (500 + pause + pause2);
    }
    input = 'Z';
  } else if (input == 'E') {
    //high-five
    int armMotors[4] = {4, 5, 6, 0};
    int pos1[4] = {550, 450, 400, 320};
    int pos2[4] = {550, 650, 430, 200};
    int pos3[4] = {200, 600, 400, 320};
    moveMotor(armMotors, pos1, 0.5, 4);
    moveMotor(armMotors, pos2, 0.25, 4);
    delay(1000);
    moveMotor(armMotors, pos1, 0.25, 4);
    moveMotor(armMotors, pos3, 0.5, 4);
    input = 'Z';
  } else if (input == 'F') {
    //nods head
    int head[1] = {13};
    int hpos1[1] = {600};
    int hpos2[1] = {200};
    for (int i = 0; i < nodReps; i++) {
      moveMotor(head, hpos2, 0.5, 1);
      delay(750);
      moveMotor(head, hpos1, 0.5, 1);
      delay(750);
      input = 'Z';
    }
  } else if (input == 'R') {
    //reset
    firstTime = true;
    turn = 1;
  } else if (input == 'Q') {
    turn++;
  } else if (input == 'T'){
    targetButton = 1;
  }else if (input == 'Y'){
    targetButton = 2;
  }else if (input == 'U'){
    targetButton = 3;
  }else if (input == 'I'){
    targetButton = 4;
  }else if (input == 'O'){
    targetButton = 5;
  }else if (input == 'P'){
    targetButton = 6;
  }else if (input == 'J'){
    targetButton = 7;
  }else if (input == 'K'){
    targetButton = 8;
  }else if (input == 'L'){
    targetButton = 9;
  }
  //input = 'Z';
}

//this function will move all given motors to their given positions in such a way that they arrive at the same time.
//accepts an array of servos id's, an array of the desired end positions, the time in seconds that it should take to get there, and the number of motors it will move
void moveMotor(int servos[], int pos[], float dur, int numMotors) {
  int num = numMotors;
  int moveRange[num];
  int movePerUpdate[num];
  int reps = dur * updateRate;

  for (int i = 0; i < num; i++) {
    //startPos[i] = servos[i].read();  //defunct with special servo library
    moveRange[i] = pos[i] - lastPos[servos[i]];
    movePerUpdate[i] = moveRange[i] / reps;
  }


  //move motors with computed values
  for (int i = 0; i < reps; i++) {
    for (int j = 0; j < num; j++) {
      servoControl.setPWM(servos[j], 0, movePerUpdate[j] + lastPos[servos[j]]); //second arguement should always be zero
      lastPos[servos[j]] += movePerUpdate[j];
    }
    delay(1000 / updateRate);
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    input = inChar;
  }
}

void fillArray(int targetArray[], int var1, int var2, int var3, int var4, int var5){
  targetArray[0] = var1;
  targetArray[1] = var2;
  targetArray[2] = var3;
  targetArray[3] = var4;
  targetArray[4] = var5;
}

