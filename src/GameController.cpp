#include "GameController.h"

#include <cassert>

#include "MoveUtil.h"


void GameController::start() {

    state = State::createDefault();

    bool running = true;
    while(running) {

        IPlayerController& currentPlayer = state.turn == 0 ? whitePlayer : blackPlayer;
        
        // TODO: Either pass vector to getAllMoves or remove this line
        std::vector<Move> availableMoves = MoveUtil::getAllMoves(state);

        Move move = currentPlayer.getMove(state, availableMoves);

        auto iterator = std::find(availableMoves.begin(), availableMoves.end(), move);
        assert(iterator != availableMoves.end());

        // Execute move
        state = MoveUtil::executeMove(state, move);
    }

}