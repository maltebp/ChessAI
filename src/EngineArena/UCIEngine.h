#pragma once

#include <string>
#include <thread>

#include "IEngine.h"
#include "Process.h"


class UCIEngine : public IEngine {
public:

    UCIEngine(const std::string& enginePath);

    ~UCIEngine();
    
    Move getMove(const State& state) override;

    void reset() override;

private:

    void readOutput();

private:

    Process* process;

};