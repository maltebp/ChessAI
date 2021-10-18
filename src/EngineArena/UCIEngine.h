#pragma once

#include <string>

#include "IPlayerController.h"
#include "Process.h"


class UCIEngine : public IPlayerController {
public:

    UCIEngine(const std::string& enginePath);

    ~UCIEngine();
    
    Move getMove(const State& state, const std::vector<Move>& validMoves) override;

private:

    void onEngineStdOutput(const std::string&);

    void onEngineStdError(const std::string&);

private:

    Process* process;

};