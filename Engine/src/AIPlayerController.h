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
    {
        Zobrist::initZobristTable();
    }


    std::string getName() override {
        return "Our Engine";
    }


    void start(std::ostream* outputStream, std::ostream* errorStream) override {
        *outputStream << "Starting Our Engine" << std::endl;
        *outputStream << "Search depth: " << searchTime << std::endl;
        srand((unsigned int)time(NULL));
    }


    TurnResult giveTurn(const GameInfo& gameInfo) {
        TurnResult result;
        result.chosenMove = getMove(gameInfo);
        return result;
    }
    

    Move getMove(const GameInfo& gameInfo) {

        prevStatesHashes.clear();
        for( auto& previousState : gameInfo.previousStates ) {
            unsigned long long hash = Zobrist::calcHashValue(previousState.board);
            prevStatesHashes.push_back(hash);
        }

        if (useOpeningBook) {
            std::vector<BookMoves::Node*> bookmoves;
            if (currentBookMove == NULL) {
                BookMoves::initTree();
                bookmoves = BookMoves::Node::getRoots();
                if (gameInfo.currentState.isWhitesTurn()) { //white
                    currentBookMove = bookmoves[rand() % bookmoves.size()];
                    return currentBookMove->move;
                }
                else { // black
                    for (int i = 0; i < bookmoves.size(); i++) {
                        if ((bookmoves[i]->move) == gameInfo.previousMoves.back()) {
                            currentBookMove = bookmoves[i];
                        }
                    }
                    
                    if (currentBookMove!= NULL) {
                        currentBookMove = currentBookMove->children[rand() % currentBookMove->children.size()];
                        return currentBookMove->move;
                    }
                    else useOpeningBook = false;
                    MinMaxSearcher::Result result = MinMaxSearcher::searchTimed(gameInfo.currentState, searchTime, prevStatesHashes);
                    return result.bestMove;
                }
                
            }
            BookMoves::Node* last = currentBookMove->findChild(gameInfo.previousMoves.back());
            if (last != NULL && last->children.size() > 0) {
                currentBookMove = last->children[rand() % last->children.size()];
                return currentBookMove->move;
            }
            else useOpeningBook = false;

        }

        MinMaxSearcher::Result result = MinMaxSearcher::searchTimed(gameInfo.currentState, searchTime, prevStatesHashes);

        return result.bestMove;
    }


private:

    int searchTime;

    bool useOpeningBook;
    
    BookMoves::Node* currentBookMove = nullptr;

    std::vector<unsigned long long> prevStatesHashes;
    
};