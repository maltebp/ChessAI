#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include "Move.h"
#include "State.h"
#include "MoveUtil.h"


class IPlayerController {
public:

    struct GameInfo;

    struct TurnResult;

public:

    virtual std::string getName() = 0;

    virtual void start(std::ostream* outputStream, std::ostream* errorStream) = 0;

    virtual TurnResult giveTurn(const GameInfo& gameInfo) = 0;

public:

    
    struct GameInfo {
    public:

        Move getLastWhiteMove() const {
            if( previousMoves.size() == 0 ) return Move();
            size_t lastTurnIndex = previousMoves.size() - (currentState.isWhitesTurn() ? 2 : 1);
            bool outOfRange = lastTurnIndex < 0 || lastTurnIndex >= previousMoves.size();
            if (outOfRange) {
                return Move();
            }
            return previousMoves[lastTurnIndex];
        }


        Move getLastBlackMove() const {
            if( previousMoves.size() < 2 ) return Move();
            size_t lastTurnIndex = previousMoves.size() - (currentState.isWhitesTurn() ? 1 : 2);
            bool outOfRange = lastTurnIndex < 0 || lastTurnIndex >= previousMoves.size();
            if (outOfRange) {
                return Move();
            }
            return previousMoves[lastTurnIndex];
        }

        Piece getLastMovedWhitePiece() const {
            if( previousMoves.size() == 0 ) return Piece();
            size_t lastTurnIndex = previousMoves.size() - (currentState.isWhitesTurn() ? 2 : 1);
            bool outOfRange = lastTurnIndex < 0 || lastTurnIndex >= previousMoves.size();
            if (outOfRange) {
                return Piece();
            }

            const Move& lastMove = previousMoves[lastTurnIndex];
            const State& lastState = previousStates[lastTurnIndex];
            return lastState[lastMove.fromField];
        }

        Piece getLastMovedBlackPiece() const {
            if( previousMoves.size() == 0 ) return Piece();
            size_t lastTurnIndex = previousMoves.size() - (currentState.isWhitesTurn() ? 1 : 2);
            bool outOfRange = lastTurnIndex < 0 || lastTurnIndex >= previousMoves.size();
            if (outOfRange) {
                return Piece();
            }

            const Move& lastMove = previousMoves[lastTurnIndex];
            const State& lastState = previousStates[lastTurnIndex];
            return lastState[lastMove.fromField];
        }

    public:

        State currentState;

        MoveUtil::GenerationList validMoves;

        std::vector<Move> previousMoves;

        std::vector<State> previousStates;

    };


    struct TurnResult {
    public:

        TurnResult() {}

        TurnResult(Move move)
            :   chosenMove(move)
        {}

    public:
    
        Move chosenMove = Move();

        // If not 0, the chosen move is ignored
        unsigned int numStatesToRevert = 0;

    };


};