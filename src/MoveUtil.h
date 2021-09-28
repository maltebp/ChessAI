#pragma once
#include "Move.h"
#include "State.h" 
#include <vector>


namespace MoveUtil {
	std::vector<Move> getAllMoves(const State& state);
	State executeMove(State& oldState, Move move);
}