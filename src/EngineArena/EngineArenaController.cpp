#include "EngineArenaController.h"

#include <cassert>
#include <fstream>
#include <iomanip>

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


    std::cout << "Output path: " << outputPath << std::endl;

    fs::path sessionPath = outputPath / Util::getDateString("%d_%m_%H_%M");
    std::cout << "Session path: " << sessionPath << std::endl;

    fs::create_directories(sessionPath);

    fs::path resultsFilePath = sessionPath / "results";

    std::fstream resultsFile;
    resultsFile.open(
        resultsFilePath,
         std::fstream::out | std::fstream::app
    );

    
    for( unsigned int i=0; i<5; i++) {
        runGame(i+1, sessionPath);
    }

    resultsFile.close();
}


void EngineArenaController::runGame(unsigned int gameNum, const fs::path& dir) {

    std::cout << "Starting game " << gameNum << " .." << std::endl;

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
   
    engine1.start(&engine1Log, &engine1Log);
    engine2.start(&engine2Log, &engine2Log);

    State state = State::createDefault();

    std::stringstream ss;   
    while(true) {

        IPlayerController& currentEngine = state.turn % 2 == 0 ? engine1 : engine2;

        // TODO: Replicate to std::cout somehow
        std::string boardString = TUIUtil::getPrettyBoard(state);
        
        std::cout << '\n' << boardString << std::endl;
        gameLog << '\n' << boardString << std::endl;

        std::cout << "  " <<  state.toFEN() << std::endl;
        gameLog << '\n' << state.toFEN() << std::endl;

        if( state.drawCounter >= 49 ) {
            gameLog << "Result: Draw" << std::endl;
            break;
        }

        if( MoveUtil::isKingThreatened(state) ) {
            // This is only necessary because of the bug in our engine
            std::cout << "  " << (state.turn % 2 == 0 ? engine1Name : engine2Name) << " wins" << std::endl;
            gameLog << "  " << (state.turn % 2 == 0 ? engine1Name : engine2Name) << " wins" << std::endl;
            break;
        }

        std::vector<Move> availableMoves = MoveUtil::getAllMoves(state);
        if( availableMoves.size() == 0 ) {
            const std::string& winner = (state.turn % 2 == 0 ? engine2Name : engine1Name);
            std::cout << "Result: " << winner << " wins" << std::endl;
            gameLog << "Result: " << winner << " wins" << std::endl;
            break;
        }

        Move move = currentEngine.getMove(state, availableMoves);
        if( move == Move() ) {  
            // TODO: This is just a bypass of an existing bug (https://github.com/maltebp/ChessAI/issues/23) - MUST BE FIXED!
            std::cout << "  " << "Invalid move!" << std::endl;
            gameLog << "Invalid move!" << std::endl;
            state.turn++;
            continue;
        }

        std::cout << "  " << move << std::endl;
        gameLog << move << std::endl;

        state = MoveUtil::executeMove(state, move);
    }    

    gameLog.close();
    engine1Log.close();
    engine2Log.close();
}
