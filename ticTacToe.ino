//call col then row
//0 = none
//1 = blue
//2 = red
int board[][3] = {{0, 0, 0},{0, 0, 0},{0, 0, 0}};
int soundLength[8] = {33000, 7500, 7500, 2500, 3500, 10000, 13000, 10000};
char lastMovePos;

//whose turn it is
//0 = game not running
//1 = player
//2 = computer
int turnIdentifier = 0;
bool dispRunning = false;

//which pin the leds start at; should be all blue then all red
int pinCounter = 22;
int soundPinStart = 40;

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
    pinMode(i+SEPERATER_CONSTANT+pinCounter, OUTPUT);
    digitalWrite(i+SEPERATER_CONSTANT+pinCounter, LOW);
    pinMode(i+pinCounter, OUTPUT);
    digitalWrite(i+pinCounter, LOW);
  }

  //sound pins
  for(int i=0;i<8;i++){
    pinMode(i+soundPinStart, OUTPUT);
    digitalWrite(i+soundPinStart, HIGH);
  }
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(11, HIGH);
  digitalWrite(13, HIGH);

  //start button
  pinMode(12, INPUT_PULLUP);
      
  //************ setup additions can go here******************/

 
 
}

void loop() {

  if(digitalRead(12) == LOW){
    dispRunning = true;
    startGame();
  }

  while(dispRunning){
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
          //Serial.println("player turn accepted");
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
              playSound(7);
              Serial.print("S");
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
    playSound(floor(turn/2)+1);
    Serial.print("S");
    delay(soundLength[int(floor(turn/2)+1)]);
    delay(4200);
    getNextMove(false);
    //Serial.print(String(lastMovePos));
    delay(150);
    Serial.print("D");
    delay(3000);
    while (turnComplete == false){
      delay(500); //was 500
      updateLED();
      turnIdentifier = 1;
      if(checkForWin() == false){
            delay(500);
            playSound(8);
            Serial.print("N");
            if(turn >= 9){
              //was a draw
              playSound(7);
              Serial.print("S");
              delay(10000);
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
              delay(5000);
              dispRunning = false;
              Serial.print("R");
              break;
            }
        }
      turn++;
      turnComplete = true;
     }
    }

  //************ loop additions can go here*******************
  }
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
  }else{return false;}
}

//c = column
//r = row
//d = diagonal
void playerWin(String type, int loc){
  /*Serial.println("player win");
  Serial.print("type = ");
  Serial.println(type);
  Serial.print("loc = ");
  Serial.print(loc);*/
  //stop game
  turnIdentifier = 0;
  turn = 0;
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
  playSound(5);
  Serial.print("Q");
  Serial.print("S");
  delay(10000);
  Serial.print("E");
  turnIdentifier = 1;
  dispRunning = false;
  Serial.print("R");
}

void computerWin(String type, int loc){
  //Serial.println("AI win");
  //stop game
  delay(2000);
  turnIdentifier = 0;
  turn = 0;
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
  playSound(6);
  Serial.print("S");
  delay(13000);
  Serial.print("V");
  turnIdentifier = 1;
  dispRunning = false;
  Serial.print("R");
}

void startGame(){
  
  playSound(0); //do intro
  Serial.print("C");
  delay(5000);
  Serial.print("S");
  delay(29000);
  turnIdentifier = 1;
  turn = 0;
}

void playSound(int index){
  if(index == 8){
    digitalWrite(11, LOW);
    digitalWrite(13, LOW);
    delay(100);
    digitalWrite(11, HIGH);
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(index + soundPinStart, LOW);
    delay(100);
    digitalWrite(index + soundPinStart, HIGH);
  }
}

