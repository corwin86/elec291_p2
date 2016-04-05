#include <Adafruit_NeoPixel.h>
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "utility/debug.h"
#include "utility/socket.h"

/********************SERVER DEFINES********************/
// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                         SPI_CLOCK_DIVIDER); // you can change this clock speed

#define WLAN_SSID       "SM-N900W8 0389"   // cannot be longer than 32 characters!
#define WLAN_PASS       "asdfghjk"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define LISTEN_PORT           80      // What TCP port to listen on for connections.  
// The HTTP protocol uses port 80 by default.

#define MAX_ACTION            10      // Maximum length of the HTTP action that can be parsed.

#define MAX_PATH              0      // Maximum length of the HTTP request path that can be parsed.
// There isn't much memory available so keep this short!

#define BUFFER_SIZE           MAX_ACTION + MAX_PATH + 20  // Size of buffer for incoming request data.
// Since only the first line is parsed this
// needs to be as large as the maximum action
// and path plus a little for whitespace and
// HTTP version.

#define TIMEOUT_MS            500    // Amount of time in milliseconds to wait for
// an incoming request to finish.  Don't set this
// too high or your server could be slow to respond.

/********************END SERVER DEFINES********************/

/********************GAME DEFINES********************/
#define LED_PIN 2
#define DEBUG 1
/*******************END GAME DEFINES*****************/



Adafruit_NeoPixel strip = Adafruit_NeoPixel(64, LED_PIN, NEO_GRB + NEO_KHZ800);

/*colours*/
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t magenta = strip.Color(255, 0, 255);
uint32_t cyan = strip.Color(0, 255, 255);
uint32_t yellow = strip.Color(255, 255, 0);

uint32_t gold = strip.Color(255, 215, 0);
uint32_t webworkGreen = strip.Color(118, 255, 122);
uint32_t white = strip.Color(255, 255, 255);
uint32_t off = strip.Color(0, 0, 0);

uint32_t victoryBlink;

// ==== Constants ====

/* ---- hardware dependent ---- */
const uint8_t LED_BRIGHTNESS = 25;
/* -- end hardware dependent -- */

/* ---- game ids ---- */
const int8_t CONNECT_FOUR = 0;
/* -- end game ids -- */

/* ---- game parameters ---- */
const int8_t P1 = 1,
             P2 = 2,          // tokens used on board
             AI = 2,
             EMPTY_CELL = 0;
const int8_t AI_VS_AI = 0,
             SINGLE_PLAYER = 1,  //game modes
             MULTIPLAYER = 2,
             UNINITIALIZED = -1;
const int8_t X_DIM = 8,
             Y_DIM = 8,
             SEQ_LENGTH = 4;
const int BAD_MOVE = -1000,
          GOOD_MOVE = 1000,
          IMPOSSIBLE_MOVE = -11111;
const int8_t TOKEN_DROP_SPEED = 35; // millis delay for token dropping
/* -- end game parameters -- */

/* ---- directions ---- */
const int8_t UP = 1,
             LEFT = 2,
             UP_LEFT = 3,
             DOWN_LEFT = 4;
/* -- end directions -- */

// == End Constants ==


// ==== Global Variables ====

/* ---- player info ---- */
uint32_t p1_colour = off,
         p2_colour = off;
int8_t      turns;
bool     gameOver,
         validMove;
/* -- end player info -- */

/* ---- gane parameters ---- */
int8_t   difficulty = 2,         //AI recursion depth
         game_mode = UNINITIALIZED;   //starts in 1p mode
/* -- end gane parameters -- */

/* ---- server variables ---- */
Adafruit_CC3000_Server httpServer(LISTEN_PORT);
uint8_t buffer[BUFFER_SIZE + 1];
int     bufindex = 0;
char    action[MAX_ACTION + 1];
char    path[MAX_PATH + 1];
/* -- end server variables -- */

// == End Global Variables ==

