#pragma once

#include <vector>

#include "IPlayerController.h"
#include "MinMax.h"

class AIPlayerController : public IPlayerController {
    
    int searchDepth = 6;

public:

    Move getMove(const State& state, const std::vector<Move>& initialMoves) {
        auto [move, score] = MinMaxSearcher::search(state, searchDepth, -1000, 1000);
        return move;
    }

};