#include "UCIEngine.h"

#include <vector>
#include <iostream>

#include "external/subprocess.h"


UCIEngine::UCIEngine(const std::string& enginePath)
{
    process = new Process(
        { 
            "C:\\Users\\Malte\\Downloads\\Helloer\\Helloer\\Debug\\Helloer.exe"
        },
        [](auto &output) {
            std::cout << "Output: " << output << std::endl;
        },
        [](auto &error) {
            std::cout << "Error: " << error << std::endl;
        }
    );

    while(true) {
        std::string input;
        std::cout << "\n>"; 
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