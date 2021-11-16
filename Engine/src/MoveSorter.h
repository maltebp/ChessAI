#pragma once

#include "MoveUtil.h"
#include <algorithm>
#include <array>


class MoveSorter {
	struct element {
		Move move;
		int benefit;
	};

	const static int lastMovedCaptureBonus = 1000;

	static int getStaticPieceValue(PieceType type) {
		switch (type) {
		case PieceType::QUEEN:
			return 900;
		case PieceType::KNIGHT:
			return 300;
		case PieceType::BISHOP:
			return 300;
		case PieceType::ROOK:
			return 500;
		case PieceType::PAWN:
			return 100;
		}
		return 0;
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

			//Give bonus if captured piece is the last moved piece 
			bool captureLastMoved = state.lastMovedPiecePosition == move.toField;
			if (captureLastMoved) {
				benefit += lastMovedCaptureBonus;
			}
		}
		return { isCapture, benefit };
	}

public:

	static void sortMoves(
		const State& state, 
		MoveUtil::GenerationList& moves, 
		Move& bestMoveFromPrevious, 
		unsigned long long stateHash) 
	{

		Move transpoTableMove = Transposition::getMove(stateHash);

		bool usingBestMoveFromTranspo = transpoTableMove != Move();
		bool usingBestFromPrevious = bestMoveFromPrevious != Move();

		//Move bestFromPrevious or bestMoveFromTranspo to front if it is there 
		if (usingBestFromPrevious) {
			bringMoveToFront(moves, bestMoveFromPrevious);
		}
		else if (usingBestMoveFromTranspo) {
			bringMoveToFront(moves, transpoTableMove);
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

	static Move bringMoveToFront(MoveUtil::GenerationList& moves, Move& move) {
		bool foundIt = false;
		for (int i = 0; i < moves.size(); i++) {
			if (moves[i] == move) {
				foundIt = true;
				Move tmp = moves[0];
				moves[0] = move;
				moves[i] = tmp;
				break;
			}
		}

	}
};