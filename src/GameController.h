#pragma once

#include "IPlayerController.h"


class GameController {
public:

    GameController(IPlayerController& whitePlayer, IPlayerController& blackPlayer)
        :   whitePlayer(whitePlayer),
            blackPlayer(blackPlayer)
    { }

    void start();

private:

    IPlayerController& whitePlayer;

    IPlayerController& blackPlayer;

    State state;

};