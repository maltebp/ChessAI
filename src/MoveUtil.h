#pragma once
#include "Move.h"
#include "State.h" 
#include <vector>


namespace MoveUtil {
	
	std::vector<Move> getAllMoves(const State& state);
	
	State executeMove(const State& oldState, Move move);

	// We compute if the player who does not have the turn
	// is in check in the given state
	bool isKingThreatened(const State& state);

}