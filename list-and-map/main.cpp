#include <iostream>
#include <vector>
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("foreach on lazy range") {
    for(const auto& x : view::ints(0, 6)) {
        std::cout << x * x << std::endl;
    }
}

TEST_CASE("foreach on list") {
    auto colors = {"red", "green", "blue", "yellow"};
    for(const auto& color : colors) {
        std::cout << color << std::endl;
    }
}

TEST_CASE("foreach on vector") {
    auto colors = std::vector<char const*>{"red", "green", "blue", "yellow"};
    for(const auto& color : colors) {
        std::cout << color << std::endl;
    }
}

TEST_CASE("foreach reversed") {
    auto colors = std::vector<char const*>{"red", "green", "blue", "yellow"};
    for(const auto& color : colors | view::reverse) {
        std::cout << color << std::endl;
    }
}

TEST_CASE("foreach with index") {
    auto colors = std::vector<char const*>{"red", "green", "blue", "yellow"};
    for(const auto& [i, color] : view::zip(view::iota(0), colors)) {
        std::cout << i << " " << color << std::endl;
    }
}