#pragma once

#include <algorithm>
#include <limits>
#include <chrono>
#include <thread>
#include <cstdlib>

#include "DynamicAllocation.h"
#include "MoveUtil.h"
#include "MoveSorter.h"
#include "Zobrist.h"
#include "Transposition.h"
#include <Heuristic.h>


class MinMaxSearcher {
public:


	struct Result {
		Move bestMove;
		double searchTime = 0;
		unsigned long long nodesVisited = 0;
		double branchingFactor = 0;
		double cutOffFactor = 0; // Cut off per node
		unsigned long long checkmates = 0;
		unsigned long long draws = 0;
		unsigned long long staticEvaluations = 0;
		unsigned long long dynamicAllocations = 0;

	};
	

public:


	static void initialize() {
		initialized = true;
		Transposition::initTranspositionTable();	
	}


	static Result searchToDepth(const State& state, int depth, std::vector<unsigned long long>& previousStateHashes) {
		assert(initialized);
		assert(depth > 0);
		return iterativeSearch(state, false, previousStateHashes, depth);
	}


	static Result searchTimed(const State& state, long long searchTime, std::vector<unsigned long long>& previousStateHashes, bool useInterrupted = true) {
		assert(initialized);
		stopSearch = false;
		
		Result result;
		std::thread thread([&](){
			result = iterativeSearch(state, useInterrupted, previousStateHashes);
		});

		std::this_thread::sleep_for(std::chrono::milliseconds(searchTime));
		stopSearch = true;
		thread.join();

		return result;
	}


private:


	static void deleteElementsBetween(std::vector<Move>& v, size_t startIndex, size_t endIndex) {
		if( startIndex >= endIndex ) return;
		v.erase(v.begin() + startIndex, v.begin() + endIndex ); 
	}


	static Result iterativeSearch(const State& state, bool useInterrupted, std::vector<unsigned long long>& previousStateHashes, int depth = std::numeric_limits<int>::max()) {
		
		// Safety not to ensure we don't call this function recursively, or
		// in a multithreaded environment
		assert(!searching);
		searching = true;

	    unsigned long long numAllocationsAtStart = DynamicAllocation::numAllocations;
		MinMaxSearcher::previousStateHashes = previousStateHashes;
	    auto startTime = std::chrono::system_clock::now();

		Result finishedResult;

		for (int i = 1; i <= depth; i++) {
			if( stopSearch ) break;

			Transposition::nextCounter();

			Result currentResult = finishedResult;
			
			bool useMoveSequence = i > 1;

			auto [move, score] = searchInternal(
				state, 
				i,
				0,
				std::numeric_limits<int>::min(), std::numeric_limits<int>::max(),
				currentResult,
				useMoveSequence
			);

			if( (useInterrupted || !stopSearch) && move != Move() ){
				currentResult.bestMove = move;
				finishedResult = currentResult;
			}

			previousBestMoves = moveList;
			moveList.clear();
		}

    	auto endTime = std::chrono::system_clock::now();
    	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

		finishedResult.searchTime = elapsed.count() / 1000.0;
		finishedResult.dynamicAllocations = DynamicAllocation::numAllocations - numAllocationsAtStart;

		previousStateHashes = MinMaxSearcher::previousStateHashes;

		searching = false;

		return finishedResult;
	}


