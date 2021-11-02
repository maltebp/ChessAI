#pragma once

#include "external/catch.hpp"

#include "Util.h"
#include "MoveUtil.h"


TEST_CASE("Promotion Moves: Generation", "[moves][castling][king][rook]") {
	State state;
	state.board[4][0] = Piece(PieceColor::WHITE, PieceType::KING);
	state.board[7][6] = Piece(PieceColor::WHITE, PieceType::PAWN);

	state.board[4][7] = Piece(PieceColor::BLACK, PieceType::KING);
	state.board[6][1] = Piece(PieceColor::BLACK, PieceType::PAWN);
	
	//White
	MoveUtil::GenerationList moves;
	MoveUtil::getAllMoves(state, moves);
	REQUIRE(moves.contains(Move{ {7,6}, {7,7}, PieceType::BISHOP}));
	REQUIRE(moves.contains(Move{ {7,6}, {7,7}, PieceType::KNIGHT}));
	REQUIRE(moves.contains(Move{ {7,6}, {7,7}, PieceType::QUEEN}));
	REQUIRE(moves.contains(Move{ {7,6}, {7,7}, PieceType::ROOK}));

	state.turn++;

	//Black
	MoveUtil::getAllMoves(state, moves);
	REQUIRE(moves.contains(Move{ {6,1}, {6,0}, PieceType::BISHOP }));
	REQUIRE(moves.contains(Move{ {6,1}, {6,0}, PieceType::KNIGHT }));
	REQUIRE(moves.contains(Move{ {6,1}, {6,0}, PieceType::QUEEN }));
	REQUIRE(moves.contains(Move{ {6,1}, {6,0}, PieceType::ROOK }));
}