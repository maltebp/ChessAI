#include "Transposition.h"


namespace Transposition {

    
    TranspositionEntry table[SIZE];

    
    void initTranspositionTable() {
        for (int i = 0; i < SIZE; i++) {
            table[i] = TranspositionEntry();
        }
    }


    unsigned long long hashToIndex(unsigned long long hash) {
        return hash & getIndexMask();
    }

    
    InsertResult insertEntry(unsigned long long hash, int score, char depth, Move best) {
        InsertResult result;
        unsigned long long index = hashToIndex(hash);

        if( table[index].depth != 0 ) {
            result.collision = true;
        }
        
        table[index].depth = depth;
        table[index].score = score;
        table[index].move = best;
        table[index].hash = hash;

        // Current implementation always overwrites. This may be changed soon
        result.inserted = true;

        return result;
    }


    bool isDeeper(unsigned long long hash, char depth) { // if the new entry is deeper then return true, else return false
        if (table[hashToIndex(hash)].depth <depth) {
            return true;
        }
        return false;
    }

    
    Move getMove(unsigned long long hash) { 
        if (table[hashToIndex(hash)].depth != -1) {
            return table[hashToIndex(hash)].move;
        }
        return Move();
    }


    int getScore(unsigned long long hash) {
        return table[hashToIndex(hash)].score;
    }


    TranspositionEntry getEntry(unsigned long long hash) {
        return table[hashToIndex(hash)];
    }

}