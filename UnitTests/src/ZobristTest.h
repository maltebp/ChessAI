#pragma once

#include "external/catch.hpp"

#include "Util.h"
#include "MoveUtil.h"
#include "Zobrist.h"


TEST_CASE("HashValue", "[Zobrist]") {

    Zobrist::initZobristTable();
    State state = State::createDefault();
    unsigned long long start = Zobrist::calcHashValue(state.board);
    
    state = MoveUtil::executeMove(state, { {1,0}, {2,2} });
    unsigned long long move = Zobrist::calcHashValue(state.board);
    state = MoveUtil::executeMove(state, { {1,7}, {2,5} });
    state = MoveUtil::executeMove(state, { {2,2}, {1,0} });
    state = MoveUtil::executeMove(state, { {2,5}, {1,7} });
    unsigned long long end = Zobrist::calcHashValue(state.board);
    REQUIRE(start == end);
    REQUIRE(start != move);
}
