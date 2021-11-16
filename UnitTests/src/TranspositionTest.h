#pragma once

#include "external/catch.hpp"

#include "Util.h"
#include "Transposition.h"
#include <limits>


TEST_CASE("InitTable", "[Transposition]") {

    Transposition::initTranspositionTable();

    REQUIRE(Transposition::tableCounter == 1);
    bool flag = true;
    for (int i = 0; i < Transposition::SIZE; i++) {
        if (Transposition::table[i].entryCounter != 0) {
            flag = false;
        }
    }
    REQUIRE(flag);
}

TEST_CASE("NewHash", "[Transposition]") {

    Transposition::initTranspositionTable();

    Transposition::insertEntry(3,27);

    REQUIRE(Transposition::table[3].score == 27);
    REQUIRE(Transposition::table[3].entryCounter == 1);
    REQUIRE(Transposition::tableCounter == 1);
     
    REQUIRE(Transposition::isEntry(3));

    Transposition::nextCounter();
    REQUIRE(Transposition::table[3].score == 27);
    REQUIRE(Transposition::tableCounter == 2);
    REQUIRE(Transposition::table[3].entryCounter == 1);
    Transposition::insertEntry(3, 24);
    REQUIRE(Transposition::table[3].score == 24);
    REQUIRE(Transposition::table[3].entryCounter == 2);

}
TEST_CASE("HashMask", "[Transposition]") {

   if( Transposition::hashToIndex(std::numeric_limits<unsigned long long int>::max())<Transposition::SIZE ){
        REQUIRE(true);
    }
   else {
       REQUIRE(false);
   }
   if (Transposition::hashToIndex(0) < Transposition::SIZE) {
       REQUIRE(true);
   }
   else {
       REQUIRE(false);
   }
}