#pragma once

#include <algorithm>
#include <limits>
#include <chrono>

#include "DynamicAllocation.h"
#include "MoveUtil.h"
#include "MoveSorter.h"
#include "Zobrist.h"



const static double pawnFieldValuesForWhite[8][8] = {
		{0,-2,-4,-4,-3,8,23,0},
		{0,0,-1,0,2,14,30,0},
		{0,3,3.5,6,9.5,23,41.5,0},
		{0,4,5,8,12,26,44,0},
		{0,5,6.5,10,14.5,29,47.5,0},
		{0,1,0.5,2,4.5,17,33.5,0},
		{0,-2,-4,-4,-3,8,23,0},
		{0,-2,-4,-4,-3,8,23,0},
};


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


	static Result search(const State& state, int depth, std::vector<unsigned long long> prevStates) {
		Result result;

	    unsigned long long numAllocationsAtStart = DynamicAllocation::numAllocations;
		previousStateHashes = prevStates;
	    auto startTime = std::chrono::system_clock::now();
		Move bestMove;
		for (int i = 1; i <= depth; i++) {
			bool useMoveSequence = i > 1;
			auto [move, score] = searchInternal(
				state, 
				i, 
				0, 
				std::numeric_limits<int>::min(), 
				std::numeric_limits<int>::max(), 
				result, 
				useMoveSequence
			);
			bestMove = move;
			previousBestMoves = moveList;
			moveList.clear();
		}

    	auto endTime = std::chrono::system_clock::now();
    	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

		result.bestMove = bestMove;		
		result.searchTime = elapsed.count() / 1000.0;
		result.dynamicAllocations = DynamicAllocation::numAllocations - numAllocationsAtStart;

		prevStates = previousStateHashes;

		return result;
	}


