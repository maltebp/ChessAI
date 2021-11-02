#pragma once

#include <string>
#include <chrono>
#include <filesystem>

#include "VariableSignal.h"
#include "Signal.h"
#include "IPlayerController.h"
#include "Process.h"


class UCIEngine : public IPlayerController {
public:

    UCIEngine(std::string name, std::filesystem::path enginePath, long long searchTime);

    ~UCIEngine();

    std::string getName() override;

    void start(std::ostream* outputStream, std::ostream* errorStream) override;
    
    Move getMove(const State& state, const MoveUtil::GenerationList& validMoves, const Move& lastMove) override;

private:

    void startProcess();

    void writeToEngine(const std::string&);

    void onEngineStdOutput(const std::string&);

    void onEngineStdError(const std::string&);

    void onProcessTerminated();

private:

    std::string name;

    std::filesystem::path enginePath;

    std::ostream* outputStream = nullptr;

    std::ostream* errorStream = nullptr;

    std::chrono::milliseconds searchTime;

    Util::Process* process;

    Util::Signal engineReadySignal;

    Util::Signal uciOkSignal;

    Util::VariableSignal<std::string> engineNameSignal;

    Util::VariableSignal<std::string> engineAuthorSignal;

    Util::VariableSignal<Move> bestMoveSignal;

};