#include "MoveUtil.h"

#include <algorithm>
#include <tuple>


class MinMaxSearcher {
public:

	std::tuple<Move ,int> search(State state, int depth, int alpha, int beta) {
		//Base case: Leaf node
		if (depth == 0) {
			int score = simpleHeuristic(state);
			return { Move(), score };
		}

		//Get all possible moves
		std::vector<Move> moves = MoveUtil::getAllMoves(state);
		bool isMaximizer = state.turn % 2 == 0;

		Move bestMove;
		for (Move move : moves)
		{
			if (alpha >= beta) {
				break;
			}

			State resultState = MoveUtil::executeMove(state, move);

			auto [resultMove,resultScore] = search(resultState, depth-1, alpha, beta);

			if (isMaximizer) {
				alpha = std::max(alpha, resultScore);
			}
			else {
				beta = std::min(alpha, resultScore);
			}

		}

		int score = isMaximizer ? alpha : beta;
		return { bestMove,score };
	}

	int randomHeuristic(State state) {
		return rand() % 100;
	}

	int simpleHeuristic(State state) {
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


