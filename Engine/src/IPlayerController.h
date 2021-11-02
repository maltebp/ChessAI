#pragma once

#include <iostream>
#include <vector>

#include "Move.h"
#include "State.h"
#include "MoveUtil.h"


class IPlayerController {
public:

    virtual std::string getName() = 0;

    virtual void start(std::ostream* outputStream, std::ostream* errorStream) = 0;

    virtual Move getMove(const State& state, const MoveUtil::GenerationList& validMoves, const Move& lastMove) = 0;

};