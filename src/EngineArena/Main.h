#pragma once

#include <string>

#include "State.h"
#include "UCIEngine.h"
#include "MoveUtil.h"


int main(int argc, char* argv[]) {

    std::string enginePath = "C:\\Users\\Malte\\Projects\\ChessAI\\resources\\stockfish\\stockfish_14_x64_popcnt.exe";

    State state = State::createDefault();

    UCIEngine engine = UCIEngine(enginePath);
    
    while(true) {
        std::string input;
        std::getline(std::cin, input);
        if( input == "end" ) break;
        
        if( input == "search" ) {
            Move move = engine.getMove(state, {});
            std::cout << "Best move: " << move;
            state = MoveUtil::executeMove(state, move);
        }

    }


}