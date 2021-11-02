#include "GameController.h"

#include <cassert>
#include <sstream>

#include "MoveUtil.h"


void GameController::start(State state) {

    whitePlayer.start(&std::cout, &std::cerr);
    blackPlayer.start(&std::cout, &std::cerr);

    Piece lastMovedWhitePiece;
    Move lastWhiteMove;

    Piece lastMovedBlackPiece;
    Move lastBlackMove;

    bool running = true;
    while(running) {

        IPlayerController& currentPlayer = state.turn % 2 == 0 ? whitePlayer : blackPlayer;

        // TODO: Either pass vector to getAllMoves or remove this line
        std::vector<Move> availableMoves = MoveUtil::getAllMoves(state);

        if( clearScreen ) {
            std::system("cls");
        }

        std::cout << '\n' << state.toPrettyString("\t", lastWhiteMove, lastBlackMove) << '\n';

        std::cout << '\n';
		std::cout << "FEN: " << state.toFEN() << '\n';
		std::cout << "Player: " << (state.turn % 2 == 0 ? "White" : "Black") << "\n";
		std::cout << "Turn: " << (state.turn+1) << std::endl;

        std::cout << "\nLast moves:" << std::endl;
        if( state.turn == 0 ) {
            std::cout << "  No one has moved..." << std::endl;;
        }
        else {
            if( state.turn > 0 ) {
                std::cout << "  White: " << lastMovedWhitePiece.getAlgebraicChar() << "  " << lastWhiteMove << std::endl;;
            }
            if( state.turn > 1 ) {
                std::cout << "  Black: " << lastMovedBlackPiece.getAlgebraicChar() << "  " << lastBlackMove << std::endl;;
            }
        }       

        if( availableMoves.size() == 0 ) {
            std::cout << "\nCHECKMATE! " << (state.turn % 2 == 0 ? "Black" : "White") << " won!" << std::endl;
            
            std::cout << "\nPress any key to exit" << std::endl;
            std::string input;
		    getline(std::cin, input);
            break;
        }

        if( printValidMoves ) {
            std::cout << "\nValid moves:\n";
            for(const auto& move : availableMoves) {
                std::cout << "  " << move << std::endl;
            }
        }

        std::cout << "\nWaiting for " << (state.turn % 2 == 0 ? "White" : "Black") << " to make a move..." << std::endl;
        // Overfør last move, check vis tur det er
        Move move;
        if (state.turn % 2 == 0)
            move = currentPlayer.getMove(state, availableMoves, lastBlackMove);
        else
            move = currentPlayer.getMove(state, availableMoves, lastWhiteMove);
        //Move move = currentPlayer.getMove(state, availableMoves);
        auto iterator = std::find(availableMoves.begin(), availableMoves.end(), move);
        assert(iterator != availableMoves.end() && "Move generated by PlayerController is not valid");

        if( state.turn % 2 == 0 ) {
            lastWhiteMove = move;
            lastMovedWhitePiece = state[move.fromField];
        }
        else {
            lastBlackMove = move;
            lastMovedBlackPiece = state[move.fromField];
        }
        
        // Execute move
        state = MoveUtil::executeMove(state, move);
    }   

}