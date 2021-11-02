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
	void sortMoves(const State& state, std::vector<Move>& moves) {
		int lastCaptureIndex = 0;
		std::array<element, 100> moveElements;
		//Move capture- moves to front
		for (int i = 0; i < moves.size(); i++) {
			Move move = moves[i];
			auto [isCaptureMove, benefit] = MoveUtil::getMoveCaptureBenfit(state, move);
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
