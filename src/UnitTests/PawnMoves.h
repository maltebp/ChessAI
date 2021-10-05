#pragma once

#include "external/catch.hpp"

#include "TestUtil.h"
#include "MoveUtil.h"


TEST_CASE("Pawn Moves: Start state", "[moves][pawn]") {

    State state = State::createDefault();
    std::vector<Move> moves = MoveUtil::getAllMoves(state);

    for( unsigned int i = 0; i < 8; i++ ) {
        REQUIRE(contains(moves, Move{ {i,1}, {i,2} }));
        REQUIRE(contains(moves, Move{ {i,1}, {i,3} }));
    }

    state.turn++;

    moves = MoveUtil::getAllMoves(state);

    for( unsigned int i = 0; i < 8; i++ ) {
        REQUIRE(contains(moves, Move{ {i,6}, {i,5} }));
        REQUIRE(contains(moves, Move{ {i,6}, {i,4} }));
    }
}