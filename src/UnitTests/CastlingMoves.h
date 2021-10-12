#pragma once

#include "external/catch.hpp"

#include "TestUtil.h"
#include "MoveUtil.h"


TEST_CASE("Castling Moves: Generation", "[moves][castling][king][rook]") {

	State state;
	state.board[0][0] = Piece(PieceColor::WHITE, PieceType::ROOK);
	state.board[4][0] = Piece(PieceColor::WHITE, PieceType::KING);
	state.board[7][0] = Piece(PieceColor::WHITE, PieceType::ROOK);
	state.board[0][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[1][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[2][4] = Piece(PieceColor::BLACK, PieceType::ROOK);
	state.board[3][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[5][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[6][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[7][1] = Piece(PieceColor::WHITE, PieceType::PAWN);

	state.board[4][7] = Piece(PieceColor::BLACK, PieceType::KING);
	state.board[7][7] = Piece(PieceColor::BLACK, PieceType::ROOK);
	state.board[0][7] = Piece(PieceColor::BLACK, PieceType::ROOK);
	state.board[0][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
	state.board[1][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
	state.board[2][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
	state.board[3][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
	state.board[4][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
	state.board[5][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
	state.board[6][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
	state.board[7][6] = Piece(PieceColor::BLACK, PieceType::PAWN);

    std::vector<Move> moves = MoveUtil::getAllMoves(state);

	REQUIRE(contains(moves, Move{ {4,0}, {6,0} }));
	REQUIRE(contains(moves, Move{ {4,0}, {2,0} }));

    state.turn++;

    moves = MoveUtil::getAllMoves(state);

	REQUIRE(contains(moves, Move{ {4,7}, {6,7} }));
	REQUIRE(contains(moves, Move{ {4,7}, {2,7} }));
}