#pragma once

#include <cassert>

#include "Piece.h"

class State {
public:

	int turn = 0;

    Piece board[8][8];

    static State createDefault() {
		State state;
		state.board[0][0] = Piece(PieceColor::WHITE, PieceType::ROOK);
		state.board[1][0] = Piece(PieceColor::WHITE, PieceType::KNIGHT);
		state.board[2][0] = Piece(PieceColor::WHITE, PieceType::BISHOP);
		state.board[3][0] = Piece(PieceColor::WHITE, PieceType::QUEEN);
		state.board[4][0] = Piece(PieceColor::WHITE, PieceType::KING);
		state.board[5][0] = Piece(PieceColor::WHITE, PieceType::BISHOP);
		state.board[6][0] = Piece(PieceColor::WHITE, PieceType::KNIGHT);
		state.board[7][0] = Piece(PieceColor::WHITE, PieceType::ROOK);
		state.board[0][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
		state.board[1][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
		state.board[2][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
		state.board[3][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
		state.board[4][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
		state.board[5][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
		state.board[6][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
		state.board[7][1] = Piece(PieceColor::WHITE, PieceType::PAWN);


		state.board[0][7] = Piece(PieceColor::BLACK, PieceType::ROOK);
		state.board[1][7] = Piece(PieceColor::BLACK, PieceType::KNIGHT);
		state.board[2][7] = Piece(PieceColor::BLACK, PieceType::BISHOP);
		state.board[3][7] = Piece(PieceColor::BLACK, PieceType::QUEEN);
		state.board[4][7] = Piece(PieceColor::BLACK, PieceType::KING);
		state.board[5][7] = Piece(PieceColor::BLACK, PieceType::BISHOP);
		state.board[6][7] = Piece(PieceColor::BLACK, PieceType::KNIGHT);
		state.board[7][7] = Piece(PieceColor::BLACK, PieceType::ROOK);
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

	static State createAlmostCheckState() {
		State state;
		state.board[0][0] = Piece(PieceColor::WHITE, PieceType::ROOK);
		state.board[1][0] = Piece(PieceColor::WHITE, PieceType::ROOK);

		state.board[0][5] = Piece(PieceColor::BLACK, PieceType::KNIGHT);
		state.board[0][7] = Piece(PieceColor::BLACK, PieceType::KING);
		state.turn = 1;

		return state;
	}


	static State createCheckState() {
		State state;
		state.board[0][0] = Piece(PieceColor::WHITE, PieceType::ROOK);
		state.board[1][0] = Piece(PieceColor::WHITE, PieceType::KNIGHT);
		state.board[2][0] = Piece(PieceColor::WHITE, PieceType::BISHOP);
		state.board[3][0] = Piece(PieceColor::WHITE, PieceType::QUEEN);
		state.board[4][0] = Piece(PieceColor::WHITE, PieceType::KING);
		state.board[5][0] = Piece(PieceColor::WHITE, PieceType::BISHOP);
		state.board[6][0] = Piece(PieceColor::WHITE, PieceType::KNIGHT);
		state.board[7][0] = Piece(PieceColor::WHITE, PieceType::ROOK);
		state.board[0][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
		state.board[1][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
		state.board[2][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
		state.board[3][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
		state.board[4][2] = Piece(PieceColor::BLACK, PieceType::ROOK);
		// state.board[4][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
		state.board[5][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
		state.board[6][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
		state.board[7][1] = Piece(PieceColor::WHITE, PieceType::PAWN);

		state.board[1][7] = Piece(PieceColor::BLACK, PieceType::KNIGHT);
		state.board[2][7] = Piece(PieceColor::BLACK, PieceType::BISHOP);
		state.board[3][7] = Piece(PieceColor::BLACK, PieceType::QUEEN);
		state.board[4][7] = Piece(PieceColor::BLACK, PieceType::KING);
		state.board[5][7] = Piece(PieceColor::BLACK, PieceType::BISHOP);
		state.board[6][7] = Piece(PieceColor::BLACK, PieceType::KNIGHT);
		state.board[7][7] = Piece(PieceColor::BLACK, PieceType::ROOK);
		state.board[0][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
		state.board[1][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
		state.board[2][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
		state.board[3][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
		state.board[4][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
		state.board[5][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
		state.board[6][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
		state.board[7][6] = Piece(PieceColor::BLACK, PieceType::PAWN);

		//state.turn = 1;

		return state;
    }


	Piece getPiece(Position position) const {
		assert(position.x < 8);
		assert(position.y < 8);
		return board[position.x][position.y];
	}


	Position getPiecePosition(const Piece& piece) const {
		for(unsigned int x=0; x<8; x++) {
			for(unsigned int y=0; y<8; y++) {
				if (board[x][y] == piece) {
					return {x,y};
				}
			}
		}
		return Position();
	}


	PieceColor getTurnColor() const {
		return turn % 2 ? PieceColor::BLACK: PieceColor::WHITE;
	}

};