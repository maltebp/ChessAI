#include "EngineArenaController.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <thread>

#include "Util.h"
#include "TUIUtil.h"
#include "MoveUtil.h"


EngineArenaController::EngineArenaController(
    const fs::path& outputPath,
    unsigned int numGames,
    IPlayerController& engine1,
    IPlayerController& engine2
)
    :   outputPath(outputPath),
        engine1(engine1),
        engine2(engine2),
        numGames(numGames)
{
    
    // Remove white space from names, and check if they are the same
    engine1Name = Util::combineStrings(Util::splitString(engine1.getName(), " \t\n"), "");
    assert(!engine1Name.empty());
    engine2Name = Util::combineStrings(Util::splitString(engine2.getName(), " \t\n"), "");
    assert(!engine2Name.empty());

    if( engine1Name == engine2Name ) {
        engine1Name += "_1";
        engine2Name += "_2";
    }
}


void EngineArenaController::start() {

    std::cout << "  Output path: " << outputPath << std::endl;

    fs::path sessionPath = outputPath / Util::getDateString("%d_%m_%H_%M_%S");
    std::cout << "  Session path: " << sessionPath << std::endl;

    // Start test thread
    std::thread s([&]() { this->runSession(sessionPath); });

    // Start input parsing
    
    while(true) {

        if( stoppedSignal.isSet() ) {
            std::cout << "Session stopped" << std::endl;
            break;
        }

        std::cout << "\n>" << std::flush;

        std::string rawInput;
        std::getline(std::cin, rawInput);
        std::vector<std::string> input = Util::splitString(rawInput, " \t\r\n");

        std::string command = input.size() == 0 ? "" : input[0];

        if( command == "stop" ) {
            std::cout << "Stopping session..." << std::endl;
            stop = true;
            stoppedSignal.wait();
            std::cout << "Session stopped" << std::endl;
            break;
        }

    }

}


void EngineArenaController::runSession(const fs::path& outputPath) {

    fs::create_directories(outputPath);
    fs::path resultsFilePath = outputPath / "results.csv";

    std::fstream resultsFile;
    resultsFile.open(
        resultsFilePath,
         std::fstream::out | std::fstream::app
    );

    // CSV header
    resultsFile << "Game Id,Winner" << std::endl; 
    
    for( unsigned int i=0; i<5; i++) {
        unsigned int gameId = i+1;
        GameResult result = runGame(gameId, outputPath);

        resultsFile << gameId << ',' << result.winner << std::endl; 
    }

    resultsFile.close();

    std::cout << "Session thread stopped" << std::endl;
}


EngineArenaController::GameResult EngineArenaController::runGame(unsigned int gameNum, const fs::path& dir) {

    GameResult result;

    // Open logs
    std::fstream gameLog;
    std::fstream engine1Log;
    std::fstream engine2Log;
    {
        std::stringstream ss;
        ss << std::setw(3) << std::setfill('0') << gameNum << Util::getDateString("_%H_%M");

        fs::path gameDir = dir / ss.str();
        fs::create_directories(gameDir);

        gameLog.open(
            gameDir / "game.log",
            std::fstream::out | std::fstream::app
        );

        engine1Log.open(
            gameDir / (engine1Name + ".log"),
            std::fstream::out | std::fstream::app
        );

        engine2Log.open(
            gameDir / (engine2Name + ".log"),
            std::fstream::out | std::fstream::app
        );
    }
   
    gameLog << "Starting " << engine1Name << " (Engine 1, White)" << std::endl;
    engine1.start(&engine1Log, &engine1Log);
    gameLog << "Starting " << engine2Name << " (Engine 2, White)" << std::endl;
    engine2.start(&engine2Log, &engine2Log);

    gameLog << "Starting game" << std::endl;
    State state = State::createDefault();
    Move lastMove;
    std::stringstream ss;   

    while(true) {

        IPlayerController& currentEngine = state.turn % 2 == 0 ? engine1 : engine2;

        std::string boardString = state.toPrettyString("  ");
        gameLog << '\n' << boardString << std::endl;
        gameLog << '\n' << state.toFEN() << std::endl;
        gameLog << "Turn: " << currentEngine.getName() << std::endl;

        if( state.drawCounter >= 49 ) {
            gameLog << "Result: Draw" << std::endl;
            result.winner = 0;
            break;
        }

        // This is only necessary because of the bug in our engine
        if( MoveUtil::isKingThreatened(state) ) {
            bool whiteWins = state.turn % 2 == 0;
            // Note: at some point, eninge1 should NOT be synonymous with "white"
            // They should switch turns every game
            std::string winnerName = whiteWins ? engine1Name : engine2Name;
            gameLog << "\nWinner: " << winnerName << std::endl;
            result.winner = whiteWins ? 1 : 2; 
            break;
        }

        std::vector<Move> availableMoves = MoveUtil::getAllMoves(state);
        if( availableMoves.size() == 0 ) {
            bool whiteWins = state.turn % 2 == 0;
            // Note: at some point, eninge1 should NOT be synonymous with "white"
            // They should switch turns every game
            std::string winnerName = whiteWins ? engine1Name : engine2Name;
            gameLog << "\nWinner: " << winnerName << std::endl;
            result.winner = whiteWins ? 1 : 2;
            break;
        }

        Move move = currentEngine.getMove(state, availableMoves, lastMove);
        if( move == Move() ) {  
            // TODO: This is just a bypass of an existing bug (https://github.com/maltebp/ChessAI/issues/23) - MUST BE FIXED!
            gameLog << "INVALID MOVE!" << std::endl;
            state.turn++;
            continue;
        }

        gameLog << "Move: " << move << std::endl;

        state = MoveUtil::executeMove(state, move);
        lastMove = move;
    }    

    gameLog.close();
    engine1Log.close();
    engine2Log.close();

    std::cout << "\nGame ended" << std::endl;

    return result;
}
