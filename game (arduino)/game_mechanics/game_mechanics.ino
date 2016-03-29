#include <Adafruit_NeoPixel.h>

#define PIN 6

#define DEBUG 0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(64, PIN, NEO_GRB + NEO_KHZ800);

/*colours*/
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t magenta = strip.Color(255, 0, 255);
uint32_t cyan = strip.Color(0, 255, 255);
uint32_t yellow = strip.Color(255, 255, 0);

uint32_t webworkGreen = strip.Color(118, 255, 122);
uint32_t white = strip.Color(255, 255, 255);
uint32_t off = strip.Color(0, 0, 0);

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
          Y_DIM = 8,
          SEQ_LENGTH = 5;
const int TOKEN_DROP_SPEED = 35; // millis delay for token dropping
/* -- end game parameters -- */

/* ---- error states ---- */
const int FN_SUCCESS = 1,
          ERR_GENERIC_FAIL = -1,
          ERR_INVALID_GAMEID = -2;
/* -- end error states -- */

/* ---- directions ---- */
const int UP = 1,
          LEFT = 2,
          UP_LEFT = 3,
          DOWN_LEFT = 4;
/* -- end directions -- */

// == End Constants ==


// ==== Global Variables ====

/* ---- player info ---- */
uint32_t p1_colour = red,
         p2_colour = blue;
int turns;
/* -- end player info -- */

// == End Global Variables ==

void setup() {
  Serial.begin(9600);

  //buffer for parsing
  char appBuffer[128] = {0};

  //initialize LED array
  strip.begin();
  strip.setBrightness(LED_BRIGHTNESS);
  strip.show();

#if !DEBUG
  startupLEDSequence();
#endif
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
      board[y][x] = EMPTY_CELL;
    }
  }
  printBoard(board);

  //---- gameplay ----
  int cur_player = P1; //cur_player is one of {P1, P2}
  turns = 0;
  while (!gameOver_connect4(board)) {
    //!!! game logic not done

    Serial.println("!GO");

    int col;
    do {
      //!!! -- take input --
      //!!! use serial for debug
      while (!Serial.available()); //wait for serial data before parsing
      col = Serial.parseInt();
      Serial.println(col);

      // clamp inputs to valid range
      col = col < 0 ? 0 : col >= X_DIM ? X_DIM - 1 : col;
    } while (!dropToken_connect4(board, col, cur_player));

    printBoard(board);

    cur_player = cur_player == P1 ? P2 : P1; //change turns
    turns++;
  }
  //-- end gameplay --

  //!!! perhaps delay this call for user to confirm end game
  connect4Cascade(board); //GUI feature indicates game end, ***CLEARS BOARD***

  // free malloc'd memory
  freeBoard(board);

  return FN_SUCCESS; //game completed successfully
}

/*
    Place a player token on board in column x !!!untested
*/
int dropToken_connect4(int **board, int col, int token) {
  int y = -1;
  while (board[y + 1][col] == EMPTY_CELL && y + 1 < Y_DIM) {
    y++;
  }

  if (y == -1) { //no empty cells in column col
    return 0;
  }

  Serial.print(col);
  Serial.print(" ");
  Serial.print(token);
  Serial.print(" ");
  Serial.print(y);
  Serial.println(" ");

  uint32_t colour = getPlayerColour(token);

  int i = -2;
  while (++i < y - 1) {
    delay(TOKEN_DROP_SPEED);
    printCell(col, i, off, 0);
    printCell(col, i + 1, colour, 0);
    strip.show();
  }

  board[y][col] = token; //place token

  return 1; //successfully dropped token
}

/*
    Return true if a player has won, or a draw is reached (board is full)
*/
int gameOver_connect4(int **board) {
  return winningPlayer_connect4(board) > 0 || boardFull();
}

/*
    Graphical sequence replicating pulling bottom out of connect 4 board
    Modifies board: replaces all user tokens with EMPTY_CELL
    !!!untested!!!
*/
void connect4Cascade(int **board) {
  Serial.print("Cascade\n");
  int x, y, count;

  for (count = 0; count < Y_DIM; count++) {
    for (y = Y_DIM - 1; y >= count; y--) {
      for (x = 0; x < X_DIM; x++) {
        if (y == 0) {
          board[y][x] = EMPTY_CELL;
        } else {
          board[y][x] = board[y - 1][x];
        }
      }
    }

    printBoard(board);
    delay(TOKEN_DROP_SPEED);
  }
}

