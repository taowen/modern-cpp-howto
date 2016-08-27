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
    auto colors = std::vector<const char*>{"red", "green", "blue", "yellow"};
    for(const auto& color : colors) {
        std::cout << color << std::endl;
    }
}

TEST_CASE("foreach reversed") {
    auto colors = std::vector<const char*>{"red", "green", "blue", "yellow"};
    for(const auto& color : colors | view::reverse) {
        std::cout << color << std::endl;
    }
}

TEST_CASE("foreach with index") {
    auto colors = std::vector<const char*>{"red", "green", "blue", "yellow"};
    for(const auto& [i, color] : view::zip(view::iota(0), colors)) {
        std::cout << i << " " << color << std::endl;
    }
}

TEST_CASE("zip") {
    auto names = std::vector<const char*>{"raymond", "rachel", "matthew"};
    auto colors = std::vector<const char*>{"red", "green", "blue", "yellow"};
    for(const auto& [name, color] : view::zip(names, colors)) {
        std::cout << name << " " << color << std::endl;
    }
}

TEST_CASE("sort") {
    auto colors = std::vector<std::string>{"red", "green", "blue", "yellow"};
    colors |= action::sort;
    for(const auto& color : colors) {
        std::cout << color << std::endl;
    }
}

TEST_CASE("sort reverse") {
    auto colors = std::vector<std::string>{"red", "green", "blue", "yellow"};
    colors |= action::sort(std::greater<std::string>());
    for(const auto& color : colors) {
        std::cout << color << std::endl;
    }
}

TEST_CASE("custom sort") {
    auto colors = std::vector<std::string>{"red", "green", "blue", "yellow"};
    colors |= action::sort(std::less<std::string>(), [](const auto&e) {
        return e.size();
    });
    for(const auto& color : colors) {
        std::cout << color << std::endl;
    }
}