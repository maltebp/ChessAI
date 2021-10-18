#include "UCIEngine.h"

#include <vector>
#include <iostream>
#include <cassert>
#include <atomic>

#include "external/subprocess.h"

#include "Util.h"
#include "Warning.h"


UCIEngine::UCIEngine(const std::string& enginePath)
{
    process = new Process(
        { 
            enginePath
        },
        [this](auto &output) { this->onEngineStdOutput(output); },
        [this](auto &error) { this->onEngineStdError(error); }
    );

    engineNameSignal.reset();
    engineAuthorSignal.reset();
    uciOkSignal.reset();

    process->writeLine("uci");

    std::string engineName = engineNameSignal.wait();
    std::string engineAuthor = engineAuthorSignal.wait();
    uciOkSignal.wait();

    std::cout << "Engine supports UCI" << std::endl;
    std::cout << "Engine name: " << engineName << std::endl;
    std::cout << "Engine author: " << engineAuthor << std::endl;

    while(true) {
        std::string input;
        std::getline(std::cin, input);
        if( input == "stop1" ) break;
        process->writeLine(input);
    }
}

UCIEngine::~UCIEngine() {
    if( process != nullptr ) {
        delete process;
    }
}


Move UCIEngine::getMove(const State& state, const std::vector<Move>& validMoves) {
    return Move();
}


void UCIEngine::onEngineStdOutput(const std::string& output) {
    
    if( output.size() == 0 ) {
        WARN("StdOut output from engine is empty");
        return;
    }

    std::cout << "  " << output << std::endl;
     
    std::vector<std::string> tokens = Util::splitString(output, " \t\r\n\0");
    if( tokens.size() == 0 ) {
        WARN("StdOut output from engine parsed into no tokens (output: '%s')", output.c_str());
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
        std::cout << "Game on!" << std::endl;
    }

    if( tokens[0] == "readyok" ) {
        std::cout << "Engine is ready" << std::endl;
    }
}


void UCIEngine::onEngineStdError(const std::string& error) {
    std::cout << "  ERROR:" << error << std::endl;
}