#pragma once

#include "external/catch.hpp"

#include "Util.h"
#include "MoveUtil.h"


TEST_CASE("Pawn Moves: Start state", "[moves][pawn]") {

    State state = State::createDefault();
    MoveUtil::GenerationList moves;
    
    MoveUtil::getAllMoves(state, moves);

    for( char i = 0; i < 8; i++ ) {
        REQUIRE(moves.contains({ {i,1}, {i,2} }));
        REQUIRE(moves.contains({ {i,1}, {i,3} }));
    }

    state.turn++;
 
    MoveUtil::getAllMoves(state, moves);

    for( char i = 0; i < 8; i++ ) {
        REQUIRE(moves.contains(Move{ {i,6}, {i,5} }));
        REQUIRE(moves.contains(Move{ {i,6}, {i,4} }));
    }
}


TEST_CASE("En Passant", "[moves][pawn]") {

    State state;
    state.board[0][0] = { PieceColor::WHITE, PieceType::KING };
    state.board[7][7] = { PieceColor::BLACK, PieceType::KING };
    state.board[2][1] = { PieceColor::WHITE, PieceType::PAWN };
    state.board[3][3] = { PieceColor::BLACK, PieceType::PAWN };
    
    REQUIRE(state.enPassantTarget == Position());

    // Move white pawn two forward -> "enabling" en passant for black
    Move moveWhitePawn = { {2,1}, {2,3} };
    state = MoveUtil::executeMove(state, moveWhitePawn);

    // Check that black can en passant
    REQUIRE(state.enPassantTarget == Position{2,2});
    MoveUtil::GenerationList blackMoves;
    MoveUtil::getAllMoves(state, blackMoves);
    Move moveBlackPawn = Move{{3,3},{2,2}};
    REQUIRE(blackMoves.contains(moveBlackPawn));

    // Perform en passant
    state = MoveUtil::executeMove(state, moveBlackPawn);
    REQUIRE(state.board[2][3] == Piece());
    REQUIRE(state.board[2][2] == Piece{PieceColor::BLACK, PieceType::PAWN});
    REQUIRE(state.enPassantTarget == Position());
}