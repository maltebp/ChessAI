#pragma once

#include <string>

#include "State.h"
#include "UCIEngine.h"


int main(int argc, char* argv[]) {

    std::string enginePath = "";

    State state = State::createDefault();

    UCIEngine engine = UCIEngine(enginePath);

    Move move = engine.getMove(state);

}