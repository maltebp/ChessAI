#include "UCIEngine.h"

#include <vector>
#include <iostream>
#include <cassert>

#include "external/subprocess.h"

#include "Util.h"
#include "Warning.h"


UCIEngine::UCIEngine(const std::string& enginePath)
{
    process = new Process(
        { 
            "C:\\Users\\Malte\\Projects\\ChessAI\\resources\\stockfish\\stockfish_14_x64_popcnt.exe"
        },
        [this](auto &output) { this->onEngineStdOutput(output); },
        [this](auto &error) { this->onEngineStdError(error); }
    );

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
    
    std::vector<std::string> tokens = Util::splitString(output, " \t\n\0");
    if( tokens.size() == 0 ) {
        WARN("StdOut output from engine parsed into no tokens (output: '%s')", output.c_str());
        return;
    }    

    if( tokens[0] == "id" ) {
        if( tokens[1] == "name" ) {
            std::string name = Util::combineStrings(tokens, " ", 2);
            std::cout << "Engine name: " << name << std::endl;
        }
        if( tokens[1] == "author" ) {
            std::string author = Util::combineStrings(tokens, " ", 2);
            std::cout << "Engine author: " << author << std::endl;
        }
    }

    if( tokens[0] == "uciok" ) {
        // Game on!
    }

    if( tokens[0] == "readyok" ) {
        std::cout << "Engine is ready" << std::endl;
    }
}


void UCIEngine::onEngineStdError(const std::string& error) {
    std::cout << "  ERROR:" << error << std::endl;
}