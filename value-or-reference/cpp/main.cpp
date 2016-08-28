#include <unordered_map>
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

class MySocket {
public:
    constexpr MySocket() = default;
    ~MySocket() = default;

protected:
    MySocket( const MySocket& ) = delete; // copy constructor
    MySocket& operator=( const MySocket& ) = delete; // assignment constructor
};

TEST_CASE("transient object") {
    MySocket sock1, sock2;
    // sock1 == sock2; 没有默认实现的 ==，无需禁用
    // sock1 != sock2; 没有默认实现的 !=，无需禁用
    // std::unordered_map<MySocket, int> some_map{}; 编译错误，MySocket没有实现std::hash
    // auto sock3 = sock1; copy constructor 已经被禁用
    // sock2 = sock1; assignment constructor 已经被禁用
}
