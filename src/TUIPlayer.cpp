#include "TUIPlayer.h"
#include <iostream>
#include <exception>
#include <sstream>
#include "Move.h"
#include "State.h"
#include "MoveUtil.h"
#include "MinMax.h"


std::vector<std::string> splitInput(const std::string& input) {
	std::vector<std::string> tokens;
	std::stringstream ss;

	auto addToken = [&](){
		std::string token = ss.str();
		if( token.length() > 0 ) {
			tokens.push_back(token);
		}
		ss.str("");
	};
		
	for( auto c : input ) {
		if(c == ' ' || c == '\t' || c == '\n' || c == '\0') {
			addToken();
		}
		else {
			ss << c;
		}
	}

	addToken();

	return tokens;
}


// x-grow, y-grow

void printMoves(const State& state) {

	std::vector<Move> moves = MoveUtil::getAllMoves(state);
	for (Move move : moves) {
		std::cout << move << std::endl;
	}
}


void printBoard(const State& state) {
	std::cout << "\n";

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
		std::cout << "\n";	
	}

	std::cout << "\t  ";
	for( int x=0; x < 8; x++ ) {
		std::cout << (char)('a' + x) << ' ';
	}
	std::cout << std::endl;
}


std::tuple<Move, std::string> parseInput(const State& state, const std::vector<Move>& moves, const std::vector<std::string> inputTokens) {
	
	std::string command = inputTokens[0];

	if( command == "mv" ) {
		if( inputTokens.size() != 3 ) {
			return { {}, "Error: 'mv' needs 2 arguments"};
		}

		Position fromPosition;
		if( !Position::fromAlgebraicNotation(inputTokens[1], fromPosition) ){
			return { {}, "Error: 'mv' origin position is invalid"};
		}

		Position toPosition;
		if( !Position::fromAlgebraicNotation(inputTokens[2], toPosition) ){
			return { {}, "Error: 'mv' target position is invalid"};
		}

		Move move = { fromPosition, toPosition };

		if( std::find(moves.begin(), moves.end(), move) == moves.end() ) {
			return { {}, "Error: Invalid move" };
		}

		return { move, "" };
	}

	if( command == "mvs" ) {
		
		if( inputTokens.size() != 1 ) {
			return { {}, "Error: 'mvs' does not take any arguments" };
		}

		std::stringstream ss;
		ss << "Valid moves:\n";
		for(auto move : moves) {
			ss << "  " << move << "\n";
		}

		return { {}, ss.str() }; 
	}

	return { {}, "Error: Invalid command '" + inputTokens[0] + "'" };
}


Move TUIPlayer::getMove(const State& state, const std::vector<Move>& moves) {

	std::string output = "";

	while( true ) {

		std::system("cls");

		printBoard(state);

		std::cout << '\n';
		std::cout << "Turn: " << (state.turn+1) << "\n";
		std::cout << "Player: " << (state.turn % 2 == 0 ? "White" : "Black") << "\n";

		if( !output.empty() && output != "" ) {
			std::cout << "\n" << output << std::endl;;
		}

		std::cout << "\n>";
		std::string input;
		getline(std::cin, input);

		std::vector<std::string> inputTokens = splitInput(input);

		if( inputTokens.size() == 0 ) continue;

		if( inputTokens[0] == "clear" ) {
			output = "";
			continue;
		}

		auto [move,parseOutput] = parseInput(state, moves, inputTokens);
		if( parseOutput.empty() || parseOutput == "" ) {
			return move;
		}

		output = parseOutput;		
	}

	return Move();
}