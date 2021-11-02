#include "ArenaController.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <thread>

#include "Util.h"
#include "TUIUtil.h"
#include "MoveUtil.h"


ArenaController::ArenaController(
    const fs::path& outputPath,
    IPlayerController& engine1,
    IPlayerController& engine2,
    std::string tag
)
    :   outputPath(outputPath),
        engine1(engine1),
        engine2(engine2),
        tag(tag)
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


void ArenaController::start() {

    std::cout << "  Output path: " << outputPath << std::endl;

    fs::path sessionPath = outputPath / Util::getDateString("%d_%m_%H_%M_%S");
    std::cout << "  Session path: " << sessionPath << std::endl;

    // Start test thread
    std::thread s([&]() { this->runSession(sessionPath); });

    // Start input parsing
        
    while(true) {

        // In case the test thread stops before we request it to
        if( testThreadHasStopped ) {
            std::cout << "Session stopped" << std::endl;
            break;
        }

        std::cout << "\n>" << std::flush;

        std::string rawInput;
        std::getline(std::cin, rawInput);
        std::vector<std::string> input = Util::splitString(rawInput, " \t\r\n");

        std::string command = input.size() == 0 ? "" : input[0];

        if( input.size() == 0 ) {
            continue;
        }

        if( command == "stop" ) {
            std::cout << "Stopping session after current game..." << std::endl;
            stopTestThread = true;
            s.join();
            std::cout << "Session stopped" << std::endl;
            break;
        }

        if( command == "results" ) {
            std::ifstream resultsFile(sessionPath / "results.csv");
            std::string content( 
                (std::istreambuf_iterator<char>(resultsFile)),
                (std::istreambuf_iterator<char>()) 
            );

            std::cout << "\nresults.csv:\n" << content << std::endl; 
            continue;
        }

        std::cout << "Error: invalid command" << std::endl;

    }
}


void ArenaController::runSession(const fs::path& outputPath) {

    fs::create_directories(outputPath);
    fs::path resultsFilePath = outputPath / "results.csv";

    std::fstream resultsFile;
    resultsFile.open(
        resultsFilePath,
         std::fstream::out | std::fstream::app
    );

    // CSV header
    resultsFile 
        << "tag,"
        << "game_id,"
        << "winner," // Engine number
        << "white,"
        << "black,"
        << "half_turns,"
        << "engine1_pieces,"
        << "engine2_pieces,"
        << "engine1_time,"
        << "engine2_time,"
        << "end_state" 
        << std::endl; 
    
    unsigned int gameId = 1;

    while( !stopTestThread ) {
        GameResult result = runGame(gameId, outputPath);

        std::ios streamState(nullptr);
        streamState.copyfmt(resultsFile);
        resultsFile << std::setprecision(2) << std::fixed;
        resultsFile 
            << tag << ','
            << gameId << ',' 
            << result.winner << ','
            << result.whitePlayerId << ','
            << result.blackPlayerId << ','
            << result.halfTurns << ','
            << result.engine1PiecesLeft << ','
            << result.engine2PiecesLeft << ','
            << result.engine1SearchTime << ','
            << result.engine2SearchTime << ','
            << result.endState
            << std::endl;

        // Reset the stream state (reset precision, and fixed)
        resultsFile.copyfmt(streamState);

        gameId++;
    }

    resultsFile.close();

    std::cout << "Session thread stopped" << std::endl;

    testThreadHasStopped = true;
}


