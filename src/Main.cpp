
#include "GameController.h"
#include "TUIPlayer.h"

int main(int argc, char** argv) {

    TUIPlayer whitePlayer;
    TUIPlayer blackPlayer;

    GameController game = GameController(whitePlayer, blackPlayer);
    game.start();

    return 0;
}