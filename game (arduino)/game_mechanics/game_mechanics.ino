#include <Adafruit_NeoPixel.h>

#define PIN 6

#define USE_LEDS 1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(64, PIN, NEO_GRB + NEO_KHZ800);

/*colours*/
uint32_t red = strip.Color(255, 0, 0);
uint32_t off = strip.Color(0, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t magenta = strip.Color(255, 0, 255);
uint32_t cyan = strip.Color(0, 255, 255);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t black = strip.Color(255, 255, 255);

// ==== Constants ====

/* ---- hardware dependent ---- */
const int MAX_BOARD_X = 8,
          MAX_BOARD_Y = 8,
          LED_BRIGHTNESS = 25;
/* -- end hardware dependent -- */

/* ---- game ids ---- */
const int CONNECT_FOUR = 0;
/* -- end game ids -- */

/* ---- game parameters ---- */
const int P1 = 1,          //
          P2 = 2,          // tokens used on board
          EMPTY_CELL = 0;  //
const int X_DIM = 8,
          Y_DIM = 8; 
/* -- end game parameters -- */

/* ---- error states ---- */
const int FN_SUCCESS = 1,
          ERR_GENERIC_FAIL = -1,
          ERR_INVALID_GAMEID = -2;
/* -- end error states -- */

// == End Constants ==

void setup() {
  Serial.begin(9600);
  
  //added by ben
  strip.begin();
  strip.setBrightness(LED_BRIGHTNESS);
  strip.show();
  
  startupLEDSequence();
}

void loop() {
  int gameId = 0; //!!! if time allows, input from bluetooth app
  int e = runGame(gameId);

  // error handle
  switch (e) {
    case FN_SUCCESS : {
      break;
    }
    case ERR_INVALID_GAMEID : {
      Serial.print("** ERR - INVALID GAME ID in loop(): "); Serial.println(gameId);
      break;
    }
    case ERR_GENERIC_FAIL : {
      Serial.println("** ERR - GENERIC FAIL: runGame(gameId) failed");
      break;
    }
  }
  
  //!!! need loop/replay handling -> TBD
}

int runGame(int gameId) {
  switch (gameId) {
    case CONNECT_FOUR : {
        int e = playConnectFour();

        // error handle
        if (e == 1) {
          return FN_SUCCESS;
        } else {
          return ERR_GENERIC_FAIL;
        }
        break;
      }
    //!!! add cases per new game
    default : {
      // gameId did not match any library games
      return ERR_INVALID_GAMEID;
    }
  }
}

int playConnectFour() {
  // Create board
  int **board = createBoard();

  // initialize board as empty (no tokens until users start dropping)
  int x, y;
  for (y = 0; y < Y_DIM; y++) {
    for (x = 0; x < X_DIM; x++) {
      board[y][x] = P1;
    }
  }
  
//  printCell(1, 1, red);
//  printCell(4, 5, blue);
//  printCell(2, 3, green);
//  while(1);
  
  //---- gameplay ----
  int cur_player = P1; //cur_player is one of {P1, P2}
  while(!gameOver_connect4(board)) {
    //!!! game logic not done
    
    int col;
    do {
      //!!! -- take input --
      //!!! use serial for debug
      while(!Serial.available()); //wait for serial data before parsing
      col = Serial.parseInt();
      Serial.println(col);
      
      // clamp inputs to valid range
      col = col < 0 ? 0 : col >= X_DIM ? X_DIM - 1 : col;
    } while (!dropToken_connect4(board, col, cur_player));
    
    printBoard(board);
    
    cur_player = cur_player == P1 ? P2 : P1; //change turns
  }
  //-- end gameplay --
  
  //!!! perhaps delay this call for user to confirm end game
  connect4Cascade(board); //GUI feature indicates game end, ***CLEARS BOARD***
  
  // free malloc'd memory
  freeBoard(board);
  
  return FN_SUCCESS; //game completed successfully
}

/*
 *  Place a player token on board in column x !!!untested
 */
int dropToken_connect4(int **board, int col, int token) {
  int y = -1;
  while(board[y + 1][col] == EMPTY_CELL) {
    y++;
  }
  
  if(y == -1) { //no empty cells in column col
    return 0;
  }
  
  board[y][col] = token; //place token
  
  return 1; //successfully dropped token
}

/*
 *  Return true if a player has won, or a draw is reached (board is full)
 */
int gameOver_connect4(int **board) {
  return winningPlayer_connect4(board) > 0 || boardFull(board);
}

/*
 *  Graphical sequence replicating pulling bottom out of connect 4 board
 *  Modifies board: replaces all user tokens with EMPTY_CELL
 */
void connect4Cascade(int **board) {
  //!!!increment tokens down, falling out of bottom of board
}

/*
 *  Return player ID of winning player, 0 if none
 */ 
int winningPlayer_connect4(int **board) {
  //!!! traverse board looking for 4 in a row
  return 0;
}

/*
 *  Returns 0 if board contains 1+ empty cells
 */
int boardFull(int **board) {
  //!!! traverse board, if has empty cell return false
  return 0;
}

/*
 *  Allocate a x_dim by y_dim board to be used in a game
 */
int ** createBoard() {
  int **board = (int **)malloc(Y_DIM * sizeof(int *));
  int x, y; // loop indeces
  for (y = 0; y < Y_DIM; y++) {
    board[y] = (int *)malloc(X_DIM * sizeof(int));
  }

  return board;
}

/*
 *  Free an x_dim by (irrelevant) board's allocated memory
 */
void freeBoard(int **board) {
  int y;
  for (y = 0; y < Y_DIM; y++) {
    free(board[y]);
  }
  free(board);
}

/*
 *  Output current state of a board
 */
void printBoard(int **board) {
  //!!! to be changed to print to LEDs
  //    uses serial for dev
  int x, y;
  for (y = 0; y < Y_DIM; y++) {
    for (x = 0; x < X_DIM; x++) {
      #if !USE_LEDS
        Serial.print(board[y][x]); Serial.print("\t");
      #else
        printCell(x, y, board[y][x] == P1 ? red : board[y][x] == P2 ? blue : off);
      #endif
    }
    #if !USE_LEDS
      Serial.println();
    #endif
  }
  #if !USE_LEDS
    Serial.println();
  #endif
}

/*
 * given a row,column and color light up specified led
 */
void printCell(int x, int y, uint32_t color){
  int led = X_DIM * y + x;
  strip.setPixelColor(led, color); //sets colour of first pixel
  strip.show(); //'pushes' colour data to the strip 
}

/*
 *  Displays a welcome/startup pattern on the LEDs
 */
void startupLEDSequence() {
  int i = 0;
  
  uint32_t square1 = red;
  uint32_t square2 = magenta;
  uint32_t square3 = green;
  uint32_t square4 = blue;

  //uint32_t[] colors = red, magenta, green, blue;
  
  for(i = 0; i < 10; i++) {//while(i == 0){
    //printsquare2
    printCell(3,3,square1);
    printCell(4,3,square1);
    printCell(3,4,square1);
    printCell(4,4,square1);
  
    //print square 2
    printCell(2,2,square2);
    printCell(2,3,square2);
    printCell(2,4,square2);
    printCell(2,5,square2);
    
    printCell(3,2,square2);
    printCell(4,2,square2);
    printCell(3,5,square2);
    printCell(4,5,square2);
  
    printCell(5,2,square2);
    printCell(5,3,square2);
    printCell(5,4,square2);
    printCell(5,5,square2); 
  
    //print square 3
    printCell(1,1,square3);  
    printCell(1,2,square3);
    printCell(1,3,square3);
    printCell(1,4,square3);
    printCell(1,5,square3);
    printCell(1,6,square3);
  
    printCell(2,1,square3);  
    printCell(3,1,square3);
    printCell(4,1,square3);
    printCell(5,1,square3);  
    printCell(2,6,square3);  
    printCell(3,6,square3);
    printCell(4,6,square3);
    printCell(5,6,square3); 
  
    printCell(6,1,square3);  
    printCell(6,2,square3);
    printCell(6,3,square3);
    printCell(6,4,square3);
    printCell(6,5,square3);
    printCell(6,6,square3);
  
    //print square 4
    printCell(0,0,square4);
    printCell(0,1,square4);  
    printCell(0,2,square4);
    printCell(0,3,square4);
    printCell(0,4,square4);
    printCell(0,5,square4);
    printCell(0,6,square4);
    printCell(0,7,square4);
  
    printCell(1,0,square4);
    printCell(2,0,square4);
    printCell(3,0,square4);
    printCell(4,0,square4);
    printCell(5,0,square4);
    printCell(6,0,square4);
    printCell(1,7,square4);
    printCell(2,7,square4);
    printCell(3,7,square4);
    printCell(4,7,square4);
    printCell(5,7,square4);
    printCell(6,7,square4);
  
    printCell(7,0,square4);
    printCell(7,1,square4);  
    printCell(7,2,square4);
    printCell(7,3,square4);
    printCell(7,4,square4);
    printCell(7,5,square4);
    printCell(7,6,square4);
    printCell(7,7,square4);

  //switch colors
    uint32_t temp = square1;
    square1 = square4;
    square4 = square3;
    square3 = square2;
    square2 = temp;
  
    delay(250);
  }  

  for(int j = 0; j<64; j++){
    strip.setPixelColor(j,0);
    strip.show();
  }

  printFive(blue);
  delay(2000);
  printFive(0);
}

void printFive(uint32_t c) {
  printCell(1,2,c);
  printCell(1,3,c);
  printCell(1,4,c);
  printCell(1,5,c); 

  printCell(2,2,c);
  printCell(3,2,c);
  
  printCell(3,3,c);
  printCell(4,5,c);
  printCell(5,5,c);
  printCell(6,4,c);
  printCell(6,3,c);
  printCell(6,2,c);
}

int calculateLedPosition(int x, int y){
  int led = 8 * y + x;
  return led; 
}

