#include "GameController.h"

#include <cassert>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "Util.h"
#include "NullStream.h"
#include "MoveUtil.h"


NullStream NULLSTREAM;

namespace fs = std::filesystem;

const fs::path LOG_DIRECTORY = "tui_game_logs";


void openLog(std::fstream& file, std::string name) {
    fs::create_directories(LOG_DIRECTORY);
    fs::path logPath = LOG_DIRECTORY / (name + ".log");

   file.open(
       logPath,
       std::fstream::out | std::fstream::trunc
   );
}


void GameController::start(State state) {

    IPlayerController::GameInfo gameInfo;
    gameInfo.currentState = state;

    if( fs::exists(LOG_DIRECTORY) ) {
        for( const auto& entry : std::filesystem::directory_iterator(LOG_DIRECTORY) ) {
            std::filesystem::remove_all(entry.path());
        }
    }

    std::string whiteLogName = Util::combineStrings(Util::splitString(whitePlayer.getName(), " \t\n"), "");
    std::fstream whiteLog;
    openLog(whiteLog, whiteLogName);

    std::string blackLogName = Util::combineStrings(Util::splitString(blackPlayer.getName(), " \t\n"), "");
    std::fstream blackLog;
    openLog(blackLog, blackLogName);

    whitePlayer.start(&whiteLog, &whiteLog);
    blackPlayer.start(&blackLog, &blackLog);

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

    whiteLog.close();
    blackLog.close();
}