#pragma once

#include "external/catch.hpp"

#include "Util.h"
#include "Transposition.h"


TEST_CASE("InitTable", "[Transposition]") {

    Transposition::initTranspositionTable();

    REQUIRE(Transposition::tableCounter == 1);
    for (int i = 0; i < 10; i++) {
        REQUIRE(Transposition::table[i].entryCounter == 0);
    }
}

TEST_CASE("NewHash", "[Transposition]") {

    Transposition::initTranspositionTable();

    Transposition::newEntry(3,27);

    REQUIRE(Transposition::table[3].score == 27);
    REQUIRE(Transposition::table[3].entryCounter == 1);
    REQUIRE(Transposition::tableCounter == 1);
    Transposition::newEntry(3, 24);
    REQUIRE(Transposition::table[3].score == 27);

    Transposition::nextCounter();
    REQUIRE(Transposition::table[3].score == 27);
    REQUIRE(Transposition::tableCounter == 2);
    REQUIRE(Transposition::table[3].entryCounter == 1);
    Transposition::newEntry(3, 24);
    REQUIRE(Transposition::table[3].score == 24);
    REQUIRE(Transposition::table[3].entryCounter == 2);

}