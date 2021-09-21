#include "Piece.h"


struct State {

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

};