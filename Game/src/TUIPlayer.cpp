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


std::string TUIPlayer::getName() {
	return "TUI Player";
}


void TUIPlayer::start(std::ostream* outputStream, std::ostream* errorStream) {
	// TODO: TUIPlayer should use these streams instead of the other ones	
}


std::tuple<Move, std::string> parseInput(const State& state, const MoveUtil::GenerationList& moves, const std::vector<std::string> inputTokens) {
	
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

		if( !moves.contains(move) ) {
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
		for( size_t i = 0; i < 100; i++ ) {
			ss << "  " << moves[i] << "\n";
		}

		return { {}, ss.str() }; 
	}

	return { {}, "Error: Invalid command '" + inputTokens[0] + "'" };
}


Move TUIPlayer::getMove(const State& state, const MoveUtil::GenerationList& validMoves, const Move& lastMove) {

	while( true ) {

		std::cout << "\n>";
		std::string input;
		getline(std::cin, input);

		std::vector<std::string> inputTokens = Util::splitString(input, " \0\n\t");

		if( inputTokens.size() == 0 ) continue;

		auto [move,parseOutput] = parseInput(state, validMoves, inputTokens);
		if( parseOutput.empty() || parseOutput == "" ) {
			return move;
		}

		std::cout << parseOutput;		
	}

	return Move();
}