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

    /*UCIEngine stockfish1 = UCIEngine("Stockfish", STOCKFISH_PATH, 8000);
    AIPlayerController ourEngine = AIPlayerController(5);*/

    UCIEngine stockfish1 = UCIEngine("Stockfish", STOCKFISH_PATH, 250);
    AIPlayerController ourEngine = AIPlayerController(5);

    ArenaController controller = {
        outputPath,
        ourEngine,
        stockfish1,
        "base_1_5_250"
    };

    controller.start();

}