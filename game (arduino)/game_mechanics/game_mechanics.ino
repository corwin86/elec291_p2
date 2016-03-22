// ==== Constants ====

/* ---- hardware dependent ---- */
const int MAX_BOARD_X = 8,
          MAX_BOARD_Y = 8;
/* -- end hardware dependent -- */

/* ---- game ids ---- */
const int CONNECT_FOUR = 0;
/* -- end game ids -- */

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
  if (e == ERR_INVALID_GAMEID) {
    Serial.print("** ERR - INVALID GAME ID: "); Serial.println(gameId);
  } else if (e == ERR_GENERIC_FAIL) {
    Serial.println("** ERR - GENERIC FAIL: runGame(gameId) failed");
  }
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
      }
    //!!! add cases per game
    default : {
        // gameId did not match any library games
        return ERR_INVALID_GAMEID;
      }
  }
}

int playConnectFour() {
  //!!! gameplay logic
  // ==== Connect Four Parameters ====
  const int x_dim = 7, y_dim = 7;
  // == End Connect Four Parameters ==
  
  // Create board
  int **board = createBoard(x_dim, y_dim);
  
  // initialize as empty
  int x, y;
  for(x = 0; x < x_dim; x++) {
    for(y = 0; y < y_dim; y++) {
      board[x][y] = x * x_dim + y; //!!! initialize to nonsense for devel testing
    }
  }
  
  printBoard(x_dim, y_dim, board);
  
  // free malloc'd memory
  freeBoard(x_dim, board);
}

/*
 *  Allocate a x_dim by y_dim board to be used in a game
 */
int ** createBoard(int x_dim, int y_dim) {
  int **board = (int **)malloc(x_dim * sizeof(int *));
  int x, y; // loop indeces
  for (x = 0; x < x_dim; x++) {
    board[x] = (int *)malloc(x_dim * sizeof(int));
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
  int y, x;
  for(y = 0; y < y_dim; y++){
    for(x = 0; x < x_dim; x++) {
      Serial.print(board[y][x]);Serial.print("\t");
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