private:

	static void deleteElementsBetween(std::vector<Move>& v, size_t startIndex, size_t endIndex) {
		if( startIndex >= endIndex ) return;
		v.erase(v.begin() + startIndex, v.begin() + endIndex ); 
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
			int score = danielsenHeuristic(state);
			return { Move(), score };
		}

		//Draw rules
		bool drawBy50Moves = state.drawCounter > 49;
		unsigned long long hash = Zobrist::calcHashValue(state.board);
		bool drawBy3FoldRep = getNumOfTimesContained(hash, previousStateHashes, isMaximizer) == 2;
		if (drawBy3FoldRep ||drawBy50Moves) {
			return { Move(), DRAW_SCORE };
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
			bestMoveFromPrevious = previousBestMoves[currentDepth];
		}

		//Sort the list of moves according to moveorder heuristic
		MoveSorter::sortMoves(state, moves, bestMoveFromPrevious);

		Move bestMove;
		for(int i=0; i<moves.size(); i++) {
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
			auto [resultMove, resultScore] = searchInternal(
				resultState, 
				remainingDepth - 1, currentDepth+1, 
				alpha, 
				beta, 
				result, 
				useMoveSequenceAgain);
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
		return { bestMove,score };
	}


	static int randomHeuristic(const State& state) {
		return rand() % 100;
	}


	static int simpleHeuristic(const State& state) {
		int sum = 0;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				Piece piece = state.board[i][j];
				int sign = piece.getColor() == PieceColor::WHITE ? 1 : -1;
				switch (piece.getType())
				{
				case PieceType::KING:
					sum += sign * 1000;
					break;
				case PieceType::QUEEN:
					sum += sign * 9;
					break;
				case PieceType::ROOK:
					sum += sign * 5;
					break;
				case PieceType::BISHOP:
					sum += sign * 3;
					break;
				case PieceType::KNIGHT:
					sum += sign * 3;
					break;
				case PieceType::PAWN:
					sum += sign * 1;
					break;
				case PieceType::NONE:
				default:
					break;
				}
			}
		}

		return sum;
	}

	static int danielsenHeuristic(const State& state) {
		static std::vector<Position> slidingPositions;
		

		int sum = 0;
		int piecesLeft = 0;
		Position blackKingPos, whiteKingPos;

		int whitesMinorPieceThreaths = 0;
		int blacksMinorPieceThreaths = 0;

		//Pieces and their positional values
		int current;
		for (unsigned int i = 0; i < 8; i++) {
			for (unsigned int j = 0; j < 8; j++) {
				slidingPositions.clear();
				current = 0;
				Piece piece = state.board[i][j];
				int sign = piece.getColor() == PieceColor::WHITE ? 1 : -1;
				bool whitePiece = sign == 1;
				Position pos = { i,j };
				bool minorPieceThreathening = false;
				switch (piece.getType())
				{
				case PieceType::KING:
				{
					//This will be usefull if we are in endgame 
					if (piece.getColor() == PieceColor::WHITE) {
						whiteKingPos = { i,j };
					}
					else {
						blackKingPos = { i,j };
					}
					piecesLeft++;
					break;
				}
				case PieceType::QUEEN:
				{
					MoveUtil::getAllSliderPositionsForPiece(state, pos, piece, slidingPositions);
					int controlledSquares = (int) slidingPositions.size();
					current = 900 + 1 * controlledSquares;
					minorPieceThreathening = MoveUtil::isRooksThreathening(state, pos, whitePiece)
						|| MoveUtil::isBishopThreathening(state, pos, whitePiece)
						|| MoveUtil::isKnightThreathening(state, pos, whitePiece)
						|| MoveUtil::isPawnThreathening(state, pos, whitePiece);
					piecesLeft++;

					break;
				}
				case PieceType::ROOK:
				{
					MoveUtil::getAllSliderPositionsForPiece(state, pos, piece, slidingPositions);
					int controlledSquares = (int) slidingPositions.size();
					current = (int)(500 + 1.5 * controlledSquares);
					minorPieceThreathening = MoveUtil::isBishopThreathening(state, pos, whitePiece)
						|| MoveUtil::isKnightThreathening(state, pos, whitePiece)
						|| MoveUtil::isPawnThreathening(state, pos, whitePiece);
					piecesLeft++;
					break;
				}
				case PieceType::BISHOP:
				{
					MoveUtil::getAllSliderPositionsForPiece(state, pos, piece, slidingPositions);
					int controlledSquares = (int) slidingPositions.size();
					current = 300 + 2 * controlledSquares;
					minorPieceThreathening = MoveUtil::isPawnThreathening(state, pos, whitePiece);
					piecesLeft++;
					break;
				}
				case PieceType::KNIGHT:
				{
					int distFromCenter = MoveUtil::manhattanDistFromMiddle(pos);
					current = 300 + 3 * (4 - distFromCenter);

					minorPieceThreathening = MoveUtil::isPawnThreathening(state, pos, whitePiece);
					piecesLeft++;
					break;
				}
				case PieceType::PAWN:
				{
					//This conversion mirrors the field-positions-board, if piece is black
					int yvalue = piece.getColor() == PieceColor::WHITE ? pos.y : 7 - pos.y;
					current = 100 + (int) pawnFieldValuesForWhite[pos.x][yvalue];

					//Double pawns - check if there is a pawn of my own color in front of me
					//(It doesn't matter that we look in the same way for black and white, and we cannot go out of the board this way)
					Piece pieceInFront = state.board[pos.x][pos.y + 1];
					bool isPawnOfSameColor = pieceInFront.getColor() == piece.getColor() && pieceInFront.getType() == piece.getType();
					if (isPawnOfSameColor) {
						//We have a double pawn
						current += 8;
					}
					piecesLeft++;
					break;
				}
				case PieceType::NONE:
					break;
				}

				//Update minor-piece-threath counters
				if (whitePiece && minorPieceThreathening) {
					//If it is a white piece and it is threathened by a minor piece
					whitesMinorPieceThreaths++;
				}
				else if (minorPieceThreathening) {
					//If it is a black piece and it is threathened by a minor piece
					blacksMinorPieceThreaths++;
				}

				sum += sign * current;
			}
		}

		//Castling
		sum -= state.blackHasCastled * 16;
		sum += state.whiteHasCastled * 16;

		//Minor pieces threaths
		sum += minorPiecesThreatheningPoints(whitesMinorPieceThreaths);
		sum -= minorPiecesThreatheningPoints(blacksMinorPieceThreaths);

		//This part is not from original Danielsen Heuristic, but added to handle endgame better
		//---------------------------------------------------
		bool isEndgame = piecesLeft < ENDGAME_PIECES_THRESHOLD;
		if (isEndgame) {
			//Best to keep the king in the middle - effect is stronger, the fewer pieces are left
			int whiteKingDist = MoveUtil::manhattanDistFromMiddle(whiteKingPos);
			int blackKingDist = MoveUtil::manhattanDistFromMiddle(blackKingPos);

			//Award white points for black kings distance
			sum += (ENDGAME_PIECES_THRESHOLD - piecesLeft) * blackKingDist * 10;

			//Award black points for white kings distance
			sum -= (ENDGAME_PIECES_THRESHOLD - piecesLeft) * whiteKingDist * 10;
		}

		//---------------------------------------------------

		return sum;
	}

	static int minorPiecesThreatheningPoints(int num) {
		return num == 0 ? 2 : num == 1 ? -10 : -50;
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

	constexpr static int ENDGAME_WINNER_SCORE_THRESHOLD  = 500;

	constexpr static int ENDGAME_PIECES_THRESHOLD  = 8;

	constexpr static int DRAW_SCORE  = 0;

	constexpr static int VERY_LARGE_POINT_BONUS  = 10000;

	constexpr static int EVEN_LARGER_POINT_BONUS  = 1000000;

	constexpr static int MAX_SCORE = std::numeric_limits<int>::max() - 1;

	constexpr static int MIN_SCORE = std::numeric_limits<int>::min() + 1;

};


