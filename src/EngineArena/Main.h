#pragma once

#include <string>

#include "AIPlayerController.h"
#include "EngineArenaController.h"
#include "State.h"
#include "UCIEngine.h"
#include "MoveUtil.h"


int main(int argc, char* argv[]) {

    

    std::string enginePath = "C:\\Users\\Malte\\Projects\\ChessAI\\resources\\stockfish\\stockfish_14_x64_popcnt.exe";

    fs::path outputPath = fs::current_path() / "engine_arena_test";

    State state = State::createDefault();

    UCIEngine stockfish1 = UCIEngine("Stockfish 14", enginePath, 1000);     
    // UCIEngine stockfish2 = UCIEngine("Stockfish 14", enginePath, 1000);
    AIPlayerController ourEngine = AIPlayerController(5);
    
    EngineArenaController controller = {
        outputPath,
        2,
        ourEngine,
        stockfish1
    };

    controller.start();
    
    // while(true) {
    //     std::string input;
    //     std::getline(std::cin, input);
    //     if( input == "end" ) break;
        
    //     if( input == "search" ) {
    //         Move move = engine.getMove(state, {});
    //         std::cout << "Best move: " << move;
    //         state = MoveUtil::executeMove(state, move);
    //     }

    // }


}