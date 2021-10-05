#pragma once

#include <vector>

#include "Move.h"
#include "State.h"

class IPlayerController {
public:

    virtual Move getMove(const State& state, const std::vector<Move>& initialMoves) = 0;

};