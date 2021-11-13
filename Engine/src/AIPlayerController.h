#pragma once

#include <vector>
#include <climits>
#include <cstdlib>

#include "IPlayerController.h"
#include "MinMax.h"
#include "BookTree.h"


class AIPlayerController : public IPlayerController {
public:


    AIPlayerController(int searchTime, bool useOpeningBook)
        :   searchTime(searchTime),
            useOpeningBook(useOpeningBook)
    { }


    std::string getName() override {
        return "Our Engine";
    }


    void start(std::ostream* outputStream, std::ostream* errorStream) override {
        *outputStream << "Starting Our Engine" << std::endl;
        *outputStream << "Search depth: " << searchTime << std::endl;
    }
    

    Move getMove(const State& state, const MoveUtil::GenerationList& validMoves, const Move& lastMove) {
        if (useOpeningBook) {
            srand(time(NULL));
            std::vector<BookMoves::Node*> bookmoves;
            if (currentBookMove == NULL) {
                BookMoves::initTree();
                bookmoves = BookMoves::Node::getRoots();
                if (state.turn % 2 == 0) {//white
                    currentBookMove = bookmoves[rand() % bookmoves.size()];
                    return currentBookMove->move;
                }
                else { // black
                    for (int i = 0; i < bookmoves.size(); i++) {
                        if ((bookmoves[i]->move) == lastMove) {
                            currentBookMove = bookmoves[i];
                        }
                    }
                    
                    if (currentBookMove!= NULL) {
                        currentBookMove = currentBookMove->children[rand() % currentBookMove->children.size()];
                        return currentBookMove->move;
                    }
                    else useOpeningBook = false;
                    MinMaxSearcher::Result result = MinMaxSearcher::searchTimed(state, searchTime);
                    return result.bestMove;
                }
                
            }
            BookMoves::Node* last = currentBookMove->findChild(lastMove);
            if (last != NULL && last->children.size() > 0) {
                currentBookMove = last->children[rand() % last->children.size()];
                return currentBookMove->move;
            }
            else useOpeningBook = false;

        }

        MinMaxSearcher::Result result = MinMaxSearcher::searchTimed(state, searchTime);
        
        return result.bestMove;
    }


private:

    int searchTime;

    bool useOpeningBook;
    
    BookMoves::Node* currentBookMove = nullptr;

};