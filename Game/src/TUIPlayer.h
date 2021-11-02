#pragma once

#include "IPlayerController.h"


class TUIPlayer : public IPlayerController {
public:

    std::string getName() override;

    void start(std::ostream* outputStream, std::ostream* errorStream) override;

    Move getMove(const State& state, const MoveUtil::GenerationList& initialMoves, const Move& lastMove) override;

};