/*
    Return player ID of winning player, 0 if none

    Modifies board: if winning player found, flashes winning
      tiles to webwork green 3 times

    !!!!!!NOTE: MUST SPECIFY WIN DATA!!!!!!!
*/
int winningPlayer_connect4(int **board) {
  int *winData;
  int *checkWin;
  int i = 0;
  int flashes = 3 * 2;

  checkWin = detectDiagLine(P1, SEQ_LENGTH, board);
  winData = checkWin[0] != -1 ? checkWin : NULL;
  if (winData != NULL)
  //flashWin();
    return winData[3];

  checkWin = detectHorizLine(P1, SEQ_LENGTH, board);
  winData = checkWin[0] != -1 ? checkWin : NULL;
  if (winData != NULL)
    return winData[3];

  checkWin = detectVertLine(P1, SEQ_LENGTH, board);
  winData = checkWin[0] != -1 ? checkWin : NULL;
  if (winData != NULL)
    return winData[3];

  checkWin = detectDiagLine(P2, SEQ_LENGTH, board);
  winData = checkWin[0] != -1 ? checkWin : NULL;
  if (winData != NULL)
    return winData[3];

  checkWin = detectHorizLine(P2, SEQ_LENGTH, board);
  winData = checkWin[0] != -1 ? checkWin : NULL;
  if (winData != NULL)
    return winData[3];

  checkWin = detectVertLine(P2, SEQ_LENGTH, board);
  winData = checkWin[0] != -1 ? checkWin : NULL;
  if (winData != NULL)
    return winData[3];


  if (winData == NULL)
    return 0;
  else
    return winData[3];

  //flashes 3 times
  //  while (flashes-- > 0) {
  //    while (i++ < SEQ_LENGTH) {
  //      switch (winData[2]) {
  //        case UP:
  //          printCell(winData[0] + i, winData[1], flashes % 2 ? webworkGreen : off, 0); break;
  //        case LEFT:
  //          printCell(winData[0], winData[1] + i, flashes % 2 ? webworkGreen : off, 0); break;
  //        case UP_LEFT:
  //          printCell(winData[0] + i, winData[1] + i, flashes % 2 ? webworkGreen : off, 0); break;
  //        case DOWN_LEFT:
  //          printCell(winData[0] - i, winData[1] + i, flashes % 2 ? webworkGreen : off, 0); break;
  //        default:
  //          Serial.println("ERROR: check line detecting logic"); break;
  //      }
  //    }
  //    delay(500);
  //  }

  //return winData[3];

}

/*
    Returns 0 if board contains 1+ empty cells
*/
int boardFull() {
  return turns >= X_DIM * Y_DIM;
}

/*
    Allocate a x_dim by y_dim board to be used in a game
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
    Free an x_dim by (irrelevant) board's allocated memory
*/
void freeBoard(int **board) {
  int y;
  for (y = 0; y < Y_DIM; y++) {
    free(board[y]);
  }
  free(board);
}

/*
    Output current state of a board
*/
void printBoard(int **board) {
  //!!! to be changed to print to LEDs
  //    uses serial for dev
  int x, y;
  for (y = 0; y < Y_DIM; y++) {
    for (x = 0; x < X_DIM; x++) {
      Serial.print(board[y][x]); Serial.print("\t");
      printCell(x, y, getPlayerColour(board[y][x]), 0);
    }
    Serial.println();
  }
  Serial.println();

  strip.show();
}

/*
   given a row,column and color light up specified led
*/
void printCell(int x, int y, uint32_t color, int show) {
  int led = X_DIM * y + x;
  strip.setPixelColor(led, color); //sets colour of first pixel
  if (show) {
    strip.show(); //'pushes' colour data to the strip
  }
}

