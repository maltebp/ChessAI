#include <iostream>
#include <exception>

#include "State.h"



unsigned char getPieceChar(Piece piece) {
	char colorOffset = piece.getColor() == PieceColor::BLACK ? 32 : 0;

	switch(piece.getType()) {
		case PieceType::NONE:	return ' ';
		case PieceType::KING: 	return 'K' + colorOffset;
		case PieceType::QUEEN:	return 'Q' + colorOffset;
		case PieceType::ROOK: 	return 'R' + colorOffset;
		case PieceType::KNIGHT: return 'N' + colorOffset;
		case PieceType::PAWN: 	return 'P' + colorOffset;
		case PieceType::BISHOP: return 'B' + colorOffset;
	}
	
	throw std::exception("Unknown piece type");
}


void printBoard(const State& state) {
	std::cout << std::endl;

	for( int y=0; y < 8; y++ ) {
		std::cout << '\t' << (8-y) << ' ';		
		for( int x=0; x < 8; x++ ) {
			Piece piece = state.board[x][y];
			bool isWhiteField = (x + y) % 2;
			if( isWhiteField ) {
				std::cout << "\033[47;30m";
				std::cout << getPieceChar(piece) << ' ';
				std::cout << "\033[0m";
			}
			else {
				std::cout << getPieceChar(piece);
			}
		}
		std::cout << std::endl;	
	}

	std::cout << "\t  ";
	for( int x=0; x < 8; x++ ) {
		std::cout << (char)('a' + x) << ' ';
	}
	std::cout << std::endl;
}


int main(int arc, char** argv) {

	State state;

	state.board[0][0] = Piece(PieceColor::BLACK, PieceType::ROOK); 
	state.board[1][0] = Piece(PieceColor::BLACK, PieceType::KNIGHT); 
	state.board[2][0] = Piece(PieceColor::BLACK, PieceType::BISHOP); 
	state.board[3][0] = Piece(PieceColor::BLACK, PieceType::QUEEN); 
	state.board[4][0] = Piece(PieceColor::BLACK, PieceType::KING); 
	state.board[5][0] = Piece(PieceColor::BLACK, PieceType::BISHOP); 
	state.board[6][0] = Piece(PieceColor::BLACK, PieceType::KNIGHT); 
	state.board[7][0] = Piece(PieceColor::BLACK, PieceType::ROOK); 
	state.board[0][1] = Piece(PieceColor::BLACK, PieceType::PAWN); 
	state.board[1][1] = Piece(PieceColor::BLACK, PieceType::PAWN); 
	state.board[2][1] = Piece(PieceColor::BLACK, PieceType::PAWN); 
	state.board[3][1] = Piece(PieceColor::BLACK, PieceType::PAWN); 
	state.board[4][1] = Piece(PieceColor::BLACK, PieceType::PAWN); 
	state.board[5][1] = Piece(PieceColor::BLACK, PieceType::PAWN); 
	state.board[6][1] = Piece(PieceColor::BLACK, PieceType::PAWN); 
	state.board[7][1] = Piece(PieceColor::BLACK, PieceType::PAWN); 

	state.board[0][7] = Piece(PieceColor::WHITE, PieceType::ROOK);
	state.board[1][7] = Piece(PieceColor::WHITE, PieceType::KNIGHT);
	state.board[2][7] = Piece(PieceColor::WHITE, PieceType::BISHOP);
	state.board[3][7] = Piece(PieceColor::WHITE, PieceType::QUEEN);
	state.board[4][7] = Piece(PieceColor::WHITE, PieceType::KING);
	state.board[5][7] = Piece(PieceColor::WHITE, PieceType::BISHOP);
	state.board[6][7] = Piece(PieceColor::WHITE, PieceType::KNIGHT);
	state.board[7][7] = Piece(PieceColor::WHITE, PieceType::ROOK);
	state.board[0][6] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[1][6] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[2][6] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[3][6] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[4][6] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[5][6] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[6][6] = Piece(PieceColor::WHITE, PieceType::PAWN);
	state.board[7][6] = Piece(PieceColor::WHITE, PieceType::PAWN);

	printBoard(state);

	return 0;
}