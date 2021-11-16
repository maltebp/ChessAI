#include "Transposition.h"


namespace Transposition {

    
    TranspositionEntry table[SIZE];
    
    void initTranspositionTable() {
        for (int i = 0; i < SIZE; i++) {
            table[i].depth = -1;
        }
    }


    unsigned long long hashToIndex(unsigned long long hash) {
        return hash & getIndexMask();
    }

    
    void insertEntry(unsigned long long hash, int score, char depth, Move best) {
        table[hashToIndex(hash)].depth = depth;
        table[hashToIndex(hash)].score = score;
        table[hashToIndex(hash)].move = best;
    }


    bool isDeeper(unsigned long long hash, char depth) { // if the new entry is deeper then return true, else return false
        if (table[hashToIndex(hash)].depth <depth) {
            return true;
        }
        return false;
    }


    short getScore(unsigned long long hash) {
        return table[hashToIndex(hash)].score;
    }

}