# 单元测试

## 开发环境

* 操作系统: ubuntu 14.04
* 编译器: clang 4.0 http://apt.llvm.org/
* make: sudo apt-get install cmake
* STL: sudo apt-get install libc++-dev
* IDE： https://www.jetbrains.com/clion/

标准的 cmake 文件，启用 C++ 14 和 libc++。

```
cmake_minimum_required(VERSION 3.6)
project(unit-testing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -stdlib=libc++")

set(SOURCE_FILES main.cpp)
add_executable(unit-testing ${SOURCE_FILES})
```

clion 基本上可以有和 java 一样的IDE体验，包括 debugger 在内。绝对值得拥有。

## 测试框架

Python 版本

```python
import unittest


def fib(n):
    if n <= 1:
        return n
    else:
        return fib(n - 1) + fib(n - 2)


class FibTest(unittest.TestCase):
    def test_fib_0(self):
        self.assertEqual(0, fib(0))

    def test_fib_1(self):
        self.assertEqual(1, fib(1))

    def test_fib_5(self):
        self.assertEqual(5, fib(5))
```

C++ 版本

引用的 catch.hpp 从这里下载 https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp

```c++
#define CATCH_CONFIG_MAIN

#include "catch.hpp"

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
```

更详细的单元测试框架用法，参见 http://catch-lib.net