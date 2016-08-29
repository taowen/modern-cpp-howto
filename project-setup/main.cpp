#include <vector>
#include <catch_with_main.hpp>

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
    auto v1 = std::vector<int>{1, 2, 3};
    auto v2 = std::vector<int>{1, 2, 4};
    REQUIRE(v1 == v2);
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

std::ostream& operator << ( std::ostream& os, const MyStruct& value ) {
    os << "MyStruct{ " << value.field << " }";
    return os;
}

TEST_CASE("assert struct equals") {
    auto v1 = MyStruct{1};
    auto v2 = MyStruct{2};
    REQUIRE(v1 == v2);
}