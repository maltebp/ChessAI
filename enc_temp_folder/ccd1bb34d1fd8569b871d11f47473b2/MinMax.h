#include "MoveUtil.h"
#include <algorithm>
class MinMaxSearcher {
public:

	static std::tuple<Move, int> search(State state, int depth, int alpha, int beta) {
		std::cout << "Search: depth=" << depth << " alpha =" << alpha << " beta= " << beta << std::endl;


		//Base case: Leaf node
		if (depth == 0) {
			int score = randomHeuristic(state);
			std::cout << score << std::endl;
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

		}

		int score = isMaximizer ? alpha : beta;
		std::cout << bestMove << std::endl;

		return { bestMove,score };
	}

	static int randomHeuristic(State state) {
		return rand() % 100;
	}
};