/*
    Displays a welcome/startup pattern on the LEDs
*/
void startupLEDSequence() {
  int i = 0;

  uint32_t square1 = blue;
  uint32_t square2 = cyan;
  uint32_t square3 = green;
  uint32_t square4 = yellow;

  //uint32_t[] colors = red, magenta, green, blue;

  for (i = 0; i < 10; i++) { //while(i == 0){
    //printsquare2
    printCell(3, 3, square1 , 1);
    printCell(4, 3, square1 , 1);
    printCell(3, 4, square1 , 1);
    printCell(4, 4, square1 , 1);

    //print square 2
    printCell(2, 2, square2 , 1);
    printCell(2, 3, square2 , 1);
    printCell(2, 4, square2 , 1);
    printCell(2, 5, square2 , 1);

    printCell(3, 2, square2 , 1);
    printCell(4, 2, square2 , 1);
    printCell(3, 5, square2 , 1);
    printCell(4, 5, square2 , 1);

    printCell(5, 2, square2 , 1);
    printCell(5, 3, square2 , 1);
    printCell(5, 4, square2 , 1);
    printCell(5, 5, square2 , 1);

    //print square 3
    printCell(1, 1, square3 , 1);
    printCell(1, 2, square3 , 1);
    printCell(1, 3, square3 , 1);
    printCell(1, 4, square3 , 1);
    printCell(1, 5, square3 , 1);
    printCell(1, 6, square3 , 1);

    printCell(2, 1, square3 , 1);
    printCell(3, 1, square3 , 1);
    printCell(4, 1, square3 , 1);
    printCell(5, 1, square3 , 1);
    printCell(2, 6, square3 , 1);
    printCell(3, 6, square3 , 1);
    printCell(4, 6, square3 , 1);
    printCell(5, 6, square3 , 1);

    printCell(6, 1, square3 , 1);
    printCell(6, 2, square3 , 1);
    printCell(6, 3, square3 , 1);
    printCell(6, 4, square3 , 1);
    printCell(6, 5, square3 , 1);
    printCell(6, 6, square3 , 1);

    //print square 4
    printCell(0, 0, square4 , 1);
    printCell(0, 1, square4 , 1);
    printCell(0, 2, square4 , 1);
    printCell(0, 3, square4 , 1);
    printCell(0, 4, square4 , 1);
    printCell(0, 5, square4 , 1);
    printCell(0, 6, square4 , 1);
    printCell(0, 7, square4 , 1);

    printCell(1, 0, square4 , 1);
    printCell(2, 0, square4 , 1);
    printCell(3, 0, square4 , 1);
    printCell(4, 0, square4 , 1);
    printCell(5, 0, square4 , 1);
    printCell(6, 0, square4 , 1);
    printCell(1, 7, square4 , 1);
    printCell(2, 7, square4 , 1);
    printCell(3, 7, square4 , 1);
    printCell(4, 7, square4 , 1);
    printCell(5, 7, square4 , 1);
    printCell(6, 7, square4 , 1);

    printCell(7, 0, square4 , 1);
    printCell(7, 1, square4 , 1);
    printCell(7, 2, square4 , 1);
    printCell(7, 3, square4 , 1);
    printCell(7, 4, square4 , 1);
    printCell(7, 5, square4 , 1);
    printCell(7, 6, square4 , 1);
    printCell(7, 7, square4 , 1);

    //switch colors
    uint32_t temp = square1;
    square1 = square4;
    square4 = square3;
    square3 = square2;
    square2 = temp;

    delay(250);
  }

  for (int j = 0; j < X_DIM * Y_DIM; j++) {
    strip.setPixelColor(j, off);
    strip.show();
  }

  printCell(2, 1, blue , 1);
  printCell(3, 1, blue , 1);
  printCell(4, 1, blue , 1);
  printCell(5, 1, blue , 1);

  printCell(2, 2, blue , 1);
  printCell(2, 3, blue , 1);
  printCell(4, 3, blue , 1);

  printCell(3, 3, blue , 1);
  printCell(5, 4, blue , 1);
  printCell(5, 5, blue , 1);
  printCell(4, 6, blue , 1);
  printCell(3, 6, blue , 1);
  printCell(2, 6, blue , 1);
  delay(5000);

  for (int j = 0; j < X_DIM * Y_DIM; j++) {
    strip.setPixelColor(j, off);
    strip.show();
  }
}

