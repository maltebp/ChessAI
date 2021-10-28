#pragma once

#include "external/catch.hpp"

#include "Util.h"
#include "MoveUtil.h"


TEST_CASE("Simple checkmate", "[checkmate]") {

    State state;
    state.blackCanCastleKingSide = false;
    state.blackCanCastleQueenSide = false;
    state.whiteCanCastleKingSide = false;
    state.whiteCanCastleQueenSide = false;
    state.turn = 1; // Blacks turn
    
    state.board[0][0] = { PieceColor::WHITE, PieceType::KING };
    state.board[7][4] = { PieceColor::WHITE, PieceType::ROOK };
    state.board[4][6] = { PieceColor::WHITE, PieceType::KNIGHT };

    state.board[7][6] = { PieceColor::BLACK, PieceType::KING };
    state.board[6][6] = { PieceColor::BLACK, PieceType::PAWN };

    std::vector<Move> moves = MoveUtil::getAllMoves(state);
    
    REQUIRE(moves.size() == 0);
}


// This case was experienced at git commit 6af9d1057eec06d1e0c2d184121b5258ac3c4dc2
TEST_CASE("Check", "[checkmate]") {

    State state;
    state.blackCanCastleKingSide = false;
    state.blackCanCastleQueenSide = false;
    state.whiteCanCastleKingSide = false;
    state.whiteCanCastleQueenSide = false;

    state.turn = 42;

    state.board[1][3] = { PieceColor::WHITE, PieceType::PAWN };
    state.board[7][2] = { PieceColor::WHITE, PieceType::PAWN };
    state.board[7][3] = { PieceColor::WHITE, PieceType::KING };

    state.board[0][0] = { PieceColor::BLACK, PieceType::KNIGHT };
    state.board[0][1] = { PieceColor::BLACK, PieceType::QUEEN };
    state.board[6][1] = { PieceColor::BLACK, PieceType::ROOK };

    state.board[6][4] = { PieceColor::BLACK, PieceType::PAWN }; // Threatens white king

    state.board[2][7] = { PieceColor::BLACK, PieceType::KING };

    std::vector<Move> moves = MoveUtil::getAllMoves(state);

    REQUIRE( Util::contains(moves, Move{{6,1}, {6,4}}) );
    REQUIRE( Util::contains(moves, Move{{7,3}, {6,4}}) );
    REQUIRE( Util::contains(moves, Move{{7,3}, {6,3}}) );
    REQUIRE( Util::contains(moves, Move{{7,3}, {7,4}}) );

    REQUIRE( moves.size() == 4 );
}