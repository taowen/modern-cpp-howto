# 基础语法

Python 版本

```
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

```
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