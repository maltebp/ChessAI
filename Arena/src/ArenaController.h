#pragma once

#include <filesystem>
#include <chrono>

#include "IPlayerController.h"
#include "Signal.h"


namespace fs = std::filesystem;


class ArenaController {
public:

    ArenaController(
        const fs::path& path,
        IPlayerController& engine1,
        IPlayerController& engine2,
        std::string tag
    ); 

    void start(); 

private:

    struct GameResult {
        
        int winner = -1; // 0 = draw, 1 = engine1, 2 = engine2;

        unsigned int halfTurns = 0; 

        unsigned int whitePlayerId = 0;
        
        unsigned int blackPlayerId = 0;

        unsigned int engine1PiecesLeft = 0;

        unsigned int engine2PiecesLeft = 0; 

        double engine1SearchTime = 0; // Seconds

        double engine2SearchTime = 0;

        std::string endState = ""; // FEN

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

    bool stopTestThread = false;

    bool testThreadHasStopped = false;
    
    std::string tag;

};