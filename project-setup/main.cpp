#include <limits>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <catch_with_main.hpp>
#include <catch_ext.hpp>
#include <range/v3/all.hpp>

const int abc = std::__1::numeric_limits<int>::digits;

using namespace ranges;
using namespace std;

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

TEST_CASE("assert list equals") {
    auto v1 = vector<int>{1, 2, 3};
    auto v2 = vector<int>{1, 2, 4};
    REQUIRE(v1 == v2);
}

TEST_CASE("assert map equals") {
    auto v1 = unordered_map<string, int>{{"1", 2}, {"3", 4}};
    auto v2 = unordered_map<string, int>{{"3", 4}, {"1", 1}};
    REQUIRE(v1 == v2);
}

TEST_CASE("assert pair equals") {
    auto v1 = make_pair("1", 2);
    auto v2 = make_pair("1", 3);
    REQUIRE(v1 == v2);
}

TEST_CASE("assert tuple equals") {
    auto v1 = ranges::make_tuple("1", 2, 3);
    auto v2 = ranges::make_tuple("1", 3, 4);
    REQUIRE(v1 == v2);
}

TEST_CASE("assert view equals") {
    auto v1 = vector<int>{1, 2, 3};
    auto v2 = vector<int>{1, 2, 4};
    REQUIRE((v1 | view::all | mcht::as_vector<int>()) == (v2 | view::all | mcht::as_vector<int>()));
}

class MyStruct {
public:
    int field;
    MyStruct(int field) {
        this->field = field;
    }
};

bool operator == (const MyStruct& left, const MyStruct& right) {
    return left.field == right.field;
}

ostream& operator << ( ostream& os, const MyStruct& value ) {
    os << "MyStruct{ " << value.field << " }";
    return os;
}

TEST_CASE("assert struct equals") {
    auto v1 = MyStruct{1};
    auto v2 = MyStruct{2};
    REQUIRE(v1 == v2);
}
