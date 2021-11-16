#pragma once
namespace Transposition {
	const int size = 10;
	struct TranspositionEntry {
		short entryCounter;
		short score;
	};
	short tableCounter = 1;
	TranspositionEntry table[size];

	void initTranspositionTable() {
		tableCounter = 1;
		for (int i = 0; i < size; i++) {
			table[i].entryCounter = 0;
		}
	}
	void insertEntry(int hash, short score) {
		table[hash].entryCounter = tableCounter;
		table[hash].score = score;
	}

	bool isEntry(int hash) { // if it already exists for this counter it'll return false. if it hasn't been explored yet it returns true, and overwrites the values at the hash.
		if (table[hash].entryCounter == tableCounter) {
			return true;
		}
		return false;
	}


	void nextCounter() {
		tableCounter++;
		return;
	}
}