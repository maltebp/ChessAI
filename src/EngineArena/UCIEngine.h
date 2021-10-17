#pragma once

#include <string>

#include "IEngine.h"


class UCIEngine : public IEngine {
public:

    UCIEngine(const std::string& enginePath);
    
    Move getMove(const State& state) override;

    void reset() override;

private:

    std::string enginePath;

    std::string engineName;

};