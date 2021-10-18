#pragma once

#include "external/catch.hpp"

#include "Util.h"
#include "MoveUtil.h"

State createPromotionState() {
	State state;
	state.board[0][0] = Piece(PieceColor::WHITE, PieceType::ROOK);
	state.board[4][0] = Piece(PieceColor::WHITE, PieceType::KING);
	state.board[7][0] = Piece(PieceColor::WHITE, PieceType::ROOK);
	state.board[0][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[1][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[2][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[3][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[4][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
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
	return state;
}


TEST_CASE("Castling Moves: Generation", "[moves][castling][king][rook]") {
	State state = createPromotionState();
	
    std::vector<Move> moves = MoveUtil::getAllMoves(state);

	REQUIRE(Util::contains(moves, Move{ {4,0}, {6,0} }));
	REQUIRE(Util::contains(moves, Move{ {4,0}, {2,0} }));

    state.turn++;

    moves = MoveUtil::getAllMoves(state);

	REQUIRE(Util::contains(moves, Move{ {4,7}, {6,7} }));
	REQUIRE(Util::contains(moves, Move{ {4,7}, {2,7} }));
}

TEST_CASE("Castling Moves: Execution White Queenside ", "[moves][castling][king][rook]") {
	State state = createPromotionState();
	Move whiteCastlesQueenSide = Move{ {4,0}, {2,0} };
	state = MoveUtil::executeMove(state, whiteCastlesQueenSide);
	Piece whiteKing = state.board[2][0];

	//White king
	bool whiteKingInRightPlace = whiteKing.getColor() == PieceColor::WHITE 
		&& whiteKing.getType() == PieceType::KING;
	REQUIRE(whiteKingInRightPlace);
	REQUIRE(state.board[4][0].getType() == PieceType::NONE);

	//White rook
	Piece whiteRook = state.board[3][0];
	bool whiteRookInRightPlace = whiteRook.getColor() == PieceColor::WHITE
		&& whiteRook.getType() == PieceType::ROOK;
	REQUIRE(whiteRookInRightPlace);
	REQUIRE(state.board[0][0].getType() == PieceType::NONE);

	//Castling no more possible for white
	REQUIRE((state.whiteCanCastleKingSide == false && state.whiteCanCastleQueenSide == false));
}

TEST_CASE("Castling Moves: Execution White Kingside ", "[moves][castling][king][rook]") {
	State state = createPromotionState();
	Move whiteCastlesKingSide = Move{ {4,0}, {6,0} };
	state = MoveUtil::executeMove(state, whiteCastlesKingSide);
	Piece whiteKing = state.board[6][0];

	//White king
	bool whiteKingInRightPlace = whiteKing.getColor() == PieceColor::WHITE
		&& whiteKing.getType() == PieceType::KING;
	REQUIRE(whiteKingInRightPlace);
	REQUIRE(state.board[4][0].getType() == PieceType::NONE);

	//White rook
	Piece whiteRook = state.board[5][0];
	bool whiteRookInRightPlace = whiteRook.getColor() == PieceColor::WHITE
		&& whiteRook.getType() == PieceType::ROOK;
	REQUIRE(whiteRookInRightPlace);
	REQUIRE(state.board[7][0].getType() == PieceType::NONE);

	//Castling no more possible for white
	REQUIRE((state.whiteCanCastleKingSide == false && state.whiteCanCastleQueenSide == false));
}


TEST_CASE("Castling Moves: Execution Black queenside", "[moves][castling][king][rook]") {
	State state = createPromotionState();
	state.turn++;
	Move blackCastlesQueenSide = Move{ {4,7}, {2,7} };
	state = MoveUtil::executeMove(state, blackCastlesQueenSide);
	Piece blackKing = state.board[2][7];

	//Black king
	bool blackKingInRightPlace = blackKing.getColor() == PieceColor::BLACK
		&& blackKing.getType() == PieceType::KING;
	REQUIRE(blackKingInRightPlace);
	REQUIRE(state.board[4][7].getType() == PieceType::NONE);

	//Black rook
	Piece blackRook = state.board[3][7];
	bool blackRookInRightPlace = blackRook.getColor() == PieceColor::BLACK
		&& blackRook.getType() == PieceType::ROOK;
	REQUIRE(blackRookInRightPlace);
	REQUIRE(state.board[0][7].getType() == PieceType::NONE);

	//Castling no more possible for black
	REQUIRE((state.blackCanCastleKingSide == false && state.blackCanCastleQueenSide == false));
}

TEST_CASE("Castling Moves: Execution Black kingside", "[moves][castling][king][rook]") {
	State state = createPromotionState();
	state.turn++;
	Move blackCastlesKingSide = Move{ {4,7}, {6,7} };
	state = MoveUtil::executeMove(state, blackCastlesKingSide);
	Piece blackKing = state.board[6][7];

	//Black king
	bool blackKingInRightPlace = blackKing.getColor() == PieceColor::BLACK
		&& blackKing.getType() == PieceType::KING;
	REQUIRE(blackKingInRightPlace);
	REQUIRE(state.board[4][7].getType() == PieceType::NONE);

	//Black rook
	Piece blackRook = state.board[5][7];
	bool blackRookInRightPlace = blackRook.getColor() == PieceColor::BLACK
		&& blackRook.getType() == PieceType::ROOK;
	REQUIRE(blackRookInRightPlace);
	REQUIRE(state.board[7][7].getType() == PieceType::NONE);

	//Castling no more possible for black
	REQUIRE((state.blackCanCastleKingSide == false && state.blackCanCastleQueenSide == false));
}