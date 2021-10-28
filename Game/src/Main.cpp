#include "GameController.h"
#include "TUIPlayer.h"
#include "AIPlayerController.h"


int main(int argc, char** argv) {

    TUIPlayer localPlayer1;
    TUIPlayer localPlayer2;
    AIPlayerController ourEngine1;
    AIPlayerController ourEngine2;

    State startState = "2k4r/1pp2ppp/p1n5/4p2b/1bP5/NP2P2P/PB2P3/2RKqBNR w - - 1 19";

    GameController game = GameController(ourEngine1, ourEngine2);
    game.start(startState);

    return 0;
}