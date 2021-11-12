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
    

    Move getMove(const State& state, const MoveUtil::GenerationList& validMoves, const Move& lastMove) {
        if (book) {
            std::vector<BookMoves::Node*> bookmoves;
            if (current == NULL) {
                BookMoves::initTree();
                bookmoves = BookMoves::Node::getRoots();
                current = bookmoves[rand() % bookmoves.size()];
                return current->move;
            }
            BookMoves::Node* last = current->findChild(lastMove);
            if (last != NULL && last->children.size() > 0) {
                current = last->children[rand() % last->children.size()];
                return current->move;
            }
            else book = false;

        }

        MinMaxSearcher::Result result = MinMaxSearcher::search(state, searchDepth, prevStatesHashes);
        auto hash = Zobrist::calcHashValue(state.board);
        prevStatesHashes.push_back(hash);

        return result.bestMove;
    }


private:

    std::vector<unsigned long long> prevStatesHashes;

    int searchDepth;
};