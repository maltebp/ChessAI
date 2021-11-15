#define CATCH_CONFIG_RUNNER
#include "external/catch.hpp"


// Test files --------

#include "PawnMoves.h"
#include "CastlingMoves.h"
#include "PromotionMoves.h"
#include "FEN.h"
#include "CheckMate.h"
#include "ZobristTest.h"
#include "TranspositionTest.h"

// -------------------


int main(int argc, const char** argv) {

    std::vector<const char*> args;
    for(int i=0; i<argc; i++) {
        args.push_back(argv[i]);
    }
	
	// Test arguments: (i.e. tag on test cases to run)
	//args.push_back("-b"); // Break when assertion failed

    return Catch::Session().run((int)args.size(), args.data());
}
