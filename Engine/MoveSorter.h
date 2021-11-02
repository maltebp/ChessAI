#pragma once

#include "MoveUtil.h"
#include <algorithm>
#include <array>

class MoveSorter {
	struct element {
		Move move;
		int benefit;
	};

public:


	int getStaticPieceValue(PieceType type) {
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
	std::tuple<bool, int> getMoveCaptureBenfit(const State& state, const Move& move) {
		Piece agressor = state[move.fromField];
		Piece victim = state[move.toField];
		int benefit = 0;

		bool isCapture = victim.getType() != PieceType::NONE;
		if (isCapture) {
			benefit = getStaticPieceValue(victim.getType()) - getStaticPieceValue(agressor.getType());
		}
		return { isCapture, benefit };
	}

	void sortMoves(const State& state, std::vector<Move>& moves) {
		int lastCaptureIndex = 0;
		std::array<element, 100> moveElements;
		//Move capture- moves to front
		for (int i = 0; i < moves.size(); i++) {
			Move move = moves[i];
			auto [isCaptureMove, benefit] = getMoveCaptureBenfit(state, move);
			moveElements[i] = { move, benefit};
			if (isCaptureMove) {
				struct element tmp = moveElements[lastCaptureIndex];
				moveElements[lastCaptureIndex] = moveElements[i];
				moveElements[i] = tmp;
				lastCaptureIndex++;
			}
		}

		//The first lastCaptureIndex elements should be sorted according to benefit
		auto start = moveElements.begin();
		auto end = moveElements.begin() + lastCaptureIndex;
		auto sortValue = [](const element& e1, const element& e2) {
			return e1.benefit > e2.benefit;
		};

		std::sort(start, end, sortValue);

		//overwrite moves with sorted list
		for (int i = 0; i < lastCaptureIndex; i++) {
			moves[i] = moveElements[i].move;
		}
	}
};
