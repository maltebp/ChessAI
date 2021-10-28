#pragma once

#include <string>
#include <chrono>

#include "VariableSignal.h"
#include "Signal.h"
#include "IPlayerController.h"
#include "Process.h"


class UCIEngine : public IPlayerController {
public:

    UCIEngine(const std::string& name, const std::string& enginePath, long long searchTime);

    ~UCIEngine();

    std::string getName() override;

    void start(std::ostream* outputStream, std::ostream* errorStream) override;
    
    Move getMove(const State& state, const std::vector<Move>& validMoves, const Move& lastMove) override;

private:

    void startProcess();

    void writeToEngine(const std::string&);

    void onEngineStdOutput(const std::string&);

    void onEngineStdError(const std::string&);

private:

    std::string name;

    std::string enginePath;

    std::ostream* outputStream = nullptr;

    std::ostream* errorStream = nullptr;

    std::chrono::milliseconds searchTime;

    Process* process;

    Signal engineReadySignal;

    Signal uciOkSignal;

    VariableSignal<std::string> engineNameSignal;

    VariableSignal<std::string> engineAuthorSignal;

    VariableSignal<Move> bestMoveSignal;

};