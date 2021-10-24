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

	//Gets the reachable fields for sliding piece without considering if king will be checked
	std::vector<Position> getAllSliderPositionsForPiece(const State& state, Position oldPos, Piece piece);

}