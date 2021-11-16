#pragma once
namespace Transposition {
	const int size = 4194304;
	struct TranspositionEntry {
		short entryCounter;
		short score;
	};
	static short tableCounter = 1;
	static TranspositionEntry table[size];

	static void initTranspositionTable() {
		tableCounter = 1;
		for (int i = 0; i < size; i++) {
			table[i].entryCounter = 0;
		}
	}
	static void insertEntry(unsigned long long hash, short score) {
		table[hash].entryCounter = tableCounter;
		table[hash].score = score;
	}

	static bool isEntry(unsigned long long hash) { // if it already exists for this counter it'll 
		//return true. if it hasn't been explored yet it returns false.
		if (table[hash].entryCounter == tableCounter) {
			return true;
		}
		return false;
	}
	static short getScore(unsigned long long hash) {
		return table[hash].score;
	}


	static void nextCounter() {
		tableCounter++;
		return;
	}
}