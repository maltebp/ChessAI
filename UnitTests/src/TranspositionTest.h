#pragma once

#include "external/catch.hpp"

#include "Util.h"
#include "Transposition.h"
#include <limits>


TEST_CASE("Init Table", "[transposition]") {

    Transposition::initTranspositionTable();
    bool flag = true;
    for (int i = 0; i < Transposition::SIZE; i++) {
        if (Transposition::table[i].depth != -1) {
            flag = false;
        }
    }
    REQUIRE(flag);
}


TEST_CASE("New Hash", "[transposition]") {

    Transposition::initTranspositionTable();

    Transposition::insertEntry(3, 27, 5, { {2,2} ,{3,3} });

    REQUIRE(Transposition::table[3].score == 27);
    REQUIRE(Transposition::table[3].depth == 5);
     
    REQUIRE(Transposition::isDeeper(3,6));
    REQUIRE(!Transposition::isDeeper(3, 4));

    Transposition::insertEntry(3, 24, 7, { {2,2} ,{3,3} });
    REQUIRE(Transposition::table[3].score == 24);
    REQUIRE(Transposition::table[3].depth == 7);

}


TEST_CASE("Hash Mask", "[transposition]") {

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