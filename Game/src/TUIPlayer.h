#pragma once

#include "IPlayerController.h"


class TUIPlayer : public IPlayerController {
public:

    Move getMove(const State& state, const std::vector<Move>& initialMoves, const Move& lastMove) override;

};