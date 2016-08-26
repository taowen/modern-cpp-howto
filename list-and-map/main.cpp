#define CATCH_CONFIG_MAIN
#include <boost/range/irange.hpp>
#include <iostream>
#include <vector>

#include "catch.hpp"

TEST_CASE("foreach on lazy range") {
    for(auto x : boost::irange(0, 6)) {
        std::cout << x * x << std::endl;
    }
}

TEST_CASE("foreach on lazy range in real world") {
    for(auto x = 0; x < 6; x++) {
        std::cout << x * x << std::endl;
    }
}

TEST_CASE("foreach on list") {
    auto colors = {"red", "green", "blue", "yellow"};
    for(auto color : colors) {
        std::cout << color << std::endl;
    }
}

TEST_CASE("foreach on vector") {
    auto colors = std::vector<char const*>{"red", "green", "blue", "yellow"};
    for(auto color : colors) {
        std::cout << color << std::endl;
    }
}