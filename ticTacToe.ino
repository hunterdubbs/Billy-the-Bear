//call col then row
//0 = none
//1 = blue
//2 = red
int board[][3] = {{0, 0, 0},{0, 0, 0},{0, 0, 0}};

//whose turn it is
//0 = game not running
//1 = player
//2 = computer
int turnIdentifier = 0;

//which pin the leds start at; should be all blue then all red
int pinCounter = 22;

//turn number; used for optimization
int turn = 0;
int feedbackCounter = 1;

const int SEPERATER_CONSTANT = 9;

void setup() {
  Serial.begin(9600);
  //assign pins******
  //buttons
  for(int i=0;i<9;i++){
    pinMode(i+2, INPUT);
  }
  //leds
  for(int i=0;i<9;i++){
    pinMode(i+SEPERATER_CONSTANT+pinCounter, INPUT);
    digitalWrite(i+SEPERATER_CONSTANT+pinCounter, LOW);
    pinMode(i+pinCounter, INPUT);
    digitalWrite(i+pinCounter, LOW);
  }
      
  //************ setup additions can go here*******************
  turnIdentifier = 1;
  
}

void loop() {
  //debugging
  /*
  for(int i=2;i<11;i++){
    if(digitalRead(i) == HIGH){
      Serial.println(i);
    }
  }*/

  /*board[0][0] = 1;
  board[0][1] = 1;
  board[0][2] = 1;
  board[1][0] = 1;
  board[1][1] = 1;
  board[1][2] = 1;
  board[2][0] = 1;
  board[2][1] = 1;
  board[2][2] = 1;
  updateLED();


  delay(3000);

  board[0][0] = 0;
  board[0][1] = 0;
  board[0][2] = 0;
  board[1][0] = 0;
  board[1][1] = 0;
  board[1][2] = 0;
  board[2][0] = 0;
  board[2][1] = 0;
  board[2][2] = 0;
  updateLED();

  delay(3000);

  board[0][0] = 2;
  board[0][1] = 2;
  board[0][2] = 2;
  board[1][0] = 2;
  board[1][1] = 2;
  board[1][2] = 2;
  board[2][0] = 2;
  board[2][1] = 2;
  board[2][2] = 2;
  updateLED();

  delay(3000);*/

  //end debugging
  
  if (turnIdentifier == 1){
    //check button states
    int pinCounter = 2;
    for (int i = 0; i < 3; i++){
      for (int j= 0; j< 3; j++){
      
        if (digitalRead(pinCounter) == HIGH && board[i][j] == 0){
          board[i][j] = 1;
          turn++;
          if(turnIdentifier == 1){
            turnIdentifier = 2;
          }else{
            turnIdentifier = 1;
          }
          Serial.println("player turn accepted");
          updateLED();
          if(checkForWin() == false){
            if(turn >= 9){
              //was a draw
              //reset
              board[0][0] = 0;
              board[0][1] = 0;
              board[0][2] = 0;
              board[1][0] = 0;
              board[1][1] = 0;
              board[1][2] = 0;
              board[2][0] = 0;
              board[2][1] = 0;
              board[2][2] = 0;
              updateLED();
              turn = 0;
              turnIdentifier = 1;
              delay(3000);
            }
          }
        }
        pinCounter++;
      }
    }
  }
  
   //computer's turn
   if (turnIdentifier == 2){
    bool turnComplete = false;
    while (turnComplete == false){
     int col = floor(random(0, 3.999));
     int row = floor(random(0, 3.999));
     if(board[col][row] == 0){
      board[col][row] = 2;
      Serial.println("AI turn accepted");
      updateLED();
      turnIdentifier = 1;
      if(checkForWin() == false){
            if(turn >= 9){
              //was a draw
              //reset
              board[0][0] = 0;
              board[0][1] = 0;
              board[0][2] = 0;
              board[1][0] = 0;
              board[1][1] = 0;
              board[1][2] = 0;
              board[2][0] = 0;
              board[2][1] = 0;
              board[2][2] = 0;
              updateLED();
              turn = 0;
              turnIdentifier = 1;
              delay(3000);
            }
        }
      turn++;
      turnComplete = true;
     }
    }
   }

  //************ loop additions can go here*******************
  
}

void setLED(int col, int row, int color){
  board[col][row] = color;
  updateLED();
}

void updateLED(){
  int pinCounter = 22;
  int pinCounterAI = 31;
  for (int i = 0; i < 3; i++){
    for (int j= 0; j< 3; j++){
      if (board[i][j] == 0){
        digitalWrite(pinCounter, LOW);
        digitalWrite(pinCounterAI, LOW);
      }
      else if (board[i][j] == 1){
        digitalWrite(pinCounter, HIGH);
        digitalWrite(pinCounterAI, LOW);
      }
      else if (board[i][j] == 2){
        digitalWrite(pinCounter, LOW);
        digitalWrite(pinCounterAI, HIGH);
      }
      pinCounter++;
      pinCounterAI++;
    }
  }
}

