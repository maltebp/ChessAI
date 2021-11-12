#pragma once

#include <vector>
#include <climits>

#include "IPlayerController.h"
#include "MinMax.h"
#include "BookTree.h"
#include <cstdlib>

class AIPlayerController : public IPlayerController {
    bool book =true;
    BookMoves::Node* current;
public:

    AIPlayerController(int searchTime)
        :   searchTime(searchTime)
    { }


    std::string getName() override {
        return "Our Engine";
    }


    void start(std::ostream* outputStream, std::ostream* errorStream) override {
        *outputStream << "Starting Our Engine" << std::endl;
        *outputStream << "Search depth: " << searchTime << std::endl;
    }
    

    Move getMove(const State& state, const MoveUtil::GenerationList& validMoves, const Move& lastMove) {
        if (book) {
            srand(time(NULL));
            std::vector<BookMoves::Node*> bookmoves;
            if (current == NULL) {
                BookMoves::initTree();
                bookmoves = BookMoves::Node::getRoots();
                if (state.turn % 2 == 0) {//white
                    current = bookmoves[rand() % bookmoves.size()];
                    return current->move;
                }
                else { // black
                    for (int i = 0; i < bookmoves.size(); i++) {
                        if ((bookmoves[i]->move) == lastMove) {
                            current = bookmoves[i];
                        }
                    }
                    
                    if (current!= NULL) {
                        current = current->children[rand() % current->children.size()];
                        return current->move;
                    }
                    else book = false;
                    MinMaxSearcher::Result result = MinMaxSearcher::searchTimed(state, searchTime);
                    return result.bestMove;
                }
                
            }
            BookMoves::Node* last = current->findChild(lastMove);
            if (last != NULL && last->children.size() > 0) {
                current = last->children[rand() % last->children.size()];
                return current->move;
            }
            else book = false;

        }

        MinMaxSearcher::Result result = MinMaxSearcher::searchTimed(state, searchTime);
        
        return result.bestMove;
    }


private:

    int searchTime;

};