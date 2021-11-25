#include "GameController.h"
#include "TUIPlayer.h"
#include "AIPlayerController.h"



int main(int argc, char** argv) {

    TUIPlayer localPlayer1;
    TUIPlayer localPlayer2;
    AIPlayerController ourEngine1(9900, true);
    //AIPlayerController ourEngine2(5000, true);
    
    State startState = State::createDefault();

    GameController game = GameController(ourEngine1, localPlayer1);
    game.clearScreen = false;
    game.printValidMoves = false;
    game.start(startState);

    return 0;
}
