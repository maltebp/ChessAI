#pragma once
namespace Transposition {

	const size_t INDEX_BITS = 26;

	static constexpr unsigned long long getIndexMask() {
		unsigned long long mask = 0;
		unsigned long long setBit = 1;

		for( size_t i = 0; i < INDEX_BITS; i++ ) {
			mask |= setBit;
			setBit = setBit << 1;
		}
		
		return mask;
	}

	constexpr size_t SIZE = getIndexMask() + 1;

	static unsigned long long hashToIndex(unsigned long long hash) {
		return hash & getIndexMask();
	}

	struct TranspositionEntry {
		short entryCounter;
		short score;
	};
	static short tableCounter = 1;
	static TranspositionEntry table[SIZE];

	static void initTranspositionTable() {
		tableCounter = 1;
		for (int i = 0; i < SIZE; i++) {
			table[i].entryCounter = 0;
		}
	}
	static void insertEntry(unsigned long long hash, short score) {
		table[hashToIndex(hash)].entryCounter = tableCounter;
		table[hashToIndex(hash)].score = score;
	}

	static bool isEntry(unsigned long long hash) { // if it already exists for this counter it'll 
		//return true. if it hasn't been explored yet it returns false.
		if (table[hashToIndex(hash)].entryCounter == tableCounter) {
			return true;
		}
		return false;
	}
	
	static short getScore(unsigned long long hash) {
		return table[hashToIndex(hash)].score;
	}


	static void nextCounter() {
		tableCounter++;
		return;
	}
}