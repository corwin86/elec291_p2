// ==== Constants ====

/* ---- hardware dependent ---- */
const int MAX_BOARD_X = 8,
          MAX_BOARD_Y = 8;
/* -- end hardware dependent -- */

/* ---- game ids ---- */
const int CONNECT_FOUR = 0;
/* -- end game ids -- */

/* ---- game parameters ---- */
const int P1 = 1,          //
          P2 = 2,          // tokens used on board
          EMPTY_CELL = 0;  //
/* -- end game parameters -- */

/* ---- error states ---- */
const int FN_SUCCESS = 1,
          ERR_GENERIC_FAIL = -1,
          ERR_INVALID_GAMEID = -2;
/* -- end error states -- */

// == End Constants ==

void setup() {
  Serial.begin(9600);

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
  // ==== Connect Four Parameters ====
  const int x_dim = 7,
            y_dim = 6;
  // == End Connect Four Parameters ==

  // Create board
  int **board = createBoard(x_dim, y_dim);

  // initialize board as empty (no tokens until users start dropping)
  int x, y;
  for (y = 0; y < y_dim; y++) {
    for (x = 0; x < x_dim; x++) {
      board[x][y] = x_dim * x + y; //set for debugging !!! EMPTY_CELL;
    }
  }
  
  while(!gameOver_connect4(x_dim, y_dim, board)) {
    //!!! game logic
  }

  printBoard(x_dim, y_dim, board);

  // free malloc'd memory
  freeBoard(x_dim, board);
  
  return 1; //game completed successfully
}

int gameOver_connect4(int x_dim, int y_dim, int **board) {
  return winningPlayer_connect4(x_dim, y_dim, board) > 0 || boardFull(x_dim, y_dim, board);
}

/*
 *  Return player ID of winning player, 0 if none
 */ 
int winningPlayer_connect4(int x_dim, int y_dim, int **board) {
  //!!! traverse board looking for 4 in a row
  return 0;
}

/*
 *  Returns 0 if board contains 1+ empty cells
 */
int boardFull(int x_dim, int y_dim, int **board) {
  //!!! traverse board, if has empty cell return false
  return 0;
}

/*
 *  Allocate a x_dim by y_dim board to be used in a game
 */
int ** createBoard(int x_dim, int y_dim) {
  int **board = (int **)malloc(y_dim * sizeof(int *));
  int x, y; // loop indeces
  for (y = 0; y < y_dim; y++) {
    board[y] = (int *)malloc(x_dim * sizeof(int));
  }

  return board;
}

/*
 *  Free an x_dim by (irrelevant) board's allocated memory
 */
void freeBoard(int x_dim, int **board) {
  int x, y;
  for (x = 0; x < x_dim; x++) {
    free(board[x]);
  }
  free(board);
}

/*
 *  Output current state of a board
 */
void printBoard(int x_dim, int y_dim, int **board) {
  //!!! to be changed to print to LEDs
  //    uses serial for dev
  int x, y;
  for (y = 0; y < y_dim; y++) {
    for (x = 0; x < x_dim; x++) {
      Serial.print(board[x][y]); Serial.print("\t");
    }
    Serial.println();
  }
}

/*
 *  Displays a welcome/startup pattern on the LEDs
 */
void startupLEDSequence() {
  Serial.println("Startup");
  //!!!
}

