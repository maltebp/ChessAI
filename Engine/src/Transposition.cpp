#include "Transposition.h"


namespace Transposition {

    short tableCounter = 1;
    
    TranspositionEntry table[SIZE];
    
    void initTranspositionTable() {
        tableCounter = 1;
        for (int i = 0; i < SIZE; i++) {
            table[i].entryCounter = 0;
        }
    }


    unsigned long long hashToIndex(unsigned long long hash) {
        return hash & getIndexMask();
    }

    
    void insertEntry(unsigned long long hash, short score) {
        table[hashToIndex(hash)].entryCounter = tableCounter;
        table[hashToIndex(hash)].score = score;
    }


    bool isEntry(unsigned long long hash) { // if it already exists for this counter it'll 
        //return true. if it hasn't been explored yet it returns false.
        if (table[hashToIndex(hash)].entryCounter == tableCounter) {
            return true;
        }
        return false;
    }


    short getScore(unsigned long long hash) {
        return table[hashToIndex(hash)].score;
    }


    void nextCounter() {
        tableCounter++;
        return;
    }

}