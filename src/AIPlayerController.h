#pragma once

#include <vector>
#include <climits>

#include "IPlayerController.h"
#include "MinMax.h"

class AIPlayerController : public IPlayerController {
public:


    AIPlayerController(int searchDepth)
        :   searchDepth(searchDepth)
    { }


    Move getMove(const State& state, const std::vector<Move>& initialMoves) {
        auto [move, score] = MinMaxSearcher::search(state, searchDepth, INT32_MIN, INT32_MAX );
        return move;
    }


private:

    int searchDepth;

};