void setup() {
  /************ SERVER SETUP **************/
  Serial.begin(115200);

  //can be safely removed later
  //Serial.println(F("Hello, CC3000!\n"));
  //Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);

  // Initialise the module
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    //Serial.println(F("Couldn't begin()! Check your wiring?"));
    while (1);
  }

  //attempt to connect to wifi network
  //Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID);
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    //Serial.println(F("Failed!"));
    while (1);
  }

  //can be safely removed later
  //Serial.println(F("Connected!"));
  //Serial.println(F("Request DHCP"));

  while (!cc3000.checkDHCP())
  {
    delay(100);
  }

  // Start listening for connections
  httpServer.begin();
  Serial.println(F("Wifi setup finished."));
  /************END SERVER SETUP**************/

  randomSeed(analogRead(4));

  //buffer for parsing
  char appBuffer[2] = {0};

  //initialize LED array
  strip.begin();
  strip.setBrightness(LED_BRIGHTNESS);
  strip.show();

}

/*
   Runs the game specified by gameID
*/
void loop() {
  int8_t gameId = 0;
  runGame(gameId);
}

//gameID's were added for future support of other games, but there was no time to implement them.
int8_t runGame(int8_t gameId) {
  switch (gameId) {
    case CONNECT_FOUR : {
        playConnectFour();
        break;
      }
    default : {
        // gameId did not match any library games
        return 0;
      }
  }
}

/*
   Includes overarching game logic for Connect 4:
   - takes input from the app to initialize (mode, p1 colour, p2 colour), then
   - while the game isn't over (no wins or stalemates), takes moves from each player, alternating
     (either a player through the app or an AI)
   - if one player has a line of 4 or 5 (or any predetermined sequence length), flash the winning line,
     drop out the bottom of the board, and start the game over

   Returns: 1 when game finished
*/
int8_t playConnectFour() {
  gameOver = false;
  connect4Setup();
  // Create 64x64 board
  int8_t **board = createBoard();
  // initialize board as empty (no tokens until users start dropping)
  int8_t x, y;
  for (y = 0; y < Y_DIM; y++) {
    for (x = 0; x < X_DIM; x++) {
      board[y][x] = EMPTY_CELL;
    }
  }
  printBoard(board);

  //---- gameplay ----
  int8_t cur_player = P1; //cur_player is one of {P1, P2}
  turns = 0;
  while (!gameOver) {
    int8_t col;
    //do {
    // -- take input --
    // use serial for debug
    if (game_mode == SINGLE_PLAYER) {
      if (cur_player == P1) {
        col = getMoveFromServer();
        if (col == -1)
          continue; //if connection error, try turn again
      }
      else
        col = aiNextMove(board, AI, P1);
    }
    else if (game_mode == MULTIPLAYER) {
      col = getMoveFromServer();
    }
    else {      //AI mode
      int8_t other_player = cur_player == P1 ? P2 : P1;
      col = aiNextMove(board, cur_player, other_player);
    }
    validMove = dropToken_connect4(board, col, cur_player, 1);

    printBoard(board);

    if (validMove) {
      cur_player = cur_player == P1 ? P2 : P1; //change turns
      turns++;
      gameOver = gameOver_connect4(board);
    }
  }
  //-- end gameplay --

  connect4Cascade(board); //GUI feature indicates game end, ***CLEARS BOARD***

  // free malloc'd memory
  freeBoard(board);

  return 1; //game completed successfully
}

/*
    Places a player token in a column on the board

    Param: col - the column to place the token in
    Param: token - the token to place (P1 or P2)
    Param: animated - 1 to show the animation on the board, 0 to not print to the board
    Modifies: board - the internal board state
    Returns: 1 if token successfully dropped, 0 if invalid move
*/
int8_t dropToken_connect4(int8_t **board, int8_t col, int8_t token, int8_t animated) {
  int8_t y = -1;
  while (board[y + 1][col] == EMPTY_CELL && y + 1 < Y_DIM) {
    y++;
  }
  if (y == -1) { //no empty cells in column col
    return 0;
  }
  if (animated) {
    uint32_t colour = getPlayerColour(token);
    int8_t i = -2;
    while (++i < y) {
      printCell(col, i, off, 0);
      printCell(col, i + 1, colour, 0);
      strip.show();
      delay(TOKEN_DROP_SPEED);
    }
  }
  board[y][col] = token; //place token
  return 1; //successfully dropped token
}

/*
    Checks the board if the game is over

    Param: board - the internal board state
    Returns: true if a player has won, or a draw is reached, false if otherwise
*/
int8_t gameOver_connect4(int8_t **board) {
  return winningPlayer_connect4(board, 1) > 0 || boardFull();
}