void getNextMove(bool flag){
  if (turn == 1 || flag){
    getRandMove();
  }else{
    //************check for possible win*******//
    //horizontal and vertical
    for(int i=0;i<3;i++){
      if (board[i][0] == 2 && board[i][1] == 2){ 
        if(board[i][2] == 0){
          board[i][2] = 2;
          if(i==0){
            Serial.print("J");
          }else if(i==1){
            Serial.print("K");
          }else if(i==2){
            Serial.print("L");
          }
          goto algorEnd;
        }
      }
      if (board[i][1] == 2 && board[i][2] == 2){ 
        if(board[i][0] == 0){
          board[i][0] = 2;
          if(i==0){
            Serial.print("T");
          }else if(i==1){
            Serial.print("Y");
          }else if(i==2){
            Serial.print("U");
          }
          goto algorEnd;
        }
      }
      if (board[0][i] == 2 && board[1][i] == 2){ 
        if(board[2][i] == 0){
          board[2][i] = 2;
          if(i==0){
            Serial.print("U");
          }else if(i==1){
            Serial.print("P");
          }else if(i==2){
            Serial.print("L");
          }
          goto algorEnd;
        }
      }
      if (board[1][i] == 2 && board[2][i] == 2){ 
        if(board[0][i] == 0){
          board[0][i] = 2;
          if(i==0){
            Serial.print("T");
          }else if(i==1){
            Serial.print("I");
          }else if(i==2){
           Serial.print("J");
          }
          goto algorEnd;
        }
      }
    }
    //diagonals
    if (board[0][0] == 2 && board[1][1] == 2){
      if(board[2][2] == 0){
        board[2][2] = 2;
        Serial.print("L");
        goto algorEnd;
      }
    }
    if (board[1][1] == 2 && board[2][2] == 2){
      if(board[0][0] == 0){
        board[0][0] = 2;
        Serial.print("T");
        goto algorEnd;
      }
    }
    if (board[0][2] == 2 && board[1][1] == 2){
      if(board[2][0] == 0){
        board[2][0] = 2;
        Serial.print("U");
        goto algorEnd;
      }
    }
    if (board[2][0] == 2 && board[1][1] == 2){
      if(board[0][2] == 0){
        board[0][2] = 2;
        Serial.print("J");
        goto algorEnd;
      }
    }
    //************check for possible loss*******//
    //horizontal and vertical
    for(int i=0;i<3;i++){
      if (board[i][0] == 1 && board[i][1] == 1){ 
        if(board[i][2] == 0){
          board[i][2] = 2;
          if(i==0){
            Serial.print("J");
          }else if(i==1){
            Serial.print("K");
          }else if(i==2){
            Serial.print("L");
          }
          goto algorEnd;
        }
      }
      if (board[i][1] == 1 && board[i][2] == 1){ 
        if(board[i][0] == 0){
          board[i][0] = 2;
          if(i==0){
            Serial.print("T");
          }else if(i==1){
            Serial.print("Y");
          }else if(i==2){
            Serial.print("U");
          }
          goto algorEnd;
        }
      }
      if (board[0][i] == 1 && board[1][i] == 1){ 
        if(board[2][i] == 0){
          board[2][i] = 2;
          if(i==0){
            Serial.print("U");
          }else if(i==1){
            Serial.print("P");
          }else if(i==2){
            Serial.print("L");
          }
          goto algorEnd;
        }
      }
      if (board[1][i] == 1 && board[2][i] == 1){ 
        if(board[0][i] == 0){
          board[0][i] = 2;
          if(i==0){
            Serial.print("T");
          }else if(i==1){
            Serial.print("I");
          }else if(i==2){
            Serial.print("J");
          }
          goto algorEnd;
        }
      }
    }
    //diagonals
    if (board[0][0] == 1 && board[1][1] == 1){
      if(board[2][2] == 0){
        board[2][2] = 2;
        Serial.print("L");
        goto algorEnd;
      }
    }
    if (board[1][1] == 1 && board[2][2] == 1){
      if(board[0][0] == 0){
        board[0][0] = 2;
        Serial.print("T");
        goto algorEnd;
      }
    }
    if (board[0][2] == 1 && board[1][1] == 1){
      if(board[2][0] == 0){
        board[2][0] = 2;
        Serial.print("U");
        goto algorEnd;
      }
    }
    if (board[2][0] == 1 && board[1][1] == 1){
      if(board[0][2] == 0){
        board[0][2] = 2;
        Serial.print("J");
        goto algorEnd;
      }
    }
    else {
      getRandMove();
    }
    algorEnd:
    //Serial.print(String(lastMovePos));
    delay(1);
  }
}
void getRandMove(){
  bool done = false;
  while (!done){
    int col = floor(random(0, 3.999));
    int row = floor(random(0, 3.999));
    if(board[col][row] == 0){
      board[col][row] = 2;
      if(col==0){
        if(row==0){
          Serial.print("T");
        }else if(row==1){
          Serial.print("I");
        }else if(row==2){
          Serial.print("J");
        }
      }else if(col==1){
        if(row==0){
          Serial.print("Y");
        }else if(row==1){
          Serial.print("O");
        }else if(row==2){
          Serial.print("K");
        }
      }else if(col==2){
        if(row==0){
          Serial.print("U");
        }else if(row==1){
          Serial.print("P");
        }else if(row==2){
          Serial.print("L");
        }
      }
      //Serial.print(String(lastMovePos));
      
      done = true;
    }
  }
}

