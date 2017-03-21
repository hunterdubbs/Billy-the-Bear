#include <Servo.h>


//this is how many times a second the motors update
//a higher value will make movements smoother
const int updateRate = 10;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

//this function will move all given motors to their given positions in such a way that they arrive at the same time.
//accepts an array of servos, an array of the desired end positions, the time that it should take to get there, and the number of motors it will move
void moveMotor(Servo servos[], int pos[], int dur, int numMotors){
  int num = numMotors;
  int startPos[num];
  int moveRange[num];
  float movePerUpdate[num];
  int reps = dur * updateRate;

  for(int i=0;i<num;i++){
    startPos[i] = servos[i].read();
    moveRange[i] = abs(startPos[i]-pos[i]);
    movePerUpdate[i] = moveRange[i] / reps;
  }


  //move motors with computed values
  for(int i=0;i<reps;i++){
    for(int j=0;j<num;j++){
       servos[j].write(movePerUpdate[j]*(i+1)+startPos[j]);
    }
    delay(1000/updateRate);
  }
}