	static std::tuple<Move, int> noMovesPossibleScore(const State& state, int remainingDepth, Result& result) {
		//In this case it is either a draw of a loss or current player
		bool isMaximizer = state.turn % 2 == 0;
		PieceColor colorToMove = state.turn % 2 == 0 ? PieceColor::WHITE : PieceColor::BLACK;
		Position kingPosition = state.getPiecePosition({ colorToMove, PieceType::KING });
		bool kingIsThreathened = MoveUtil::isFieldThreatened(state, kingPosition, isMaximizer);

		if (kingIsThreathened) {
			//If king is threathened - it is check mate

			//Adjust score with depth, so quick mate is preferred no matter the other factors
			int scoreValue = MAX_SCORE - EVEN_LARGER_POINT_BONUS + (remainingDepth * VERY_LARGE_POINT_BONUS);
			int score = isMaximizer ? -scoreValue : scoreValue;

			result.checkmates++;

			return { Move(), score };
		}
		else {
			//Else it is a draw
			result.draws++;
			return { Move(), DRAW_SCORE };
		}
	}

	
	static std::tuple<Move, int> searchInternal(
		const State& state, 
		int remainingDepth, 
		int currentDepth, 
		int alpha, 
		int beta, 
		Result& result, 
		bool useMoveSequence
	) {
		//Bookkeeping
		size_t startIndex = moveList.size();
		result.nodesVisited++;
		bool isMaximizer = state.turn % 2 == 0;

		//--------------------------------BASE CASES---------------------------------------------------------------
		//Leaf node
		if (remainingDepth == 0) {
			bool mateOrStalemate = MoveUtil::anyMovePossible(state);
			if (mateOrStalemate) {
				return noMovesPossibleScore(state, remainingDepth, result);
			}

			result.staticEvaluations++;
			int score = Heuristic::danielsenHeuristic(state);
			return { Move(), score };
		}

		//Draw rules
		bool drawBy50Moves = state.drawCounter > 49;
		unsigned long long hash = Zobrist::calcHashValue(state.board);
		bool drawBy3FoldRep = getNumOfTimesContained(hash, previousStateHashes, isMaximizer) == 2;
		if (drawBy3FoldRep ||drawBy50Moves) {
			return { Move(), DRAW_SCORE };
		}
		
		//Check Transpotable
		if (Transposition::isEntry(hash)) {
			return { Move(), Transposition::getScore(hash)};
		}

		//----------------------------------SEARCH SUBTREE-------------------------------------------------------------
		previousStateHashes.push_back(hash);

		//Get all possible moves
		MoveUtil::GenerationList moves;
		MoveUtil::getAllMoves(state, moves);

		if (moves.size() == 0) {
			return noMovesPossibleScore(state, remainingDepth, result);
		}

		// Branching factor is average of all nodes
		double currentBranchingFactor = result.branchingFactor;
		result.branchingFactor =
			currentBranchingFactor + (moves.size() - currentBranchingFactor) / result.nodesVisited;

		Move bestMoveFromPrevious = Move();
		if (useMoveSequence) {
			if (previousBestMoves.size()>currentDepth) {
				bestMoveFromPrevious = previousBestMoves[currentDepth];
			}
		}

		//Sort the list of moves according to moveorder heuristic
		MoveSorter::sortMoves(state, moves, bestMoveFromPrevious);

		Move bestMove;
		for(int i=0; i<moves.size(); i++) {

			if( stopSearch ) break;

            Move move = moves[i];

			if (alpha >= beta) {
				size_t numCutOffs = moves.size() - i;
				double currentCutOffFactor = result.cutOffFactor;
				result.cutOffFactor =
					currentCutOffFactor + (numCutOffs - currentCutOffFactor) / result.nodesVisited;

				break;
			}

			size_t currentIndex = moveList.size();
			moveList.push_back(move);

			State resultState = MoveUtil::executeMove(state, move);

			bool useMoveSequenceAgain = useMoveSequence && move == bestMoveFromPrevious && remainingDepth -1 > 1;

			auto [resultMove, resultScore] = searchInternal(resultState, remainingDepth - 1, currentDepth+1, alpha, beta, result, useMoveSequenceAgain);
			
			// Considering a move from an interrupted branch may result in
			// not choosing the optimal move.
			if( stopSearch ) {
				deleteElementsBetween(moveList, currentIndex, moveList.size());
				break;
			}

			if (isMaximizer && resultScore > alpha) 
			{
				alpha = resultScore;
				bestMove = move;

				deleteElementsBetween(moveList, startIndex, currentIndex);
			} else if (!isMaximizer && resultScore < beta) 
			{
				beta = resultScore;
				bestMove = move;

				deleteElementsBetween(moveList, startIndex, currentIndex);
			} else {
				deleteElementsBetween(moveList, currentIndex, moveList.size());
			}

		}

		//Pop this state from previousStateHashes
		previousStateHashes.pop_back();
		int score = isMaximizer ? alpha : beta;
		//insert in transpotable
		Transposition::insertEntry(hash, score);
		return { bestMove,score };
	}

	static int getNumOfTimesContained(unsigned long long hash, std::vector<unsigned long long> hashes, bool whitesTurn) {
		//White only looks at even indices. Black only looks at uneven indices
		int startIndex = whitesTurn ? 0 : 1;
		int count = 0;
		for (int i = startIndex; i < hashes.size(); i = i+2) {
			if (hashes[i] == hash) {
				count++;
			}
		}
		return count;
	}

private:


	static inline std::vector<unsigned long long> previousStateHashes;

	static inline std::vector<Move> moveList;

	static inline std::vector<Move> previousBestMoves;

	static inline bool searching = false;

	static inline bool stopSearch = false;

	static inline bool initialized = true;

	constexpr static int DRAW_SCORE  = 0;

	constexpr static int VERY_LARGE_POINT_BONUS  = 10000;

	constexpr static int EVEN_LARGER_POINT_BONUS  = 1000000;

	constexpr static int MAX_SCORE = std::numeric_limits<int>::max() - 1;

	constexpr static int MIN_SCORE = std::numeric_limits<int>::min() + 1;

};


