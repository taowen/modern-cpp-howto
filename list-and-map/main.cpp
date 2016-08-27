#include <unordered_map>

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
    colors |= action::sort(std::less<int>(), [](const auto& e) {
        return e.size();
    });
    for(const auto& color : colors) {
        std::cout << color << std::endl;
    }
}

TEST_CASE("any of") {
    auto colors = std::vector<std::string>{"red", "green", "blue", "yellow"};
    auto result = any_of(colors, [](const auto& e) {
        return e == "green";
    });
    std::cout << result << std::endl;
}

TEST_CASE("list comprehension") {
    auto colors = std::vector<std::string>{"red", "green", "blue", "yellow"};
    auto result = colors | view::transform([](const auto& e) { return e.size(); });
    std::cout << result << std::endl;
}

TEST_CASE("slicing") {
    auto colors = std::vector<std::string>{"red", "green", "blue", "yellow"};
    auto colors_view = view::all(colors);
    std::cout << colors_view[{1, 2}] << std::endl;
    std::cout << colors_view[{0, 2}] << std::endl;
    std::cout << colors_view[{1, end}] << std::endl;
    std::cout << colors_view[{0, end-1}] << std::endl;
}

TEST_CASE("foreach map keys") {
    auto d = std::unordered_map<std::string, std::string>{
            {"matthew", "blue"},
            {"rachel", "green"},
            {"raymond", "red"}
    };
    // 取出 keys，此处不是lazy操作
    auto keys = std::vector<std::string>{d | view::keys};
    for (const auto& k : keys) {
        if (k.find("r") == 0) {
            // 因为 keys 不是lazy操作，此处的删除不会影响遍历
            d.erase(k);
        }
    }
    std::cout << (d | view::keys) << std::endl;
}

TEST_CASE("foreach key/value") {
    auto d = std::unordered_map<std::string, std::string>{
            {"matthew", "blue"},
            {"rachel", "green"},
            {"raymond", "red"}
    };
    for (const auto& [k, v] : d) {
        std::cout << k << " " << v << std::endl;
    }
}

TEST_CASE("construct map by pairs") {
    auto colors = std::vector<std::string>{"red", "green", "blue", "yellow"};
    auto d = std::unordered_map<std::string, int>{
        colors | view::transform([](const auto& e) {
            return std::make_pair(e, e.size());
        })
    };
    std::cout << (d | view::values) << std::endl;
}