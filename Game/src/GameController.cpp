#include "GameController.h"

#include <cassert>
#include <sstream>

#include "MoveUtil.h"


void GameController::start(State state) {

    IPlayerController::GameInfo gameInfo;
    gameInfo.currentState = state;

    whitePlayer.start(&std::cout, &std::cerr);
    blackPlayer.start(&std::cout, &std::cerr);

    bool running = true;
    while(running) {

        IPlayerController& currentPlayer = state.isWhitesTurn() ? whitePlayer : blackPlayer;

        MoveUtil::getAllMoves(state, gameInfo.validMoves);

        if( clearScreen ) {
            std::system("cls");
        }

        std::cout << '\n' << state.toPrettyString("\t", gameInfo.getLastWhiteMove(), gameInfo.getLastBlackMove()) << '\n';

        std::cout << '\n';
		std::cout << "FEN: " << state.toFEN() << '\n';
		std::cout << "Player: " << ( state.isWhitesTurn() ? "White" : "Black") << "\n";
		std::cout << "Turn: " << (state.turn+1) << std::endl;

        std::cout << "\nLast moves:" << std::endl;
        if( state.turn == 0 ) {
            std::cout << "  No one has moved..." << std::endl;;
        }
        else {
            if( state.turn > 0 ) {
                std::cout << "  White: " << gameInfo.getLastMovedWhitePiece().getAlgebraicChar() << "  " << gameInfo.getLastWhiteMove() << std::endl;;
            }
            if( state.turn > 1 ) {
                std::cout << "  Black: " << gameInfo.getLastMovedBlackPiece().getAlgebraicChar() << "  " << gameInfo.getLastBlackMove() << std::endl;;
            }
        }       

        if( gameInfo.validMoves.size() == 0 ) {
            std::cout << "\nCHECKMATE! " << ( state.isWhitesTurn() ? "Black" : "White") << " won!" << std::endl;
            
            std::cout << "\nPress any key to exit" << std::endl;
            std::string input;
		    getline(std::cin, input);
            break;
        }

        if( printValidMoves ) {
            std::cout << "\nValid moves:\n";
            for( size_t i = 0; i < gameInfo.validMoves.size(); i++ ) {
                std::cout << "  " << gameInfo.validMoves[i] << std::endl;
            }
        }

        std::cout << "\nWaiting for " << ( state.isWhitesTurn() ? "White" : "Black") << " to make a move..." << std::endl;
        
        IPlayerController::TurnResult turnResult = currentPlayer.giveTurn(gameInfo);

        if( turnResult.numStatesToRevert > 0 ) {
            
            assert(turnResult.numStatesToRevert <= gameInfo.previousStates.size() 
            && "Cannot revert more states than number of states that has been played");

            for( unsigned int i = 0; i < turnResult.numStatesToRevert; i++ ) {
                gameInfo.currentState = gameInfo.previousStates.back();
                gameInfo.previousStates.pop_back();
                gameInfo.previousMoves.pop_back();
            }
            state = gameInfo.currentState;
        }
        else {

            assert(gameInfo.validMoves.contains(turnResult.chosenMove) && "Move generated by PlayerController is not valid");
            
            // Execute move
            gameInfo.previousStates.push_back(state);
            gameInfo.previousMoves.push_back(turnResult.chosenMove);
            state = MoveUtil::executeMove(state, turnResult.chosenMove);
            gameInfo.currentState = state;    
        }
    }   

}