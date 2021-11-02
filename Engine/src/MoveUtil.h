#pragma once

#include <vector>
#include <array>

#include "FixedVector.h"
#include "Move.h"
#include "State.h" 


namespace MoveUtil {

	/**
	 * @brief	Centralized definition of a compile-time sized array, used for the output
	 * 			of the move generation. We estimate/assume that the move generation will
	 * 			never compute more moves than what this array may hold.
	 * 			Theoretically, it's not large enough to hold all moves for all states (see
	 * 			link below), but covers 99.99% of all cases.
	 * 
	 * 			https://www.chess.com/forum/view/fun-with-chess/what-chess-position-has-the-most-number-of-possible-moves
	 */
	using GenerationList = Util::FixedVector<Move,100>;

	
	void getAllMoves(const State& state, GenerationList& moves);
	
	State executeMove(const State& oldState, Move move);

	// We compute if the player who does not have the turn
	// is in check in the given state
	bool isKingThreatened(const State& state);

	//Gets the reachable fields for sliding piece without considering if king will be checked
	void getAllSliderPositionsForPiece(const State& state, Position oldPos, Piece piece, std::vector<Position>& positions);

	/**
	How many rooks are threathening this position?
	*/	
	bool isRooksThreathening(const State& state, Position pos, bool whitesPerspective);

	/**
	How many enemy bishops are threathening this position?
	*/
	bool isBishopThreathening(const State& state, Position pos, bool whitesPerspective);

	/**
	How many enemy knights are threathening this position?
	*/
	bool isKnightThreathening(const State& state, Position position, bool whitesPerspective);

	/**
	How many enemy pawns are threathening this position
	*/
	bool isPawnThreathening(const State& state, Position pos, bool whitesPerspective);

	bool isFieldThreatened(const State& state, const Position position, bool threatenedByBlack);

	int manhattanDistFromMiddle(const Position pos);
}