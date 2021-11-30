#include "GameController.h"
#include "TUIPlayer.h"
#include "AIPlayerController.h"



int main(int argc, char** argv) {

    TUIPlayer localPlayer1;
    TUIPlayer localPlayer2;
    AIPlayerController ourEngine1(9900, true);
    //AIPlayerController ourEngine2(5000, true);
    
    State startState;
    startState.board[0][0] = { PieceColor::WHITE, PieceType::KING };
    startState.board[7][7] = { PieceColor::BLACK, PieceType::KING };
    startState.board[0][6] = { PieceColor::WHITE, PieceType::PAWN };

    //State startState = State::createDefault();

    GameController game = GameController(localPlayer1, ourEngine1);
    game.clearScreen = false;
    game.printValidMoves = false;
    game.start(startState);

    return 0;
}
