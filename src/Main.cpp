#ifndef UNIT_TEST

#include "GameController.h"
#include "TUIPlayer.h"
#include "AIPlayerController.h"


int main(int argc, char** argv) {

    Move move = { {7,6}, {7,7}, PieceType::QUEEN };
    std::cout << move << std::endl;

    move = { {7,1}, {7,0}, PieceType::QUEEN };
    std::cout << move << std::endl;

    
    Move::fromAlgebraicNotation("e2e1q", move);
    std::cout << move << std::endl;


    TUIPlayer localPlayer1;
    TUIPlayer localPlayer2;
    AIPlayerController ourEngine1;
    AIPlayerController ourEngine2;

    State startState = "2k4r/1pp2ppp/p1n5/4p2b/1bP5/NP2P2P/PB2P3/2RKqBNR w - - 1 19";

    GameController game = GameController(ourEngine1, ourEngine2);
    game.start(startState);

    return 0;
}



#else

#define CATCH_CONFIG_RUNNER
#include "external/catch.hpp"


// Test files --------

#include "UnitTests/PawnMoves.h"
#include "UnitTests/CastlingMoves.h"
#include "UnitTests/PromotionMoves.h"
#include "UnitTests/FEN.h"
#include "UnitTests/CheckMate.h"

// -------------------


int main(int argc, const char** argv) {

    std::vector<const char*> args;
    for(int i=0; i<argc; i++) {
        args.push_back(argv[i]);
    }
	
	// Test arguments: (i.e. tag on test cases to run)
	args.push_back("-b"); // Break when assertion failed

    return Catch::Session().run((int)args.size(), args.data());
}


#endif // UNIT_TEST