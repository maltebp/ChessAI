#include "UCIEngine.h"

#include <vector>
#include <iostream>

#include "external/subprocess.h"


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


Move UCIEngine::getMove(const State& state) {
    return Move();
}


void UCIEngine::reset() {

}


void UCIEngine::onEngineStdOutput(const std::string& output) {
    std::cout << "  " << output << std::endl;
}


void UCIEngine::onEngineStdError(const std::string& error) {
    std::cout << "  ERROR: " << error << std::endl;
}