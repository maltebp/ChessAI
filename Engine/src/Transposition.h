#pragma once
#include "Move.h"

namespace Transposition {	

	static constexpr size_t INDEX_BITS = 26;

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

	struct TranspositionEntry {
		char depth = 0;
		int score = 0;
		Move move = Move();
	};

	struct InsertResult {
		bool inserted = false;
		bool collision = false;
	};
	
	extern TranspositionEntry table[SIZE];

	void initTranspositionTable(); 

	unsigned long long hashToIndex(unsigned long long hash);

	InsertResult insertEntry(unsigned long long hash, int score, char depth, Move best);

	bool isDeeper(unsigned long long hash, char depth);

	Move getMove(unsigned long long hash);
	
	int getScore(unsigned long long hash);

	TranspositionEntry getEntry(unsigned long long hash);

}