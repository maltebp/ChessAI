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


using TurnResult = IPlayerController::TurnResult;
using GameInfo = IPlayerController::GameInfo;


std::string TUIPlayer::getName() {
	return "TUI Player";
}


void TUIPlayer::start(std::ostream* outputStream, std::ostream* errorStream) {
	// TODO: TUIPlayer should use these streams instead of the other ones	
}


std::tuple<TurnResult, std::string> parseInput(const GameInfo& gameInfo, const std::vector<std::string> inputTokens) {
	
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

		if( !gameInfo.validMoves.contains(move) ) {
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
			ss << "  " << gameInfo.validMoves[i] << "\n";
		}

		return { {}, ss.str() }; 
	}

	
	if( command == "rev" ) {
		
		if( inputTokens.size() != 2 ) {
			return { {}, "Error: 'rev' takes exactly 1 argument" };
		}

		unsigned long numStatesToRevert = 0; 
		try {
			numStatesToRevert = std::stoul(inputTokens[1]);
		} catch (std::invalid_argument& e) {
			return { {}, "Error: invalid integer argument" };
		} catch (std::out_of_range& e) {
			return { {}, "Error: integer is too large" };
		}

		if( numStatesToRevert == 0 ) {
			return { {}, "Error: argument must be larger than 0" };
		}

		if( numStatesToRevert > gameInfo.previousStates.size() ) {
			std::stringstream ss;
			ss << "Error: " << " cannot revert more than " << gameInfo.previousStates.size() << " states" << std::endl;
			return { {}, ss.str() };
		}

		State targetState = gameInfo.previousStates[gameInfo.previousStates.size() - numStatesToRevert];
		std::stringstream ss;
		ss << "Reverting to state: " << targetState.toFEN() << std::endl; 

		TurnResult turnResult;
		turnResult.numStatesToRevert = (unsigned int)numStatesToRevert;
		return { turnResult, "" };
	}

	return { {}, "Error: Invalid command '" + inputTokens[0] + "'" };
}


TurnResult TUIPlayer::giveTurn(const GameInfo& gameInfo) {

	while( true ) {

		std::cout << "\n>";
		std::string input;
		getline(std::cin, input);

		std::vector<std::string> inputTokens = Util::splitString(input, " \0\n\t");

		if( inputTokens.size() == 0 ) continue;

		auto [turnResult, parseOutput] = parseInput(gameInfo, inputTokens);
		if( parseOutput.empty() || parseOutput == "" ) {
			return turnResult;
		}

		std::cout << parseOutput;		
	}

	return TurnResult();
}