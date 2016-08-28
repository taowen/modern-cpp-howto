#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

void modify_single_value(int single) {
    single = 2;
}

TEST_CASE("pass single value") {
    auto local_variable = 1;
    modify_single_value(local_variable);
    std::cout << local_variable << std::endl; // 1
}

void modify_single_reference(int& single) {
    single = 2;
}

TEST_CASE("pass single reference") {
    auto local_variable = 1;
    modify_single_reference(local_variable);
    std::cout << local_variable << std::endl; // 1
}

void modify_single_pointer(int* single) {
    *single = 2;
}

TEST_CASE("pass single pointer") {
    auto local_variable = 1;
    modify_single_pointer(&local_variable);
    std::cout << local_variable << std::endl; // 1
}

void modify_list_value(std::vector<int> list) {
    list[1] = 2;
}

TEST_CASE("pass list value") {
    auto local_variable = std::vector<int>{1, 1};
    modify_list_value(local_variable);
    std::cout << (local_variable | view::all) << std::endl; // 1
}

class MyStruct {
public:
    int field;
};

void modify_struct_value(MyStruct struct_) {
    struct_.field = 2;
}

TEST_CASE("pass struct value") {
    auto local_variable = MyStruct{1};
    modify_struct_value(local_variable);
    std::cout << local_variable.field << std::endl; // 1
}