ArenaController::GameResult ArenaController::runGame(unsigned int gameNum, const fs::path& dir) {

    GameResult result;

    // Open logs
    std::fstream gameLog;
    std::fstream engine1Log;
    std::fstream engine2Log;
    {
        // Create dir
        std::stringstream ss;
        ss << std::setw(3) << std::setfill('0') << gameNum;
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
   
    gameLog << "Starting " << engine1Name << " (Engine 1)" << std::endl;
    engine1.start(&engine1Log, &engine1Log);
    gameLog << "Starting " << engine2Name << " (Engine 2)" << std::endl;
    engine2.start(&engine2Log, &engine2Log);
    
    unsigned int whitePlayerId = (gameNum % 2 == 1) ? 1 : 2;
    unsigned int blackPlayerId = (gameNum % 2 == 1) ? 2 : 1;
    IPlayerController& whitePlayer = whitePlayerId == 1 ? engine1 : engine2;
    IPlayerController& blackPlayer = blackPlayerId == 1 ? engine1 : engine2;

    gameLog << "Starting game" << std::endl;
    gameLog << "Time: " << Util::getDateString("%d/%m %H:%M:%S ") << std::endl;
    gameLog << "White player: " << whitePlayer.getName() << " (Engine " << whitePlayerId << ")" << std::endl;
    gameLog << "Black player: " << blackPlayer.getName() << " (Engine " << blackPlayerId << ")" << std::endl;

    State state = State::createDefault();
    Move lastMove;
    std::stringstream ss;   

    while(true) {

        bool whitesTurn = state.turn % 2 == 0;
        unsigned currentPlayerId = whitesTurn ? whitePlayerId : blackPlayerId;
        IPlayerController& currentEngine = whitesTurn ? whitePlayer : blackPlayer;

        std::string boardString = state.toPrettyString("  ");
        gameLog << '\n' << boardString << std::endl;
        gameLog << '\n' << state.toFEN() << std::endl;
        gameLog << "Turn: " << currentEngine.getName() << std::endl;

        if( state.drawCounter >= 49 ) {
            gameLog << "Winner: Draw" << std::endl;
            result.winner = 0;
            break;
        }

        MoveUtil::GenerationList availableMoves;
        MoveUtil::getAllMoves(state, availableMoves);
        if( availableMoves.size() == 0 ) {
            bool whiteWins = !whitesTurn;
            std::string winnerName = whiteWins ? whitePlayer.getName() : blackPlayer.getName();
            gameLog << "\nWinner: " << winnerName << std::endl;
            result.winner = whiteWins ? whitePlayerId : blackPlayerId; 
            break;
        }

        auto searchStartClock = std::chrono::system_clock::now();
        
        Move move = currentEngine.getMove(state, availableMoves, lastMove);

        auto searchEndClock = std::chrono::system_clock::now();
        auto searchTime = std::chrono::duration_cast<std::chrono::milliseconds>(searchEndClock - searchStartClock);    

        if( currentPlayerId == 1 ) {
            result.engine1SearchTime += searchTime.count() / 1000.0;
        }
        else {
            result.engine2SearchTime += searchTime.count() / 1000.0;
        }

        gameLog << "Search time: " << searchTime.count() << "ms" << std::endl;
        
        if( move == Move() ) {  
            std::stringstream ss;
            ss << currentEngine.getName() << " did INVALID MOVE!" << std::endl;
            ss << "Available moves were:" << std::endl;
            for( size_t i = 0; i < availableMoves.size(); i++ ) {
                ss << "  " << availableMoves[i] << std::endl;
            }

            gameLog << ss.str();
            std::cout << ss.str();

            state.turn++;
            state.drawCounter++;
            continue;
        }

        gameLog << "Move: " << move << std::endl;
        state = MoveUtil::executeMove(state, move);
        lastMove = move;
    }    

    result.whitePlayerId = whitePlayerId;
    result.blackPlayerId = blackPlayerId;
    result.halfTurns = state.turn;
    result.endState = state.toFEN();

    for( int x = 0; x < 8; x++ ) {
        for( int y = 0; y < 8; y++ ) {
            Piece piece = state.board[x][y];
            if( piece.getType() == PieceType::NONE ) {
                continue;
            }

            unsigned int pieceOwnerId = piece.getColor() == PieceColor::WHITE ? whitePlayerId : blackPlayerId;

            if( pieceOwnerId == 1 ) {
                result.engine1PiecesLeft++;
            }
            else {
                result.engine2PiecesLeft++;
            }
        }
    }

    gameLog.close();
    engine1Log.close();
    engine2Log.close();

    gameLog << "\nGame ended" << std::endl;

    return result;
}
