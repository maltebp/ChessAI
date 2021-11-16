#pragma once

#include <vector>
#include <climits>

#include "IPlayerController.h"
#include "MinMax.h"
#include "BookTree.h"
#include <cstdlib>

class AIPlayerController : public IPlayerController {
    bool book = false;
    BookMoves::Node* current;
public:

    AIPlayerController(int searchDepth)
        :   searchDepth(searchDepth)
    { 
        Zobrist::initZobristTable();
    }


    std::string getName() override {
        return "Our Engine";
    }


    void start(std::ostream* outputStream, std::ostream* errorStream) override {
        *outputStream << "Starting Our Engine" << std::endl;
        *outputStream << "Search depth: " << searchDepth << std::endl;
    }


    TurnResult giveTurn(const GameInfo& gameInfo) {
        TurnResult result;
        result.chosenMove = getMove(gameInfo);
        return result;
    }
    

    Move getMove(const GameInfo& gameInfo) {

        if (book) {
            std::vector<BookMoves::Node*> bookmoves;
            if (current == NULL) {
                BookMoves::initTree();
                bookmoves = BookMoves::Node::getRoots();
                current = bookmoves[rand() % bookmoves.size()];
                return current->move;
            }
            BookMoves::Node* last = current->findChild(gameInfo.previousMoves.back());
            if (last != NULL && last->children.size() > 0) {
                current = last->children[rand() % last->children.size()];
                return current->move;
            }
            else book = false;

        }

        prevStatesHashes.clear();
        for( auto& previousState : gameInfo.previousStates ) {
            unsigned long long hash = Zobrist::calcHashValue(previousState.board);
            prevStatesHashes.push_back(hash);
        }

        MinMaxSearcher::Result result = MinMaxSearcher::search(gameInfo.currentState, searchDepth, prevStatesHashes);

        return result.bestMove;
    }


private:

    std::vector<unsigned long long> prevStatesHashes;

    int searchDepth;
};