#pragma once

#include <filesystem>

#include "IPlayerController.h"


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

    void runGame(unsigned int gameNum, const fs::path& dir);

private:

    fs::path outputPath;



    IPlayerController& engine1;

    IPlayerController& engine2;

    unsigned int numGames;

};