#include <unordered_map>
#include <map>

#include <catch_with_main.hpp>
#include <range/v3/all.hpp>
#include <json.hpp>

using namespace std;
using namespace ranges;
using json = nlohmann::json;

template<typename value_type, typename Rng2>
void check_range_equal(vector<value_type> expected, const Rng2& actual)
{
    auto actualAsVector = vector<value_type>{};
    for(const auto& e : actual) {
        actualAsVector.emplace_back(e);
    }
    CHECK(expected == actualAsVector);
}

template<typename key_type, typename value_type, typename Rng2>
void check_map_equal(unordered_map<key_type, value_type> expected, const Rng2& actual)
{
    auto actualAsMap = unordered_map<key_type, value_type>{};
    for(const auto& [k, v] : actual) {
    }
    CHECK(expected == actualAsMap);
}

namespace Catch {
    template<> struct StringMaker<pair<string, string>> {
        static std::string convert( pair<string, string> const& value ) {
            return "(\"" + value.first + "\", \"" + value.second + "\")";
        }
    };
    template<> struct StringMaker<pair<string, int>> {
        static std::string convert( pair<string, int> const& value ) {
            return "(\"" + value.first + "\", " + to_string(value.second) + ")";
        }
    };
}

TEST_CASE("foreach on lazy range") {
    for(const auto& x : view::ints(0, 6)) {
        cout << x * x << endl;
    }
}

TEST_CASE("foreach on list") {
    auto colors = {"red", "green", "blue", "yellow"};
    for(const auto& color : colors) {
        cout << color << endl;
    }
}

TEST_CASE("foreach on vector") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    for(const auto& color : colors) {
        cout << color << endl;
    }
}

TEST_CASE("foreach with index") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    for(const auto& [i, color] : view::zip(view::iota(0), colors)) {
        cout << i << " " << color << endl;
    }
}

TEST_CASE("foreach with pair") {
    auto names = vector<string>{"raymond", "rachel", "matthew"};
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    for(const auto& [name, color] : view::zip(names, colors)) {
        cout << name << " " << color << endl;
    }
    check_map_equal<string, string>(
            {{"raymond", "red"}, {"rachel", "green"}, {"matthew", "blue"}},
            view::zip(names, colors));
}

TEST_CASE("foreach map") {
    auto d = unordered_map<string, string>{
            {"matthew", "blue"},
            {"rachel", "green"},
            {"raymond", "red"}
    };
    for (const auto& [k, v] : d) {
        cout << k << " " << v << endl;
    }
}

TEST_CASE("reverse") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    check_range_equal<string>(
            {"yellow", "blue", "green", "red"},
            colors | view::reverse);
}

TEST_CASE("sort") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    colors |= action::sort;
    check_range_equal<string>(
            {"blue", "green", "red", "yellow"},
            colors);
}

TEST_CASE("sort reverse") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    colors |= action::sort(greater<string>());
    check_range_equal<string>({"yellow", "red", "green", "blue"}, colors);
}

TEST_CASE("custom sort") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    colors |= action::sort(less<int>(), [](const auto& e) {
        return e.size();
    });
    check_range_equal<string>({"red", "blue", "green", "yellow"}, colors);
}

TEST_CASE("any of") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto result = ranges::any_of(colors, [](const auto& e) {
        return e == "green";
    });
    CHECK(result);
}

TEST_CASE("list comprehension") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto result = colors | view::transform([](const auto& e) { return e.size(); });
    check_range_equal<int>({3, 5, 4, 6}, result);
}

TEST_CASE("slicing") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto colors_view = colors | view::all;
    check_range_equal<string>({"green"}, colors_view[{1, 2}]);
    check_range_equal<string>({"red", "green"}, colors_view[{0, 2}]);
    check_range_equal<string>({"green", "blue", "yellow" }, colors_view[{1, ranges::end}]);
    check_range_equal<string>({"red", "green", "blue"}, colors_view[{0, ranges::end-1}]);
}

TEST_CASE("foreach map keys") {
    auto d = unordered_map<string, string>{
            {"matthew", "blue"},
            {"rachel", "green"},
            {"raymond", "red"}
    };
    // 取出 keys，此处不是lazy操作
    auto keys = vector<string>{d | view::keys};
    for (const auto& k : keys) {
        if (k.find("r") == 0) {
            // 因为 keys 不是lazy操作，此处的删除不会影响遍历
            d.erase(k);
        }
    }
    check_range_equal<pair<string, string>>({{"mattew", "blue"}}, d | view::all);
}

TEST_CASE("construct map by pairs") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto d = unordered_map<string, int>{
        colors | view::transform([](const auto& e) {
            return make_pair(e, e.size());
        })
    };
    check_range_equal<pair<string, int>>(
            {
                    {"red", 1},
                    {"green", 2},
                    {"green", 6},
            },
            d | view::all);
}

TEST_CASE("json literal") {
    json obj {
            {"int", 1},
            {"string", {1, 2, 3}},
    };
    cout << obj << endl;
}