/*
    Graphical sequence replicating pulling bottom out of connect 4 board.

    Modifies: board - replaces all user tokens with EMPTY_CELL
*/
void connect4Cascade(int8_t **board) {
  int8_t x, y, count;
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
    Checks the board if one player has won.

    Param: board - the internal board state
    Param: flash - if 1, the winning tiles flash to a colour not currently belonging
      to either player 3 times, if 0 there is no animation
    Returns: player ID of winning player, 0 if none
*/
int8_t winningPlayer_connect4(int8_t **board, int8_t flash) {
  int8_t winData[4] = {0};
  int8_t flashes = 3 * 2;
  detectDiagLine(P1, SEQ_LENGTH, board, winData);
  if (winData[0] == -1) {
    detectHorizLine(P1, SEQ_LENGTH, board, winData);
    if (winData[0] == -1) {
      detectVertLine(P1, SEQ_LENGTH, board, winData);
      if (winData[0] == -1) {
        detectDiagLine(P2, SEQ_LENGTH, board, winData);
        if (winData[0] == -1) {
          detectHorizLine(P2, SEQ_LENGTH, board, winData);
          if (winData[0] == -1) {
            detectVertLine(P2, SEQ_LENGTH, board, winData);
            if (winData[0] == -1) {
              return 0;
            }
          }
        }
      }
    }
  }

  //flashes 3 times
  if (flash) {
    printBoard(board);
    delay(500);
    int8_t x = winData[0], y = winData[1];
    if (p1_colour != green) {
      if (p2_colour != green) {
        victoryBlink = green;  //!green vs !green
      } else {
        if (p1_colour != red) {
          victoryBlink = red;  //!red vs green
        } else {
          victoryBlink = blue; //red vs green
        }
      }
    } else {
      if (p2_colour != red) {
        victoryBlink = red;  //green vs !red
      } else {
        victoryBlink = blue; //green vs red
      }
    }

    while (flashes-- > 0) {
      int8_t i = -1;
      while (i++ < SEQ_LENGTH - 1) {
        uint32_t colour = flashes % 2 ? victoryBlink : getPlayerColour(winData[3]);
        switch (winData[2]) {
          case UP:
            printCell(x    , y + i, colour, 0); break;
          case LEFT:
            printCell(x + i, y    , colour, 0); break;
          case UP_LEFT:
            printCell(x + i, y + i, colour, 0); break;
          case DOWN_LEFT:
            printCell(x + i, y - i, colour, 0); break;
          default:
            break;
        }
      }
      strip.show();
      delay(500);
    }
  }
  return winData[3];
}

/*
  NEXT 3 FUNCTIONS:
  Detects if there is a vertical, horizontal, or diagonal sequence of one player's tokens of length len.

  Param: board - the internal board state
  Param: len - the length of the sequence to look for
  Param: player - the players tokens to check
  Modifies: coordAndDir - if a line is found, changes the fields to the coordinates of the first token,
    the direction of the sequence, and the winning player, in the form:
    -> coordAndDir[] = {x, y, dir, player}
    If no such sequence is found, sets all fields of coordAndDir to -1
*/
void detectVertLine(int8_t player, int8_t len, int8_t** board, int8_t *coordAndDir) {
  int8_t x, y, count;
  for (y = 0; y <= Y_DIM - len; y++) {                          //iterate through bottom half of grid
    for (x = 0; x < X_DIM; x++) {
      for (count = 0; count < len; count++) {                   //count that you have a sequence of length len
        if (board[y + count][x] != player)
          break;     //you're not on the right track, try a different sequence
      }
      if (count == len) {
        //we found a sequence of length len, so we return coords/direction
        coordAndDir[0] = x; coordAndDir[1] = y; coordAndDir[2] = UP; coordAndDir[3] = player;
        return;
      }
    }
  }
  coordAndDir[0] = -1; coordAndDir[1] = -1; coordAndDir[2] = -1; coordAndDir[3] = -1;
}
void detectHorizLine(int8_t player, int8_t len, int8_t** board, int8_t *coordAndDir) {
  int8_t x, y, count;

  for (x = 0; x <= X_DIM - len; x++) {                          //iterate through left half of grid
    for (y = 0; y < Y_DIM; y++) {
      for (count = 0; count < len; count++) {                   //count that you have a sequence of length len
        if (board[y][x + count] != player)
          break;     //you're not on the right track, try a different sequence
      }
      if (count == len) {
        //we found a sequence of length len, so we return coords/direction
        coordAndDir[0] = x; coordAndDir[1] = y; coordAndDir[2] = LEFT; coordAndDir[3] = player;
        return;
      }
    }
  }
  coordAndDir[0] = -1; coordAndDir[1] = -1; coordAndDir[2] = -1; coordAndDir[3] = -1;
}
void detectDiagLine(int8_t player, int8_t len, int8_t** board, int8_t *coordAndDir) {
  int8_t x, y, count;
  for (y = 0; y <= Y_DIM - len; y++) {                          //iterate through all of bottom left corner of grid
    for (x = 0; x <= X_DIM - len; x++) {
      for (count = 0; count < len; count++) {                   //count that you have a sequence of length len
        if (board[y + count][x + count] != player)
          break;     //you're not on the right track, try a different sequence
      }
      if (count == len) {
        //we found a sequence of length len, so we return coords/direction
        coordAndDir[0] = x; coordAndDir[1] = y; coordAndDir[2] = UP_LEFT; coordAndDir[3] = player;
        return;
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
        return;
      }
    }
  }
  coordAndDir[0] = -1; coordAndDir[1] = -1; coordAndDir[2] = -1; coordAndDir[3] = -1;
}

/*
   Checks if the board is full

   Returns: 1 if the board is full, 0 if not
*/
int8_t boardFull() {
  //if 64 turns have passed, the board is full
  return turns >= X_DIM * Y_DIM;
}

/*
    Determines the next move for AI by calling a recursive search algorithm

    Param: board - the internal board state
    Param: ai_token - the number representing the ai token on the board (P1 or P2)
    Param: player_token - the number representing the player token on the board (P1 or P2)
    Returns: the AI's next move
*/
int8_t aiNextMove(int8_t **board, int8_t ai_token, int8_t player_token) {
  int col_points[X_DIM] = {0};
  aiRecursiveSearch(board, 0, col_points, ai_token, player_token);
  return selectNextMove(col_points);
}

/*
    Searches through possible moves recursively by checking for wins a number of moves in the future (brute force style)

    Param: level - the current depth of the algorithm (how many moves in advance it is currently checking)
    Param: board - the internal board state
    Param: ai_token - the number representing the ai token on the board (P1 or P2)
    Param: player_token - the number representing the player token on the board (P1 or P2)
    Modifies: col_points - the current number of points for each column, changes through recursive calls
*/
void aiRecursiveSearch(int8_t **board, int8_t level, int *col_points, int8_t ai_token, int8_t player_token) {
  level++;
  int8_t ai_col, p_col;
  //note: num points assigned to a column is equal to 1000/level for a future win, -1000/level for a future loss.
  for (ai_col = 0; ai_col < X_DIM; ai_col++) {
    if (board[0][ai_col] != EMPTY_CELL) {
      if (level == 1) {
        col_points[ai_col] = IMPOSSIBLE_MOVE;
      }
      continue;
    }
    dropToken_connect4(board, ai_col, ai_token, 0); //simulate dropping an ai token (no animation)

    if (winningPlayer_connect4(board, 0) == ai_token) {
      col_points[ai_col] += GOOD_MOVE / level;        //future win with this move
    } else {
      for (p_col = 0; p_col < X_DIM; p_col++) {
        if (board[0][p_col] != EMPTY_CELL) {
          continue;
        }
        int8_t p_break = 0;
        dropToken_connect4(board, p_col, player_token, 0); //simulate dropping a player token (no animation)

        if (winningPlayer_connect4(board, 0) == player_token) {
          col_points[ai_col] += BAD_MOVE / ((double) level + 0.5); //future loss with this move
          p_break = 1;
        } else if (level + 1 <= difficulty) {  //decrement level and call again, otherwise start popping tokens and returning
          aiRecursiveSearch(board, level, col_points, ai_token, player_token);
        }
        popToken(board, p_col); //remove temporary board modifications

        if (p_break) {
          break;
        }
      }
    }
    popToken(board, ai_col); //remove temporary board modifications
  }
  level--;
}

/*
   Selects the next move for the ai depending on points for each column

   Param: col_points[] - an array containing the number of points per column
   Returns: the column for the AI's next move
*/
int8_t selectNextMove(int *col_points) {
  int8_t i;
  int8_t next_move = 0, ties = 0;
  for (i = 0; i < X_DIM; i++) { //!!! start at 0
    if (col_points[i] > col_points[next_move]) {
      next_move = i;
      ties = 0;
    } else if (col_points[i] == col_points[next_move]) {
      ties++;
    }
  }

  ties = random(0, ties + 1);

  i = 0;
  while (ties >= 0) {
    if (col_points[i] == col_points[next_move]) {
      ties--;
      next_move = i;
    }
    i++;
    if (i == X_DIM) {
      i = 0;
    }
  }

  return next_move;
}

void popToken(int8_t **board, int8_t col) {
  int8_t y = 0;
  while (board[y][col] == EMPTY_CELL && y < Y_DIM) {
    y++;
  }

  board[y][col] = EMPTY_CELL;
}

/*
    Allocate a x_dim by y_dim board to be used in a game

    Returns: a pointer to the board
*/
int8_t **createBoard() {
  int8_t **board = (int8_t **)malloc(Y_DIM * sizeof(int8_t *));
  int8_t x, y; // loop indeces
  for (y = 0; y < Y_DIM; y++) {
    board[y] = (int8_t *)malloc(X_DIM * sizeof(int8_t));
  }
  return board;
}

/*
    Free an x_dim by (irrelevant) board's allocated memory

    Param: board - the structure to be freed
*/
void freeBoard(int8_t **board) {
  int8_t y;
  for (y = 0; y < Y_DIM; y++) {
    free(board[y]);
  }
  free(board);
}

/*
    Output current state of a board to LED array

    Param: board - the internal board state
*/
void printBoard(int8_t **board) {
  int8_t x, y;
  for (y = 0; y < Y_DIM; y++) {
    for (x = 0; x < X_DIM; x++) {
      printCell(x, y, getPlayerColour(board[y][x]), 0);
    }
  }
  strip.show();
}

/*
   Write to a specific LED.

   Param: x - the x coordinate of the light
   Param: y - the y coordinate of the light
   Param: colour - the colour to change the light to
   Param: show - if 1, turns on the LED
*/
void printCell(int8_t x, int8_t y, uint32_t color, int8_t show) {
  int8_t led = X_DIM * y + x;
  strip.setPixelColor(led, color); //sets colour of first pixel
  if (show) {
    strip.show(); //'pushes' colour data to the strip
  }
}

/*
    Displays a welcome/startup pattern on the LEDs
*/
//void startupLEDSequence() {
//  uint32_t s1 = blue;
//  uint32_t s2 = cyan;
//  uint32_t s3 = green;
//  uint32_t s4 = yellow;
//  uint32_t s;
//  int8_t i, j, k;
//  for (i = 0; i < 3 * 4; i++) {
//    for (j = 0; j < X_DIM; j++) {
//      for (k = 0; k < Y_DIM; k++) {
//        if ((j == 3 || j == 4) && (k == 3 || k == 4)) {
//          s = s1;
//        } else if (((j > 1 && j < X_DIM - 2) && (k == 2 || k == 5)) || ((k > 1 && k < Y_DIM - 2) && (j == 2 || j == 5))) {
//          s = s2;
//        } else if (((j > 0 && j < X_DIM - 1) && (k == 1 || k == 6)) || ((k > 0 && k < Y_DIM - 1) && (j == 1 || j == 6))) {
//          s = s3;
//        } else {
//          s = s4;
//        }
//        printCell(i, j, s, 1);
//      }
//    }
//    s = s1;
//    s1 = s2;
//    s2 = s3;
//    s3 = s4;
//    s4 = s;
//    delay(500);
//  }
//
//  for (i = 0; i < 64; i++) {
//    strip.setPixelColor(i + 3, yellow);
//    strip.setPixelColor(i + 2, green);
//    strip.setPixelColor(i + 1, cyan);
//    strip.setPixelColor(i    , blue);
//    delay(10);
//    strip.show();
//  }
//
//  for (i = 0; i < 64; i++) {
//    strip.setPixelColor(i + 3, yellow);
//    strip.setPixelColor(i + 2, green);
//    strip.setPixelColor(i + 1, cyan);
//    if ((i > 9 && i < 14) || i == 18 || (i > 25 && i < 29) || i == 37 || i == 45 || (i > 49 && i < 53)) {
//      strip.setPixelColor(i, blue);
//    } else {
//      strip.setPixelColor(i, off);
//    }
//
//    delay(10);
//    strip.show();
//  }
//
//  //!!! Wait for mode and colour inputs
//  //  delay(2000); //!!! delete
//
//
//  for (int8_t j = 0; j < X_DIM * Y_DIM; j++) {
//    strip.setPixelColor(j, off);
//    strip.show();
//  }
//}

/*
    Finds the colour of a player.

    Param: player - the player (P1 or P2) to find the colour for
    Returns: the colour of that player
*/
uint32_t getPlayerColour(int8_t player) {
  uint32_t p = player == P1 ? p1_colour : player == P2 ? p2_colour : off;
  return p;
}

/*
   Calculates the position of an LED in the strip.

   Param: x - the x coordinate of the LED
   Param: y - the y coordinate of the LED
   Returns: the position of the LED in the 64 light strip
*/
int8_t calculateLedPosition(int8_t x, int8_t y) {
  return 8 * y + x;
}

//================ SERVER FUNCTIONS ================= //


/*
   Parses a server request.

   Param: client - the client
   Param: parsed - whether the request has been parsed
   Param: endtime - the maximum time to wait
   Returns: 1 if the request was parsed successfully
*/
bool parsingRequest(Adafruit_CC3000_ClientRef client, bool parsed, unsigned long endtime)
{
  while (!parsed && (millis() < endtime) && (bufindex < BUFFER_SIZE)) {
    if (client.available()) {
      buffer[bufindex++] = client.read();
    }
    parsed = parseRequest(buffer, bufindex, action, path);
  }
  return parsed;
}

/*
   Once the request is parsed, determine which type of request and perfrom action accordingly

   Param: client - the client
   Modifies: action - the type of action requested by the client
   Returns: a post read from the client, "ERROR" if no post read
*/
String processRequest(Adafruit_CC3000_ClientRef client, char* action)
{
  String data;
  // RESPONSE FOR POST REQUEST
  if (strcmp(action, "POST") == 0) {
    Serial.println("responding to post request...");
    Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);
    data = respondPost(client);
    //Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);
    Serial.println("responded.");
  }
  else {
    Serial.println(F("NOT A POST!!"));
    // Unsupported action, respond with an HTTP 405 method not allowed error.
    client.fastrprintln(F("HTTP/1.1 200 OK"));
    client.fastrprintln(F(""));
    client.fastrprintln(F("-1"));
  }
  return data;
}

