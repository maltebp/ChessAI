#pragma once

#include "State.h"
#include "Move.h"


class IEngine {
public:

    virtual Move getMove(const State& state) = 0;
    
    virtual void reset() = 0;    

};