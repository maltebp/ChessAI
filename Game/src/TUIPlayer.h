#pragma once

#include "IPlayerController.h"


class TUIPlayer : public IPlayerController {
public:

    std::string getName() override;

    void start(std::ostream* outputStream, std::ostream* errorStream) override;

    TurnResult giveTurn(const GameInfo& gameInfo) override;

};