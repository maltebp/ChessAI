#pragma once

#include <vector>

#include "IPlayerController.h"
#include "MinMax.h"

class AIPlayerController : public IPlayerController {
    
    int searchDepth = 4;

public:

    Move getMove(const State& state, const std::vector<Move>& initialMoves, const Move& lastMove) {
        //TODO: check state tur tur %2 er hvid overf�r last white move eller last black move
        /*TODO: first take lastMove. If last move is NULL start from white tree 
        if last move isn't NULL, last move is a book move and turn is white insert move into white tree and return a response from the next nodes
        if last move isn't NULL, last move is a book move and turn is black insert move into black tree and return a response from the next nodes
        */ 
        // ACTUALLY all trees should work for both players.
        std::vector<Move> moves = MinMaxSearcher::search(state, searchDepth);
        return moves.size() > 0 ? moves[0] : Move();
    }

};