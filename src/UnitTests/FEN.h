#pragma once

#include "external/catch.hpp"

#include "State.h"
#include "MoveUtil.h"

void testFEN(const State& state, const std::string& fen) {
    REQUIRE(state.toFEN() == fen);
    REQUIRE(state == State(fen));
}


TEST_CASE("FEN conversion: Simple move sequence", "[fen]") {

    // Default state
    State state = State::createDefault();
    testFEN(state, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    // Move white Pawn
    state = MoveUtil::executeMove(state, { {4,1}, {4,3} });
    testFEN(state, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

    // Move black pawn
    state = MoveUtil::executeMove(state, { {2,6 }, {2,4} });
    testFEN(state, "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");

    // Move White Knight
    state = MoveUtil::executeMove(state, { {6,0}, {5,2} });
    testFEN(state, "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
}



TEST_CASE("FEN conversion: Castling", "[fen]") {

    // Default state
    State state = State::createDefault();

    state = MoveUtil::executeMove(state, { {6,1}, {6,2} }); // White: Pawn forward
    state = MoveUtil::executeMove(state, { {0,6}, {0,5} }); // Black: Pawn forward
    state = MoveUtil::executeMove(state, { {5,0}, {6,1} }); // White: Bishop diagonal
    state = MoveUtil::executeMove(state, { {1,6}, {1,5} }); // Black: Pawn forward
    state = MoveUtil::executeMove(state, { {6,0}, {7,2} }); // White: Knight forward
    state = MoveUtil::executeMove(state, { {2,6}, {2,5} }); // Black: Pawn forward
    state = MoveUtil::executeMove(state, { {4,0}, {6,0} }); // White: castle king side
    testFEN(state, "rnbqkbnr/3ppppp/ppp5/8/8/6PN/PPPPPPBP/RNBQ1RK1 b kq - 1 4");
}
