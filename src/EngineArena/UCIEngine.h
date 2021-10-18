#pragma once

#include <string>

#include "VariableSignal.h"
#include "Signal.h"
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

    Signal engineReadySignal;

    Signal uciOkSignal;

    VariableSignal<std::string> engineNameSignal;

    VariableSignal<std::string> engineAuthorSignal;

    VariableSignal<Move> bestMoveSignal;

};