#pragma once

#include <vector>

#include "Move.h"
#include "State.h"
#include "PlayerController.h"
#include "MinMax.h"

class AIPlayerController : public PlayerController{
    int searchDepth = 2;

public:
    Move getMove(const State& state, const std::vector<Move>& initialMoves) {
        auto [move, score] = MinMaxSearcher::search(state, searchDepth, -1000, 1000);
        return move;
    }

};