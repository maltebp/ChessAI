
#define ENGINE_ARENA
//#define UNIT_TEST

#ifdef UNIT_TEST


#define CATCH_CONFIG_RUNNER
#include "external/catch.hpp"


// Test files --------

#include "UnitTests/PawnMoves.h"
#include "UnitTests/CastlingMoves.h"
#include "UnitTests/PromotionMoves.h"
#include "UnitTests/FEN.h"

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


#elif defined ENGINE_ARENA

#include "EngineArena/Main.h"

#else

#include "GameController.h"
#include "TUIPlayer.h"
#include "AIPlayerController.h"


int main(int argc, char** argv) {

    TUIPlayer whitePlayer;
    AIPlayerController blackPlayer;

    GameController game = GameController(whitePlayer, blackPlayer);
    game.start();

    return 0;
}

#endif