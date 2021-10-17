#include "UCIEngine.h"

#include <vector>
#include <iostream>
#include <cassert>

#include "external/subprocess.h"

#include "Warning.h"


namespace {

    std::string combineTokens(const std::vector<std::string>& tokens, size_t fromIndex) {
        assert(tokens.size() > 0);
        assert(fromIndex < tokens.size());

        std::stringstream ss;
        for(size_t i = fromIndex; i < tokens.size(); i++ ) {
            ss << tokens[i] << " ";
        }

        return ss.str();
    }


    std::vector<std::string> splitString(const std::string& input) {
        std::vector<std::string> tokens;
        std::stringstream ss;

        auto addToken = [&](){
            std::string token = ss.str();
            if( token.length() > 0 ) {
                tokens.push_back(token);
            }
            ss.str("");
        };
            
        for( auto c : input ) {
            if(c == ' ' || c == '\t' || c == '\n' || c == '\0') {
                addToken();
            }
            else {
                ss << c;
            }
        }

        addToken();

        return tokens;
    }

}


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
    
    if( output.size() == 0 ) {
        WARN("StdOut output from engine is empty");
        return;
    }

    std::cout << "  " << output << std::endl;
    
    std::vector<std::string> tokens = splitString(output);
    if( tokens.size() == 0 ) {
        WARN("StdOut output from engine parsed into no tokens (output: '%s')", output.c_str());
        return;
    }    

    if( tokens[0] == "id" ) {
        if( tokens[1] == "name" ) {
            std::string name = combineTokens(tokens, 2);
            std::cout << "Engine name: " << name << std::endl;
        }
        if( tokens[1] == "author" ) {
            std::string author = combineTokens(tokens, 2);
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