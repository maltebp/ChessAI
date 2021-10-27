#include "MoveUtil.h"

#include <algorithm>
#include <limits>

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

	static Move search(const State& state, int depth) {
		auto [move, score] = searchInternal(state, depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
		return move;
	}

private:
	
	static std::tuple<Move, int> searchInternal(const State& state, int depth, int alpha, int beta) {

		//Base case: Leaf node
		if (depth == 0) {
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
				//If king is threathened - it is a loss
				int scoreValue = MAX_SCORE - 10000 + (depth *10);//Adjust score with depth, so quick mate is preferred 
				int score = isMaximizer ? -scoreValue : scoreValue;
				
				return { Move(), score };
			}
			else {
				//Else it is a draw
				return { Move(), DRAW_SCORE };
			}
		}


		Move bestMove;
		for (Move move : moves)
		{
			if (alpha >= beta) {
				break;
			}

			State resultState = MoveUtil::executeMove(state, move);

			auto [resultMove, resultScore] = searchInternal(resultState, depth - 1, alpha, beta);
			if (isMaximizer && resultScore > alpha) 
			{
				alpha = resultScore;
				bestMove = move;
			} else if (!isMaximizer && resultScore < beta) 
			{
				beta = resultScore;
				bestMove = move;
			}
			if (beta == std::numeric_limits<int>::min()) {
				int x = 1;
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
					break;
				}
				case PieceType::ROOK:
				{
					int controlledSquares = MoveUtil::getAllSliderPositionsForPiece(state, pos, piece).size();
					current = (int)(500 + 1.5 * controlledSquares);
					minorPieceThreathening = MoveUtil::isBishopThreathening(state, pos, whitePiece)
						|| MoveUtil::isKnightThreathening(state, pos, whitePiece)
						|| MoveUtil::isPawnThreathening(state, pos, whitePiece);
					break;
				}
				case PieceType::BISHOP:
				{
					int controlledSquares = MoveUtil::getAllSliderPositionsForPiece(state, pos, piece).size();
					current = 300 + 2 * controlledSquares;
					minorPieceThreathening = MoveUtil::isPawnThreathening(state, pos, whitePiece);
					break;
				}
				case PieceType::KNIGHT:
				{
					int xDist = pos.x < 4 ? 3 - pos.x : 4 - pos.x;
					xDist = xDist < 0 ? -xDist : xDist;
					int yDist = pos.y < 4 ? 3 - pos.y : 4 - pos.y;
					yDist = yDist < 0 ? -yDist : yDist;

					int distFromCenter = xDist + yDist;
					current = 300 + 3 * (4 - distFromCenter);

					minorPieceThreathening = MoveUtil::isPawnThreathening(state, pos, whitePiece);
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

		return sum;
	}

	static int minorPiecesThreatheningPoints(int num) {
		return num == 0 ? 2 : num == 1 ? -10 : -50;
	}

private:

	constexpr static int DRAW_SCORE  = 0;

	constexpr static int MAX_SCORE = std::numeric_limits<int>::max() - 1;

	constexpr static int MIN_SCORE = std::numeric_limits<int>::min() + 1;


};