/*
   returns the first sequence of lights found of length x
   in an array.
*/
//int* detectLineXLong(int player, int X, int **board) {
//  int i = 0;
//  int j = 0;
//  int k = 0;
//  int foundLine = 0;
//  int coordinateAndDirection[2];
//
//  for (i = 0; i < 64; i++) {
//    if (strip.getPixelColor(i) == player) {   //if the color is the same color as the player, check directions
//      for (j = 0; i < 8; j++) {
//        //search all 8 directions x long,
//        //if not same color or off, try next direction
//
//        if (j = 0) {
//          //try up
//          //for x= 3 if i > (x-1)*8 - 1
//          if (i > (X - 1) * 8 - 1) {
//            for (k = 0; k < X; k++) {
//              if (strip.setPixelColor(i - (k + 1) * 8, player) != strip.setPixelColor(i, player)) {
//                break;
//              }
//              //return the direction and pixel
//              foundLine = 1;
//            }
//          }
//        }
//
//        if (j = 1) {
//          //try down
//          if (i < 64 - 8 * (X - 1)) {
//            for (k = 0; k < X; k++) {
//              if (strip.setPixelColor(i + (k + 1) * 8, player) != strip.setPixelColor(i, player)) {
//                break;
//              }
//              //return the direction and pixel
//              foundLine = 1;
//            }
//          }
//        }
//
//        if (j = 2) {
//          //try left
//          if ( i % 8 > (X - 1)) { //Equation could be wrong
//            for (k = 0; k < X; k++) {
//              if (strip.setPixelColor(i + (k + 1), player) != strip.setPixelColor(i, player)) {
//                break;
//              }
//              //return the direction and pixel
//              foundLine = 1;
//            }
//          }
//        }
//
//        if (j = 3) {
//          //try right
//          if ( i % 8 < 8 - (X - 1)) { //Equation could be wrong
//            for (k = 0; k > X; k++) {
//              if (strip.setPixelColor(i - (k + 1), player) != strip.setPixelColor(i, player)) {
//                break;
//              }
//              i++; // since we are moving forward, we can skip lights if they are same color
//              foundLine = 1;
//            }
//          }
//        }
//
//        if (j = 4) {
//          //try up left
//          if ( i % 8 > (X - 1) && i > (X - 1) * 8 - 1) {
//            //...
//            for (k = 0; k > X; k++) {
//              if (strip.setPixelColor(i - (k + 1) * 9, player) != strip.setPixelColor(i, player)) {
//                break;
//              }
//              //return the direction and pixel
//              foundLine = 1;
//            }
//          }
//        }
//
//        if (j = 5) {
//          //try up right
//          if (i % 8 < 8 - (X - 1) && i > (X - 1) * 8 - 1) {
//            //...
//            for (k == 0; k > X; k++) {
//              if (strip.setPixelColor(i - (k + 1) * 7, player) != strip.setPixelColor(i, player)) {
//                break;
//              }
//              foundLine = 1;
//            }
//          }
//        }
//        if (j = 6) {
//          //try down left
//          if ( i % 8 > (X - 1) && i < 64 - 8 * (X - 1)) {
//            //...
//            for (k = 0; k > X; k++) {
//              if (strip.setPixelColor(i + (k + 1) * 7, player) != strip.setPixelColor(i, player))
//                break;
//              foundLine = 1;
//            }
//          }
//
//        }
//
//        if (j = 7) {
//          //try down right
//          if ( i % 8 < 8 - (X - 1) && i < 64 - 8 * (X - 1)) {
//            //...
//            for (k = 0; k > X; k++) {
//              if (strip.setPixelColor(i + (k + 1) * 9, player) != strip.setPixelColor(i, player)) {
//                break;
//              }
//              foundLine = 1;
//            }
//          }
//        }
//
//        if (foundLine == 1) break;
//      }
//    }
//    if (foundLine == 1) break;
//  }
//  if (foundLine == 0) coordinateAndDirection = {-1,-1};
//  else coordinateAndDirection = {i, j};
//  return coordinateAndDirection;
//}