/*
   Sends protocol messages back to the client using protocols and returns the client's post

   Param: client - the client
   Returns: the string read from the client
*/
String respondPost(Adafruit_CC3000_ClientRef client) {
  //Read data from post request body and store info into fields
  String data = "";
  char last_char = '0'; //not a new line
  while (client.available()) {
    char currentChar = client.read();

    data = (String) last_char + (String) currentChar;

    last_char = currentChar;
  } Serial.print("Data recieved: "); Serial.println(data); //can be removed later

  //Send server response back to client
  //  This would be used to send stuff like game state, win alert, etc back to client
  client.fastrprintln(F("HTTP/1.1 200 OK"));
  client.fastrprintln(F("Content-Type: text/plain"));
  client.fastrprintln(F("Connection: close"));
  client.fastrprintln(F("Server: Adafruit CC3000"));

  // Send an empty line to signal start of body.
  client.fastrprintln(F(""));

  if (gameOver) {
    client.fastrprintln("gameover");
  }
  else {
    client.fastrprintln(turns % 2 == 0 ? "1" : "2");
  }
  return data;
}

/*
  Returns true if the buffer contains an HTTP request.  Also returns the request
  path and action strings if the request was parsed.  This does not attempt to
  parse any HTTP headers because there really isn't enough memory to process
  them all.
  HTTP request looks like:
  [method] [path] [version] \r\n
  Header_key_1: Header_value_1 \r\n
  ...
  Header_key_n: Header_value_n \r\n
  \r\n
*/
bool parseRequest(uint8_t* buf, int bufSize, char* action, char* path) {
  // Check if the request ends with \r\n to signal end of first line.
  if (bufSize < 2)
    return false;
  if (buf[bufSize - 2] == '\r' && buf[bufSize - 1] == '\n') {
    parseFirstLine((char*)buf, action, path);
    return true;
  }
  return false;
}

