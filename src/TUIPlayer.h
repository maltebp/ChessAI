#pragma once

#include "PlayerController.h"


class TUIPlayer : public PlayerController {
public:

    Move getMove(const State& state, const std::vector<Move>& initialMoves) override;

};