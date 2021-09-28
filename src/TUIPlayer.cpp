#include "TUIPlayer.h"

#include <iostream>
#include <exception>
#include <sstream>
#include "Move.h"
#include "State.h"
#include "MoveUtil.h"


// x-grow, y-grow

void printMoves(const State& state) {

	std::vector<Move> moves = MoveUtil::getAllMoves(state);
	for (Move move : moves) {
		std::cout << move << std::endl;
	}
}


void printBoard(const State& state) {
	std::cout << std::endl;

	for( int y=0; y < 8; y++ ) {
		std::cout << '\t' << (8-y) << ' ';		
		for( int x=0; x < 8; x++ ) {
			Piece piece = state.board[x][7-y];
			bool isWhiteField = (x + 7-y) % 2;
			if( isWhiteField ) {
				std::cout << "\033[47;30m"; // 47 = white background, 30 = black text
				std::cout << piece.getAlgebraicChar() << ' ';
				std::cout << "\033[0m"; // Reset terminal colors
			}
			else {
				std::cout << piece.getAlgebraicChar() << ' ';
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


Move TUIPlayer::getMove(const State& state, const std::vector<Move>& moves) {

	std::string previousOutput = "";

	while( true ) {

		std::system("cls");

		printBoard(state);

		printMoves(state);

		if( !previousOutput.empty() ) {
			std::cout << "\n" << previousOutput << std::endl;;
		}

		std::cout << "\n>";
		std::string a;
		std::cin >> a;

		std::stringstream ss;
		ss << Position::fromAlgebraicNotation(a);

		previousOutput = ss.str();

		std::cout << "\n" << a;
	}

	return Move();
}