/*
    Parses the action and path from the first line of an HTTP request.

    Modifies: line - the line to parse
    Modifies: action - the action recieved
    Modifies: path - the path to the next word
*/
void parseFirstLine(char* line, char* action, char* path) {
  // Parse first word up to whitespace as action.
  char* lineaction = strtok(line, " ");
  if (lineaction != NULL)
    strncpy(action, lineaction, MAX_ACTION);
  // Parse second word up to whitespace as path.
  char* linepath = strtok(NULL, " ");
  if (linepath != NULL)
    strncpy(path, linepath, MAX_PATH);
}

//================ END SERVER FUNCTIONS ================= //


//================ GAME TO SERVER FUNCTIONS ================= //
/*
    Sets up game parameters from app input

    Returns: 1 if successfully initialized, or 0 if connection error
*/
int8_t connect4Setup() {
  char value;
  char field;
  Serial.println("start game setup");
  boolean start = false;
  while (!start) {
    parseFieldValuePair(listenForInput(), &field, &value);
    if       (field == 'm') {
      game_mode = decodeGameMode(value);
    } else if (field == '1') {
      p1_colour = decodeColour(value);
    } else if (field == '2') {
      p2_colour = decodeColour(value);
    } else if (field == 's') {
      if (game_mode != UNINITIALIZED && p1_colour != off && p2_colour != off)
        start = true; //successfully setup connect 4
    }
    else continue;
  }
  Serial.println("end game setup");
  return 1;
}

