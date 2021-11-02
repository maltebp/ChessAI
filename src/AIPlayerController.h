#pragma once

#include <vector>

#include "IPlayerController.h"
#include "MinMax.h"
#include "BookTree.h"
#include <cstdlib>

class AIPlayerController : public IPlayerController {
    
    int searchDepth = 4;
    bool book = false;
    BookMoves::Node* current;
public:

    Move getMove(const State& state, const std::vector<Move>& initialMoves, const Move& lastMove) {
        if (book) {
            std::vector<BookMoves::Node*> bookmoves;
            if (current == NULL) {
                BookMoves::initTree;
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
        auto [move, score] = MinMaxSearcher::search(state, searchDepth, -1000, 1000);
        return move;
    }

};