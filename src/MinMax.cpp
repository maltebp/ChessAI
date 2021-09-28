#include "MoveUtil.h"
class MinMaxSearcher {
public:



};


class MinMaxState {
public:
    MinMaxState(State state, bool isMinimizer) {
        this->state = state;
        this->isMinimizer = isMinimizer;
    } 

    void generateChildren() {
        std::vector possibleMoves = MoveUtil::getAllMoves(state);
        
    }

private:
    State state;
    bool isMinimizer;
    std::vector<MinMaxState> children;


};

class IHeuristic {
public:
    virtual int eval(MinMaxState state) = 0;
};

class RandomHeuristic : IHeuristic{
    int eval(MinMaxState state) override {
        return rand() % 100;
    }
};