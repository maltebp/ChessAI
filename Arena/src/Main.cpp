#pragma once

#include <string>

#include "AIPlayerController.h"
#include "ArenaController.h"
#include "State.h"
#include "UCIEngine.h"
#include "MoveUtil.h"


// Remember to update .gitignore if this is changed
const char* OUTPUT_PATH = "arena_results";

// This relative path will only work when Arena is being run from Visual Studio
const char* STOCKFISH_PATH = "resources\\stockfish\\stockfish_14_x64_popcnt.exe";


int main(int argc, char* argv[]) {
    
    fs::path outputPath = fs::current_path() / OUTPUT_PATH;

    State state = State::createDefault();

    UCIEngine stockfish1 = UCIEngine("Stockfish 14", STOCKFISH_PATH, 8000);

    // UCIEngine stockfish2 = UCIEngine("Stockfish 14", enginePath, 1000);
    AIPlayerController ourEngine = AIPlayerController(5);

    ArenaController controller = {
        outputPath,
        2,
        ourEngine,
        stockfish1
    };

    controller.start();

}