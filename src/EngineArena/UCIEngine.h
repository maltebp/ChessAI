#pragma once

#include <string>

#include "IEngine.h"
#include "Process.h"


class UCIEngine : public IEngine {
public:

    UCIEngine(const std::string& enginePath);

    ~UCIEngine();
    
    Move getMove(const State& state) override;

    void reset() override;

private:

    void onEngineStdOutput(const std::string&);

    void onEngineStdError(const std::string&);

private:

    Process* process;

};