#include "GameController.h"

#include <cassert>

#include "MoveUtil.h"


void GameController::start() {

    state = State::createDefault();

    bool running = true;
    while(running) {

        PlayerController* currentPlayer = state.turn == 0 ? whitePlayer : blackPlayer;
        
        // TODO: Either pass vector to getAllMoves or remove this line
        std::vector<Move> initialMoves = MoveUtil::getAllMoves(state);

        Move move = currentPlayer->getMove(state, initialMoves);

        auto iterator = std::find(initialMoves.begin(), initialMoves.end(), move);
        assert(iterator != initialMoves.end());

        // Execute move

        state.turn = state.turn + 1 % 2;

    }

}