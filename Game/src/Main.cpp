#include "GameController.h"
#include "TUIPlayer.h"
#include "AIPlayerController.h"



int main(int argc, char** argv) {

    TUIPlayer localPlayer1;
    TUIPlayer localPlayer2;
    AIPlayerController ourEngine1(4);
    AIPlayerController ourEngine2(4);
    
    State startState = State::createDefault();

    GameController game = GameController(ourEngine1, ourEngine2);
    game.start(startState);

    return 0;
}
