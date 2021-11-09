#include "GameController.h"
#include "TUIPlayer.h"
#include "AIPlayerController.h"


int main(int argc, char** argv) {

    TUIPlayer localPlayer1;
    TUIPlayer localPlayer2;
    AIPlayerController ourEngine1(5);
    AIPlayerController ourEngine2(5);

    //State startState = "2k4r/1pp2ppp/p1n5/4p2b/1bP5/NP2P2P/PB2P3/2RKqBNR w - - 1 19";
    State startState = State::createDefault();
    //GameController game = GameController(localPlayer1, ourEngine1);
    GameController game = GameController(ourEngine1, localPlayer1);
    game.printValidMoves = false;
    game.start(startState);

    return 0;
}
