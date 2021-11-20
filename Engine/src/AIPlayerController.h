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
        this->outputStream = outputStream;
        *outputStream << "\n\n------------------------------------------------------------" << std::endl;
        *outputStream << "Starting new game!" << std::endl;
        *outputStream << "Search time: " << searchTime << std::endl;
        *outputStream << "Using opening book: " << (useOpeningBook ? "true" : "false") << std::endl;
        openingFinished = !useOpeningBook;
        currentBookMove = nullptr;
        srand((unsigned int)time(NULL));
    }


    TurnResult giveTurn(const GameInfo& gameInfo) {
        TurnResult result;
        result.chosenMove = getMove(gameInfo);

        *outputStream << "Chosen move: " << result.chosenMove << std::endl;

        return result;
    }
    

private:


    Move getMove(const GameInfo& gameInfo) {

        unsigned long long stateHash = Zobrist::calcHashValue(gameInfo.currentState.board);
        *outputStream << "State: " << gameInfo.currentState.toFEN() << "  (Hash: " << stateHash << ")" << std::endl;

        prevStatesHashes.clear();
        for( auto& previousState : gameInfo.previousStates ) {
            unsigned long long hash = Zobrist::calcHashValue(previousState.board);
            prevStatesHashes.push_back(hash);
        }

        BookMoves::Node* bookMove = getBookMove(gameInfo);
        if( bookMove != nullptr ) {
            *outputStream << "Using move from opening: " << bookMove->openingName << std::endl;
            return bookMove->move;
        } 

        *outputStream << "Analyzing MinMax move..." << std::endl;

        MinMaxSearcher::Result minmaxResult = MinMaxSearcher::searchTimed(gameInfo.currentState, searchTime, prevStatesHashes);

        *outputStream << "MinMax result: " << std::endl;
        *outputStream << "  Search time:      " << minmaxResult.searchTime << " sec" << std::endl;
        *outputStream << "  Depths finished:  " << minmaxResult.depthsFinished << std::endl;
        *outputStream << "  Nodes visited:    " << minmaxResult.nodesVisited << std::endl;
        *outputStream << "  Branch factor:    " << minmaxResult.branchingFactor << std::endl;
        *outputStream << "  Cut offs:         " << minmaxResult.cutOffFactor << std::endl;
        *outputStream << "  Evaluations:      " << minmaxResult.staticEvaluations << std::endl;
        *outputStream << "  Checkmates found: " << minmaxResult.checkmates << std::endl;
        *outputStream << "  Draws found:      " << minmaxResult.draws << std::endl;

        if( minmaxResult.dynamicAllocations > 0 ) {
            *outputStream << "WARNING: MinMax did " << minmaxResult.dynamicAllocations << " dynamic allocation(s)!" << std::endl;
        }

        return minmaxResult.bestMove;
    }


    BookMoves::Node* getBookMove(const GameInfo& gameInfo) {

        if( openingFinished ) return nullptr;
        
        std::vector<BookMoves::Node*> bookmoves;

        if (currentBookMove == nullptr) {
        
            BookMoves::initTree();
            bookmoves = BookMoves::Node::getRoots();
        
            if (gameInfo.currentState.isWhitesTurn()) { //white
                currentBookMove = bookmoves[rand() % bookmoves.size()];
                return currentBookMove;
            }
            else { // black
                for (int i = 0; i < bookmoves.size(); i++) {
                    if ((bookmoves[i]->move) == gameInfo.previousMoves.back()) {
                        currentBookMove = bookmoves[i];
                    }
                }
                
                if (currentBookMove != nullptr) {
                    currentBookMove = currentBookMove->children[rand() % currentBookMove->children.size()];
                    return currentBookMove;
                }
                else {
                    openingFinished = true;
                    return nullptr;
                }
            }
            
        }
        
        BookMoves::Node* last = currentBookMove->findChild(gameInfo.previousMoves.back());
        if (last != NULL && last->children.size() > 0) {
            currentBookMove = last->children[rand() % last->children.size()];
            return currentBookMove;
        }
        else {
            openingFinished = true;
            return nullptr;
        }
    }


private:

    const int searchTime;

    const bool useOpeningBook;

    bool openingFinished = false;
    
    BookMoves::Node* currentBookMove = nullptr;

    std::vector<unsigned long long> prevStatesHashes;

    std::ostream* outputStream = nullptr;
    
};