#include "EngineArenaController.h"

#include <cassert>
#include <fstream>
#include <iomanip>

#include "Util.h"
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
{ }


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

    std::stringstream ss;
    ss << std::setw(3) << std::setfill('0') << gameNum << Util::getDateString("_%H_%M");

    fs::path gameDir = dir / ss.str();
    fs::create_directories(gameDir);

    std::fstream gameLog;
    gameLog.open(
        gameDir / "game.log",
        std::fstream::out | std::fstream::app
    );

    
    std::string engine1Name = Util::combineStrings(Util::splitString(engine1.getName(), " \t\n"), "");
    std::string engine2Name = Util::combineStrings(Util::splitString(engine2.getName(), " \t\n"), "");

    if( engine1Name == engine2Name ) {
        engine1Name = "engine1_" + engine1Name;
        engine2Name = "engine2_" + engine2Name;
    }

    std::fstream engine1Log;
    engine1Log.open(
        gameDir / (engine1Name + ".log"),
        std::fstream::out | std::fstream::app
    );

    fs::path engine2LogPath = gameDir / (engine2Name + ".log");
    std::fstream engine2Log;
    engine2Log.open(
        gameDir / (engine2Name + ".log"),
        std::fstream::out | std::fstream::app
    );

    engine1.start(&engine1Log, &engine1Log);
    engine2.start(&engine2Log, &engine2Log);

    State state = State::createDefault();
    
    while(true) {

        IPlayerController& currentEngine = state.turn % 2 == 0 ? engine1 : engine2;

        // TODO: Replicate to std::cout somehow
        std::cout << "  " <<  state.toFEN() << std::endl;
        gameLog << state.toFEN() << std::endl;

        std::vector<Move> availableMoves = MoveUtil::getAllMoves(state);
        if( availableMoves.size() == 0 ) {
            gameLog << "  " << engine1Name << " wins" << std::endl;
        }

        Move move = currentEngine.getMove(state, availableMoves);
        if( move == Move() ) {
            // TODO: This is just a bypass of an existing bug - MUST BE FIXED!
            std::cout << "  " << "Invalid move!" << std::endl;
            gameLog << "Invalid move!" << std::endl;
            state.turn++;
        }

        std::cout << "  " << move << std::endl;
        gameLog << move << std::endl;

        state = MoveUtil::executeMove(state, move);
    }    

    gameLog.close();
    engine1Log.close();
    engine2Log.close();
}


