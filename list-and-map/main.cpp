#include <unordered_map>
#include <map>

#include <catch_with_main.hpp>
#include <catch_ext.hpp>
#include <range/v3/all.hpp>
#include <json.hpp>

using namespace std;
using namespace ranges;
using json = nlohmann::json;


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

TEST_CASE("reverse") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    CHECK((vector<string>{"yellow", "blue", "green", "red"})
          ==
          (colors | view::reverse | to_vector));
}

TEST_CASE("zip") {
    auto names = vector<string>{"raymond", "rachel", "matthew"};
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto zipped = view::zip(names, colors);
    CHECK((vector<pair<string, string>>{
               {"raymond", "red"}, {"rachel", "green"}, {"matthew", "blue"}
           })
          ==
          (zipped | to_vector));
    for(const auto& [name, color] : zipped) {
        cout << name << " " << color << endl;
    }
}

TEST_CASE("foreach with index") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    for(const auto& [i, color] : view::zip(view::iota(0), colors)) {
        cout << i << " " << color << endl;
    }
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

TEST_CASE("sort") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto sorted = action::sort(colors);
    CHECK((vector<string>{"blue", "green", "red", "yellow"})
          ==
          (sorted | to_vector));
}

TEST_CASE("sort reverse") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto sorted = action::sort(colors, greater<string>());
    CHECK((vector<string>{"yellow", "red", "green", "blue"})
          ==
          (sorted | to_vector));
}

TEST_CASE("sort by lambda") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto sorted = action::sort(colors, less<int>(), [](const auto& e) {
        return e.size();
    });
    CHECK((vector<string>{"red", "blue", "green", "yellow"})
          ==
          (sorted | to_vector));
}

TEST_CASE("any of") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    CHECK(ranges::any_of(colors, [](const auto& e) {
        return e == "green";
    }));
}

TEST_CASE("list comprehension") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto actual = vector<int>{
        colors | view::transform([](const auto& e) { return e.size(); })
    };
    CHECK((vector<int>{3, 5, 4, 6}) == actual);
}

TEST_CASE("slicing") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto colors_view = colors | view::all;
    CHECK((vector<string>{"green"})
          ==
          (colors_view[{1, 2}] | to_vector));
    CHECK((vector<string>{"red", "green"})
          ==
          (colors_view[{0, 2}] | to_vector));
    CHECK((vector<string>{"green", "blue", "yellow"})
          ==
          (colors_view[{1, ranges::end}] | to_vector));
    CHECK((vector<string>{"red", "green", "blue"})
          ==
          (colors_view[{0, ranges::end-1}] | to_vector));
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
    CHECK((unordered_map<string, string>{{"matthew", "blue"}}) == d);
}

TEST_CASE("construct map by pairs") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto d = unordered_map<string, int>{
        colors | view::transform([](const auto& e) {
            return make_pair(e, e.size());
        })
    };
    CHECK((unordered_map<string, int>{
                    {"red", 3},
                    {"green", 5},
                    {"yellow", 6},
                    {"blue", 4}
        }) == d);
}

TEST_CASE("json literal") {
    json obj {
            {"int", 1},
            {"string", {1, 2, 3}},
    };
    cout << obj << endl;
}
