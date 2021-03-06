#include "UCIEngine.h"

#include <vector>
#include <iostream>
#include <cassert>
#include <atomic>

#include "Util.h"
#include "Warning.h"


namespace fs = std::filesystem;

using TurnResult = IPlayerController::TurnResult;
using GameInfo = IPlayerController::GameInfo;


// It's uncertain whether printing debug info will slow down Engine
// (it's untested).
constexpr bool PRINT_DEBUG_INFO = true;


UCIEngine::UCIEngine(std::string name, fs::path enginePath, long long searchTime)
    :   name(name),
        enginePath(enginePath),
        searchTime(searchTime)
{
    assert(!name.empty());
    assert(!enginePath.empty());
}


UCIEngine::~UCIEngine() {
    if( process != nullptr ) {
        delete process;
    }
}


std::string UCIEngine::getName() {
    return name;
}


void UCIEngine::start(std::ostream* outputStream, std::ostream* errorStream) {

    this->outputStream = outputStream;
    this->errorStream = errorStream;

    if( process == nullptr ) {
        startProcess();
    }

    *outputStream << "Starting new game.." << std::endl;
    *outputStream << "  Search time: " << searchTime.count() << "ms" << std::endl; 
    writeToEngine("ucinewgame");
    writeToEngine("position startpos");
    engineReadySignal.reset();
    writeToEngine("isready");
    engineReadySignal.wait();
    *outputStream << "Engine is ready for new game" << std::endl;    
}


void UCIEngine::startProcess() {
    process = new Util::Process(
        { 
            fs::absolute(enginePath).string()
        },
        [&](auto &output) { this->onEngineStdOutput(output); },
        [&](auto &error) { this->onEngineStdError(error); },
        [&]() { this->onProcessTerminated(); }
    );

    engineNameSignal.reset();
    engineAuthorSignal.reset();
    uciOkSignal.reset();

    writeToEngine("uci");

    std::string engineName = engineNameSignal.wait();
    std::string engineAuthor = engineAuthorSignal.wait();
    uciOkSignal.wait();

    *outputStream << "Engine supports UCI" << std::endl;
    *outputStream << "Engine name: " << engineName << std::endl;
    *outputStream << "Engine author: " << engineAuthor << std::endl;

    // Write options
    // WARNING: This is setup specifcally for the Stockfish engine, and it will break
    // if we switch it out with another. This should really be setup using a more
    // general system
    writeToEngine("setoption name UCI_LimitStrength value true"); // Enables lower elo (default is 1359)

    if( PRINT_DEBUG_INFO ) {
        writeToEngine("debug on");
    }

    engineReadySignal.reset();
    writeToEngine("isready");
    engineReadySignal.wait();
    *outputStream << "Engine is ready" << std::endl;
}


TurnResult UCIEngine::giveTurn(const GameInfo& gameInfo) {

    *outputStream << "\nStarting engine search" << std::endl;
    std::stringstream ss;
    ss << "position fen " << gameInfo.currentState.toFEN();
    writeToEngine(ss.str());
    
    engineReadySignal.reset();
    writeToEngine("isready");
    engineReadySignal.wait();

    writeToEngine("go infinite");

    *outputStream << "Waiting for search..." << std::endl;
    std::this_thread::sleep_for(searchTime);

    *outputStream << "Stopping search..." << std::endl;
    bestMoveSignal.reset();
    writeToEngine("stop");
    Move bestMove = bestMoveSignal.wait();

    *outputStream << "Stopped search" << std::endl;
       
    return bestMove;
}


void UCIEngine::writeToEngine(const std::string& message) {
    if( outputStream != nullptr ) {
        *outputStream << message << std::endl;
    }
    process->writeLine(message);
     if( outputStream != nullptr ) {
        *outputStream << "Sent" << std::endl;
    }   
}


void UCIEngine::onEngineStdOutput(const std::string& output) {
    
    if( output.size() == 0 ) {
        std::cout << "WARNING: StdOut output from engine is empty" << std::endl;
        return;
    }

    *outputStream << "  " << output << std::endl;
     
    std::vector<std::string> tokens = Util::splitString(output, " \t\r\n\0");
    if( tokens.size() == 0 ) {
        *outputStream << "StdOut output from engine parsed into no tokens" << std::endl;
        return;
    }    

    if( tokens[0] == "id" ) {
        if( tokens[1] == "name" ) {
            engineNameSignal = Util::combineStrings(tokens, " ", 2);
        }
        if( tokens[1] == "author" ) {
            engineAuthorSignal = Util::combineStrings(tokens, " ", 2);
        }
    }

    if( tokens[0] == "uciok" ) {
        uciOkSignal.set();
    }

    if( tokens[0] == "readyok" ) {
        engineReadySignal.set();
    }

    if( tokens[0] == "bestmove" ) {
        Move bestMove;
        bool validMove = Move::fromAlgebraicNotation(tokens[1], bestMove);
        if( !validMove ) {
            *errorStream << "Invalid best move " << tokens[1] << std::endl;
        }
        bestMoveSignal = bestMove;
    }
}


void UCIEngine::onEngineStdError(const std::string& error) {
    if( errorStream != nullptr ) {
        *errorStream << "  ERROR:" << error << std::endl;    
    }
}


void UCIEngine::onProcessTerminated() {
    *outputStream << "Engine process terminated" << std::endl;

    // Note: we don't do anything to unblock the engine, which may be waiting
    // for a response from the Engine. We can do that (+ proper restarting)
    // if we have time.
}