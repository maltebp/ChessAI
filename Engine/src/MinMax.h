#pragma once

#include <algorithm>
#include <limits>
#include <chrono>
#include <cstdlib>

#include "MoveUtil.h"


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

	};
	

public:


	static Result search(const State& state, int depth) {
		Result result;

	    auto startTime = std::chrono::system_clock::now();

		auto [move, score] = searchInternal(state, depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), result);

    	auto endTime = std::chrono::system_clock::now();
    	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

		result.bestMove = move;		
		result.searchTime = elapsed.count() / 1000.0;

		return result;
	}


private:

	
	static std::tuple<Move, int> searchInternal(const State& state, int depth, int alpha, int beta, Result& result) {

		result.nodesVisited++;

		//Base case: Leaf node
		if (depth == 0) {
			result.staticEvaluations++;
			int score = danielsenHeuristic(state);
			return { Move(), score };
		}

		bool isMaximizer = state.turn % 2 == 0;

		//Get all possible moves
		std::vector<Move> moves = MoveUtil::getAllMoves(state);

		if (moves.size() == 0) {
			//In this case it is either a draw of a loss or current player
			PieceColor colorToMove = isMaximizer ? PieceColor::WHITE : PieceColor::BLACK;
			Position kingPosition = state.getPiecePosition({ colorToMove, PieceType::KING });
			bool kingIsThreathened = MoveUtil::isFieldThreatened(state, kingPosition, isMaximizer);

			if (kingIsThreathened) {
				//If king is threathened - it is check mate

				//Adjust score with depth, so quick mate is preferred no matter the other factors
				int scoreValue = MAX_SCORE - EVEN_LARGER_POINT_BONUS + (depth *VERY_LARGE_POINT_BONUS);
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

		// Branching factor is average of all nodes
		double currentBranchingFactor = result.branchingFactor;
		result.branchingFactor =
			currentBranchingFactor + (moves.size() - currentBranchingFactor) / result.nodesVisited;

		Move bestMove;
		for(int i=0; i<moves.size(); i++) {
            Move move = moves[i];

			if (alpha >= beta) {
				unsigned int numCutOffs = moves.size() - i;
				double currentCutOffFactor = result.cutOffFactor;
				result.cutOffFactor =
					currentCutOffFactor + (numCutOffs - currentCutOffFactor) / result.nodesVisited;

				break;
			}

			State resultState = MoveUtil::executeMove(state, move);

			auto [resultMove, resultScore] = searchInternal(resultState, depth - 1, alpha, beta, result);
			if (isMaximizer && resultScore > alpha) 
			{
				alpha = resultScore;
				bestMove = move;
			} else if (!isMaximizer && resultScore < beta) 
			{
				beta = resultScore;
				bestMove = move;
			}

		}

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
		int sum = 0;
		int piecesLeft = 0;
		Position blackKingPos, whiteKingPos;
		int whiteSum = 0;
		int blackSum = 0;
		int whitesMinorPieceThreaths = 0;
		int blacksMinorPieceThreaths = 0;

		//Pieces and their positional values
		int current;
		for (unsigned int i = 0; i < 8; i++) {
			for (unsigned int j = 0; j < 8; j++) {
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
					int controlledSquares = MoveUtil::getAllSliderPositionsForPiece(state, pos, piece).size();
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
					int controlledSquares = MoveUtil::getAllSliderPositionsForPiece(state, pos, piece).size();
					current = (int)(500 + 1.5 * controlledSquares);
					minorPieceThreathening = MoveUtil::isBishopThreathening(state, pos, whitePiece)
						|| MoveUtil::isKnightThreathening(state, pos, whitePiece)
						|| MoveUtil::isPawnThreathening(state, pos, whitePiece);
					piecesLeft++;
					break;
				}
				case PieceType::BISHOP:
				{
					int controlledSquares = MoveUtil::getAllSliderPositionsForPiece(state, pos, piece).size();
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
					current = 100 + pawnFieldValuesForWhite[pos.x][yvalue];

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
				if (whitePiece) {
					whiteSum += current;
				}
				else {
					blackSum += current;
				}
			}
		}

		//Castling
		sum -= state.blackHasCastled * 16;
		sum += state.whiteHasCastled * 16;

		//Minor pieces threaths
		sum += minorPiecesThreatheningPoints(whitesMinorPieceThreaths);
		sum -= minorPiecesThreatheningPoints(blacksMinorPieceThreaths);

		//This part is from original Danielsen Heuristic, added to handle endgame better
		//---------------------------------------------------
		bool isEndgame = (whiteSum <= ENDGAME_SCORE_THRESHOLD || blackSum <= ENDGAME_SCORE_THRESHOLD);
		if (isEndgame) {

			int whiteDistFromCenter = MoveUtil::manhattanDistFromMiddle(whiteKingPos);
			int blackDistFromCenter = MoveUtil::manhattanDistFromMiddle(blackKingPos);
			whiteSum -= 1 * whiteDistFromCenter;
			blackSum -= 1 * blackDistFromCenter;
			sum += -1 * whiteDistFromCenter;
			sum -= -1 * blackDistFromCenter;

			bool isMating = (whiteSum <= MATING_SCORE_THRESHOLD || blackSum <= MATING_SCORE_THRESHOLD);
			if (isMating) {
				int distBetweenKings = std::abs((int)whiteKingPos.x-(int)blackKingPos.x) + std::abs((int)whiteKingPos.y - (int)blackKingPos.y);

				if (whiteSum >= blackSum) {//white winning
					sum -= -8 * blackDistFromCenter;
					sum += -2 * distBetweenKings;
				}
				else { //black winning
					sum += -8 * whiteDistFromCenter;
					sum -= -2 * distBetweenKings;

				}

			}
		}

		//---------------------------------------------------

		return sum;
	}

	static int minorPiecesThreatheningPoints(int num) {
		return num == 0 ? 2 : num == 1 ? -10 : -50;
	}

private:

	constexpr static int ENDGAME_WINNER_SCORE_THRESHOLD  = 500;

	constexpr static int ENDGAME_SCORE_THRESHOLD  = 1500;

	constexpr static int MATING_SCORE_THRESHOLD = 600;

	constexpr static int DRAW_SCORE  = 0;

	constexpr static int VERY_LARGE_POINT_BONUS  = 10000;

	constexpr static int EVEN_LARGER_POINT_BONUS  = 1000000;

	constexpr static int MAX_SCORE = std::numeric_limits<int>::max() - 1;

	constexpr static int MIN_SCORE = std::numeric_limits<int>::min() + 1;

};


