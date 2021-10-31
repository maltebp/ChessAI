#pragma once

#include <vector>
#include <climits>

#include "IPlayerController.h"
#include "MinMax.h"

class AIPlayerController : public IPlayerController {
public:

    AIPlayerController(int searchDepth)
        :   searchDepth(searchDepth)
    { }


    std::string getName() override {
        return "Our Engine";
    }


    void start(std::ostream* outputStream, std::ostream* errorStream) override {
        *outputStream << "Starting Our Engine" << std::endl;
        *outputStream << "Search depth: " << searchDepth << std::endl;
    }
    

    Move getMove(const State& state, const std::vector<Move>& initialMoves, const Move& lastMove) {
        //TODO: check state tur tur %2 er hvid overf�r last white move eller last black move
        /*TODO: first take lastMove. If last move is NULL start from white tree 
        if last move isn't NULL, last move is a book move and turn is white insert move into white tree and return a response from the next nodes
        if last move isn't NULL, last move is a book move and turn is black insert move into black tree and return a response from the next nodes
        */ 
        // ACTUALLY all trees should work for both players.
        MinMaxSearcher::Result result = MinMaxSearcher::search(state, searchDepth);
        return result.bestMove;
    }


private:

    int searchDepth;

};