/*
    Takes string in, parses into a Field-Value pair into two characters

    Param: in - the input string
    Modifies: field - the field in the string (e.g. mode, colour, etc)
    Modifies: value - the value in the string (e.g. single player, red, etc)
*/
void parseFieldValuePair(String in, char *field, char *value) {
  if (in.equals("ERROR")) {
    return;
  }
  *field = in.charAt(0);
  *value = in.charAt(1);
}

/*
    Take char representation of player colour, return colour representation

    Param: c - the character to read
    Returns: the colour representation, off if wrong character passed
*/
uint32_t decodeColour(char c) {
  if       (c == 'r') {
    return red;
  } else if (c == 'b') {
    return blue;
  } else if (c == 'g') {
    return green;
  } else if (c == 'c') {
    return cyan;
  } else if (c == 'm') {
    return magenta;
  } else if (c == 'y') {
    return yellow;
  } else {
    return off;
  }
}

/*
    Take char representation of game mode, return int representation

    Param: c - the character to read
    Returns: the int representation, UNITITIALIZED if wrong character passed
*/
int8_t decodeGameMode(char c) {
  if       (c == 's') {
    return SINGLE_PLAYER;
  } else if (c == 'm') {
    return MULTIPLAYER;
  } else if (c == 'a') {
    return AI_VS_AI;
  } else {
    return UNINITIALIZED;
  }
}

