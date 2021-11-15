#pragma once
namespace Transposition {

	struct TranspositionEntry {
		short EntryCounter;
		short score;
	};
	short tableCounter = 1;
	TranspositionEntry table[];

	void initTranspositionTable(int size) {
		tableCounter = 1;
		
		/*for (int i = 0; i < size; i++) {
			table[i]
		}*/
		
	}

	void nextCounter() {
		tableCounter++;
		return;
	}
}