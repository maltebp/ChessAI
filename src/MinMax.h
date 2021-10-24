#include "MoveUtil.h"

#include <algorithm>
#include <limits>


class MinMaxSearcher {
	
public:
	const static int drawScore = 0;
	const static int minScore = std::numeric_limits<int>::min();
	const static int maxScore = std::numeric_limits<int>::max();
	static std::tuple<Move, int> search(State state, int depth, int alpha = std::numeric_limits<int>::min(), int beta = std::numeric_limits<int>::max()) {

		//Base case: Leaf node
		if (depth == 0) {
			int score = simpleHeuristic(state);
			return { Move(), score };
		}

		bool isMaximizer = state.turn % 2 == 0;

		//Get all possible moves
		std::vector<Move> moves = MoveUtil::getAllMoves(state);
		if (moves.size() == 0) {
			//In this case it is either a draw of a loss or current player
			if (MoveUtil::isKingThreatened(state)) {
				//If king is threathened - it is a loss
				int score = isMaximizer ? maxScore : minScore;
				return { Move(), score };
			}
			else {
				//Else it is a draw
				return { Move(), drawScore };
			}
		}



		Move bestMove;
		for (Move move : moves)
		{
			if (alpha >= beta) {
				break;
			}

			State resultState = MoveUtil::executeMove(state, move);

			auto [resultMove, resultScore] = search(resultState, depth - 1, alpha, beta);
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

	static int randomHeuristic(State state) {
		return rand() % 100;
	}

	static int simpleHeuristic(State state) {
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
};


