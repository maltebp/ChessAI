#pragma once


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
		short entryCounter;
		short score;
	};

	extern short tableCounter;
	
	extern TranspositionEntry table[SIZE];

	void initTranspositionTable(); 

	unsigned long long hashToIndex(unsigned long long hash);

	void insertEntry(unsigned long long hash, short score);

	bool isEntry(unsigned long long hash);
	
	short getScore(unsigned long long hash);

	void nextCounter();

}