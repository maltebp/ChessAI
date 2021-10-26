#pragma once

#include <filesystem>

#include "IPlayerController.h"
#include "Signal.h"


namespace fs = std::filesystem;


class EngineArenaController {
public:

    EngineArenaController(
        const fs::path& path,
        unsigned int numGames,
        IPlayerController& engine1,
        IPlayerController& engine2
    ); 

    void start(); 

private:

    struct GameResult {
        int winner = -1; // 0 = draw, 1 = engine1, 2 = engine2;
    };

private:

    void runSession(const fs::path& outputPath);

    GameResult runGame(unsigned int gameNum, const fs::path& dir);

private:

    fs::path outputPath;

    IPlayerController& engine1;

    IPlayerController& engine2;

    std::string engine1Name;

    std::string engine2Name;

    bool stop = false;

    Signal stoppedSignal;  

    unsigned int numGames;

};