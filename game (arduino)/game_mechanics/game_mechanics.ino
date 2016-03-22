// ==== Constants ====

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
}

/*
 *  Displays a welcome/startup pattern on the LEDs
 */
void startupLEDSequence() {
  Serial.println("Startup");
  //!!!
}

