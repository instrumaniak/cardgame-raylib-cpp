// src/logic/test_fog.cpp — Unit tests for fog visibility logic
#include "doctest/doctest.h"
#include "logic/fog.h"

using namespace game::logic;

TEST_CASE("rowVisible") {
  SUBCASE("currentRow is visible") {
    CHECK(rowVisible(0, 0) == true);
    CHECK(rowVisible(1, 1) == true);
    CHECK(rowVisible(5, 5) == true);
  }

  SUBCASE("currentRow+1 is visible") {
    CHECK(rowVisible(1, 0) == true);
    CHECK(rowVisible(2, 1) == true);
  }

  SUBCASE("currentRow+2 is visible") {
    CHECK(rowVisible(2, 0) == true);
    CHECK(rowVisible(3, 1) == true);
  }

  SUBCASE("currentRow+3 is not visible") {
    CHECK(rowVisible(3, 0) == false);
    CHECK(rowVisible(4, 1) == false);
  }

  SUBCASE("row before currentRow is not visible") {
    CHECK(rowVisible(0, 1) == false);
    CHECK(rowVisible(0, 5) == false);
  }
}

TEST_CASE("rowFog") {
  SUBCASE("0 monster-eyes — no fog on any visible row") {
    CHECK(rowFog(0, 0, 0) == false);
    CHECK(rowFog(1, 0, 0) == false);
    CHECK(rowFog(2, 0, 0) == false);
  }

  SUBCASE("1 monster-eye — fog on row currentRow+2") {
    CHECK(rowFog(0, 0, 1) == false);
    CHECK(rowFog(1, 0, 1) == false);
    CHECK(rowFog(2, 0, 1) == true);
  }

  SUBCASE("2 monster-eyes — fog on row currentRow+1 and currentRow+2") {
    CHECK(rowFog(0, 0, 2) == false);
    CHECK(rowFog(1, 0, 2) == true);
    CHECK(rowFog(2, 0, 2) == true);
  }

  SUBCASE("non-visible rows are never fogged") {
    CHECK(rowFog(3, 0, 2) == false);
    CHECK(rowFog(-1, 0, 1) == false);
  }

  SUBCASE("works with non-zero currentRow") {
    CHECK(rowFog(3, 1, 1) == true);
    CHECK(rowFog(3, 1, 2) == true);
    CHECK(rowFog(2, 1, 1) == false);
  }
}