bool checkForWin(){
  //see if it is yet possible to win
  if(turn >= 5){
  //player wins **********
  //up - down
    if(board[0][0] == 1 && board[0][1] == 1 && board[0][2] == 1) {playerWin("c", 0);return true;}
    else if(board[1][0] == 1 && board[1][1] == 1 && board[1][2] == 1) {playerWin("c", 1);return true;}
    else if(board[2][0] == 1 && board[2][1] == 1 && board[2][2] == 1) {playerWin("c", 2);return true;}
    //left - right
    else if(board[0][0] == 1 && board[1][0] == 1 && board[2][0] == 1) {playerWin("r", 0);return true;}
    else if(board[0][1] == 1 && board[1][1] == 1 && board[2][1] == 1) {playerWin("r", 1);return true;}
    else if(board[0][2] == 1 && board[1][2] == 1 && board[2][2] == 1) {playerWin("r", 2);return true;}
    //diagonal
    else if(board[0][0] == 1 && board[1][1] == 1 && board[2][2] == 1) {playerWin("d", 0);return true;}
    else if(board[0][2] == 1 && board[1][1] == 1 && board[2][0] == 1) {playerWin("d", 1);return true;}
  
    //computer wins **********
    //up - down
    else if(board[0][0] == 2 && board[0][1] == 2 && board[0][2] == 2) {computerWin("c", 0);return true;}
    else if(board[1][0] == 2 && board[1][1] == 2 && board[1][2] == 2) {computerWin("c", 1);return true;}
    else if(board[2][0] == 2 && board[2][1] == 2 && board[2][2] == 2) {computerWin("c", 2);return true;}
    //left - right
    else if(board[0][0] == 2 && board[1][0] == 2 && board[2][0] == 2) {computerWin("r", 0);return true;}
    else if(board[0][1] == 2 && board[1][1] == 2 && board[2][1] == 2) {computerWin("r", 1);return true;}
    else if(board[0][2] == 2 && board[1][2] == 2 && board[2][2] == 2) {computerWin("r", 2);return true;}
    //diagonal
    else if(board[0][0] == 2 && board[1][1] == 2 && board[2][2] == 2) {computerWin("d", 0);return true;}
    else if(board[0][2] == 2 && board[1][1] == 2 && board[2][0] == 2) {computerWin("d", 1);return true;}
    else{return false;}
  }
}

//c = column
//r = row
//d = diagonal
void playerWin(String type, int loc){
  Serial.println("player win");
  Serial.print("type = ");
  Serial.println(type);
  Serial.print("loc = ");
  Serial.print(loc);
  //stop game
  turnIdentifier = 0;
  //turn off all lights
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      board[i][j] = 0;
    }
  }
  updateLED();
  //begin blink sequence - 3 iterations
  for(int i=0;i<3;i++){
    delay(250);
    if(type == "c"){
      board[loc][0] = 1;
      board[loc][1] = 1;
      board[loc][2] = 1;
      updateLED();
      delay(250);
      board[loc][0] = 0;
      board[loc][1] = 0;
      board[loc][2] = 0;
      updateLED();
    }
    else if(type == "r"){
      board[0][loc] = 1;
      board[1][loc] = 1;
      board[2][loc] = 1;
      updateLED();
      delay(250);
      board[0][loc] = 0;
      board[1][loc] = 0;
      board[2][loc] = 0;
      updateLED();
    }
    else if(type == "d"){
      if(loc == 0){
        board[0][0] = 1;
        board[1][1] = 1;
        board[2][2] = 1;
        updateLED();
        delay(250);
        board[0][0] = 0;
        board[1][1] = 0;
        board[2][2] = 0;
        updateLED();
      }
      else if(loc == 1){
        board[0][2] = 1;
        board[1][1] = 1;
        board[2][0] = 1;
        updateLED();
        delay(250);
        board[0][2] = 0;
        board[1][1] = 0;
        board[2][0] = 0;
        updateLED();
      }
    }
  }
  turnIdentifier = 1;
}

void computerWin(String type, int loc){
  Serial.println("AI win");
  //stop game
  turnIdentifier = 0;
  //turn off all lights
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      board[i][j] = 0;
    }
  }
  updateLED();
  //begin blink sequence - 3 iterations
  for(int i=0;i<3;i++){
    delay(250);
    if(type == "c"){
      board[loc][0] = 2;
      board[loc][1] = 2;
      board[loc][2] = 2;
      updateLED();
      delay(250);
      board[loc][0] = 0;
      board[loc][1] = 0;
      board[loc][2] = 0;
      updateLED();
    }
    else if(type == "r"){
      board[0][loc] = 2;
      board[1][loc] = 2;
      board[2][loc] = 2;
      updateLED();
      delay(250);
      board[0][loc] = 0;
      board[1][loc] = 0;
      board[2][loc] = 0;
      updateLED();
    }
    else if(type == "d"){
      if(loc == 0){
        board[0][0] = 2;
        board[1][1] = 2;
        board[2][2] = 2;
        updateLED();
        delay(250);
        board[0][0] = 0;
        board[1][1] = 0;
        board[2][2] = 0;
        updateLED();
      }
      else if(loc == 1){
        board[0][2] = 2;
        board[1][1] = 2;
        board[2][0] = 2;
        updateLED();
        delay(250);
        board[0][2] = 0;
        board[1][1] = 0;
        board[2][0] = 0;
        updateLED();
      }
    }
  }
  turnIdentifier = 1;
}
