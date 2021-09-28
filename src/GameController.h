#pragma once

#include "PlayerController.h"


class GameController {
public:

    GameController(PlayerController& whitePlayer, PlayerController& blackPlayer)
        :   whitePlayer(whitePlayer),
            blackPlayer(blackPlayer)
    { }

    void start();

private:

    PlayerController& whitePlayer;

    PlayerController& blackPlayer;

    State state;

};