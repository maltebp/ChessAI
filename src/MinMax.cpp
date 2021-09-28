#include "MoveUtil.h"
class MinMaxSearcher {
public:

	Move search(State state, int depth) {
		bool isMaximizer = state.turn % 2 == 0;
		return Move();
	}



	int randomHeuristic(State state) {
		return rand() % 100;
	}
};


