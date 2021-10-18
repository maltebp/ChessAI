#pragma once

#include <string>

#include "State.h"
#include "UCIEngine.h"


int main(int argc, char* argv[]) {

    std::string enginePath = "C:\\Users\\Malte\\Projects\\ChessAI\\resources\\stockfish\\stockfish_14_x64_popcnt.exe";

    State state = State::createDefault();

    UCIEngine engine = UCIEngine(enginePath);

    Move move = engine.getMove(state, {});

}