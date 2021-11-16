#pragma once

#include "IPlayerController.h"


class GameController {
public:

    GameController(IPlayerController& whitePlayer, IPlayerController& blackPlayer)
        :   whitePlayer(whitePlayer),
            blackPlayer(blackPlayer)
    { }

    void start(State state = State::createDefault());

public:

    bool clearScreen = true;   

    bool printValidMoves = false;

private:

    IPlayerController& whitePlayer;

    IPlayerController& blackPlayer;

    State state;

};