/*
    Listens and waits for server input from app and returns input read from app.

    Returns: a string read from the server, "ERROR" if no string read
*/
String listenForInput() {
  String data = "ERROR";
  // Declare and wait for client
  bool clientFlag = false;
  do {
    Adafruit_CC3000_ClientRef client = httpServer.available();
    // If client is connected... start processing its request
    if (client) {
      clientFlag = true;
      //can be removed later
      Serial.println(F("Client connected."));
      // Clear the incoming data buffer and point to the beginning of it
      bufindex = 0;
      memset(&buffer, 0, sizeof(buffer));
      memset(&action, 0, sizeof(action));
      memset(&path,   0, sizeof(path));

      // Set a timeout for reading all the incoming data.
      unsigned long endtime = millis() + TIMEOUT_MS;

      // Read all the incoming data until it can be parsed or the timeout expires.
      bool parsed = false;
      parsed = parsingRequest(client, parsed, endtime); //function to parse request

      // Handle the request if it was parsed.
      if (parsed) {
        //can be removed later
#if DEBUG
        //Serial.println(F("Processing request"));
        //Serial.print(F("Action: ")); Serial.println(action);
#endif
        data = processRequest(client, action); //function to process request

        // Wait a short period to make sure the response had time to send before
        // the connection is closed (the CC3000 sends data asyncronously).
        delay(100);

        // Close the connection when done.
        client.close();
        Serial.println(F("Client disconnected"));
      }
    }
  } while (!clientFlag);
  return data;
}

/*
   Reads a move from the server by parsing a number

   Returns: the next move (column) from a player on the server, -1 if no valid move read
*/
int8_t getMoveFromServer() {
  return listenForInput().charAt(1) - '0';
}

