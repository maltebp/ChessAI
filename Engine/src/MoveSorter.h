#pragma once

#include "MoveUtil.h"
#include <algorithm>
#include <array>


static class MoveSorter {
	struct element {
		Move move;
		int benefit;
	};

	static int getStaticPieceValue(PieceType type) {
		switch (type) {
		case PieceType::QUEEN:
			return 900;
			break;
		case PieceType::KNIGHT:
			return 300;
			break;
		case PieceType::BISHOP:
			return 300;
			break;
		case PieceType::ROOK:
			return 500;
			break;
		case PieceType::PAWN:
			return 100;
			break;
		default:
			break;
		}
	}

	/**
	bool: indicates if it is a capture mvoe
	int: indicates relative "gain" calculated as agressor_value - victim_value
	DOES NOT TAKE CARE OF EN PASSANT AND CASTLING MOVES
	*/
	static std::tuple<bool, int> getMoveCaptureBenfit(const State& state, const Move& move) {
		Piece agressor = state[move.fromField];
		Piece victim = state[move.toField];
		int benefit = 0;

		bool isCapture = victim.getType() != PieceType::NONE;
		if (isCapture) {
			benefit = getStaticPieceValue(victim.getType()) - getStaticPieceValue(agressor.getType());
		}
		return { isCapture, benefit };
	}

public:

	static void sortMoves(const State& state, Util::PushableArray<Move>& moves, Move& bestMoveFromPrevious) {

		bool usingBestFromPrevious = bestMoveFromPrevious != Move();
		//Move bestFromPrevious to front if it is there
		if (usingBestFromPrevious) {
			bool foundIt = false;
			for (int i = 0; i < moves.size(); i++) {
				if (moves[i] == bestMoveFromPrevious) {
					foundIt = true;
					Move tmp = moves[0];
					moves[0] = bestMoveFromPrevious;
					moves[i] = tmp;
					break;
				}
			}
			if (!foundIt) {
				std::cout << "ERROR in sortMoves() - sought move was not present." << std::endl;
			}
		}
		
		int lastCaptureIndex = usingBestFromPrevious ? 1 : 0;
		int sortingStartIndex = lastCaptureIndex;
		std::array<element, 100> moveElements;
		//Move capture- moves to front
		for (int i = sortingStartIndex; i < moves.size(); i++) {
			Move move = moves[i];
			auto [isCaptureMove, benefit] = getMoveCaptureBenfit(state, move);
			moveElements[i] = { move, benefit };
			if (isCaptureMove) {
				struct element tmp = moveElements[lastCaptureIndex];
				moveElements[lastCaptureIndex] = moveElements[i];
				moveElements[i] = tmp;
				lastCaptureIndex++;
			}
		}

		//The first lastCaptureIndex elements should be sorted according to benefit
		auto start = moveElements.begin() + sortingStartIndex;
		auto end = moveElements.begin() + lastCaptureIndex;
		auto sortValue = [](const element& e1, const element& e2) {
			return e1.benefit > e2.benefit;
		};

		std::sort(start, end, sortValue);

		//overwrite moves with sorted list
		for (int i = sortingStartIndex; i < moves.size(); i++) {
			moves[i] = moveElements[i].move;
		}
	}
};