/*
   Detects if there is a vertical sequence of one player's tokens of length len.
   If one is found, it returns the coordinates of the first token, as well as the direction
   of the sequence, in the form:

   -> returnArray[] = {x, y, dir, player}

   If no such sequence is found, returns NULL
*/
int* detectVertLine(int player, int len, int** board) {
  int x, y, count;
  int coordAndDir[4];

  for (y = 0; y <= Y_DIM - len; y++) {                          //iterate through bottom half of grid
    for (x = 0; x < X_DIM; x++) {
      for (count = 0; count < len; count++) {                   //count that you have a sequence of length len
        if (board[y + count][x] != player)
          break;     //you're not on the right track, try a different sequence
      }
      if (count == len) {
        //we found a sequence of length len, so we return coords/direction
        coordAndDir[0] = x; coordAndDir[1] = y; coordAndDir[2] = UP; coordAndDir[3] = player;
        return coordAndDir;
      }
    }
  }
  coordAndDir[0] = -1; coordAndDir[1] = -1; coordAndDir[2] = -1; coordAndDir[3] = -1;
  return coordAndDir;
}

/*
   Detects if there is a horizontal sequence of one player's tokens of length len.
   If one is found, it returns the coordinates of the first token, as well as the direction
   of the sequence, in the form:

   -> returnArray[] = {x, y, dir, player}

   If no such sequence is found, returns NULL
*/
int* detectHorizLine(int player, int len, int** board) {
  int x, y, count;
  int coordAndDir[4];

  for (x = 0; x <= X_DIM - len; x++) {                          //iterate through left half of grid
    for (y = 0; y < Y_DIM; y++) {
      for (count = 0; count < len; count++) {                   //count that you have a sequence of length len
        if (board[y][x + count] != player)
          break;     //you're not on the right track, try a different sequence
      }
      if (count == len) {
        //we found a sequence of length len, so we return coords/direction
        coordAndDir[0] = x; coordAndDir[1] = y; coordAndDir[2] = LEFT; coordAndDir[3] = player;
        return coordAndDir;
      }
    }
  }
  return NULL;
}

/*
   Detects if there is a diagonal sequence of one player's tokens of length len.
   If one is found, it returns the coordinates of the first token, as well as the direction
   of the sequence, in the form:

   -> returnArray[] = {x, y, dir, player}

   If no such sequence is found, returns NULL
*/
int* detectDiagLine(int player, int len, int** board) {
  int x, y, count;
  int coordAndDir[4];

  for (y = 0; y <= Y_DIM - len; y++) {                          //iterate through all of bottom left corner of grid
    for (x = 0; x <= X_DIM - len; x++) {
      for (count = 0; count < len; count++) {                   //count that you have a sequence of length len
        if (board[y + count][x + count] != player)
          break;     //you're not on the right track, try a different sequence
      }
      if (count == len) {
        //we found a sequence of length len, so we return coords/direction
        coordAndDir[0] = x; coordAndDir[1] = y; coordAndDir[2] = UP_LEFT; coordAndDir[3] = player;
        return coordAndDir;
      }
    }
  }
  for (y = len - 1; y < Y_DIM; y++) {                           //iterate through all of top left corner of grid
    for (x = 0; x <= X_DIM - len; x++) {
      for (count = 0; count < len; count++) {                   //count that you have a sequence of length len
        if (board[y - count][x + count] != player)
          break;     //you're not on the right track, try a different sequence
      }
      if (count == len) {
        //we found a sequence of length len, so we return coords/direction
        coordAndDir[0] = x; coordAndDir[1] = y; coordAndDir[2] = DOWN_LEFT; coordAndDir[3] = player;
        return coordAndDir;
      }
    }
  }
  return NULL;
}

/*
   Returns the colour of the player
*/
uint32_t getPlayerColour(int player) {
  if (player == P1)
    return p1_colour;
  else if (player == P2)
    return p2_colour;
  else
    return off;
}

int calculateLedPosition(int x, int y) {
  int led = 8 * y + x;
  return led;
}

