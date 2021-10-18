#include "TUIPlayer.h"

#include <iostream>
#include <exception>
#include <sstream>
#include <tuple>

#include "Util.h"
#include "Move.h"
#include "State.h"
#include "MoveUtil.h"
#include "MinMax.h"


void printMoves(const State& state) {

	std::vector<Move> moves = MoveUtil::getAllMoves(state);
	for (Move move : moves) {
		std::cout << move << std::endl;
	}
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

	while( true ) {

		std::cout << "\n>";
		std::string input;
		getline(std::cin, input);

		std::vector<std::string> inputTokens = Util::splitString(input, " \0\n\t");

		if( inputTokens.size() == 0 ) continue;

		auto [move,parseOutput] = parseInput(state, moves, inputTokens);
		if( parseOutput.empty() || parseOutput == "" ) {
			return move;
		}

		std::cout << parseOutput;		
	}

	return Move();
}