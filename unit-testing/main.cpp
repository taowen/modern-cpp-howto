#define CATCH_CONFIG_MAIN

#include "catch.hpp"

unsigned int fib(unsigned int n) {
    if (n <= 1) {
        return n;
    } else {
        return fib(n - 1) + fib(n - 2);
    }
}

TEST_CASE("fib 0") {
    REQUIRE(fib(0) == 0);
}

TEST_CASE("fib 1") {
    REQUIRE(fib(1) == 1);
}

TEST_CASE("fib 5") {
    REQUIRE(fib(5) == 5);
}