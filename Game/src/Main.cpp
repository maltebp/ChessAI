#include "GameController.h"
#include "TUIPlayer.h"
#include "AIPlayerController.h"



int main(int argc, char** argv) {

    TUIPlayer localPlayer1;
    TUIPlayer localPlayer2;
    AIPlayerController ourEngine1(5000, true);
    AIPlayerController ourEngine2(5000, true);
    
    State startState = State::createDefault();

    GameController game = GameController(localPlayer1, ourEngine2);
    game.start(startState);

    return 0;
}
