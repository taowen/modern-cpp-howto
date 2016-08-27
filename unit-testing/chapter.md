# 单元测试

## 开发环境

* 操作系统: ubuntu 14.04
* 编译器 clang 4.0: http://apt.llvm.org/ 使用 c++ 17
* cmake: sudo apt-get install cmake
* 包管理: git clone 依赖到自己的repository，所谓vendoring
* IDE clion： https://www.jetbrains.com/clion/

标准的 cmake 文件，启用 C++ 14 和 libc++。其中 _vendor 目录是我们用于保存包含的第三方库的位置。

```
cmake_minimum_required(VERSION 3.6)
project(unit-testing)

include_directories(_vendor/catch/include)
include_directories(_vendor/range-v3/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -stdlib=libc++")

set(SOURCE_FILES main.cpp)
add_executable(unit-testing ${SOURCE_FILES})
```

clion 基本上可以有和 java 一样的IDE体验，包括 debugger 在内。绝对值得拥有。

参考资料

* http://arne-mertz.de/2016/08/the-4c-development-environment/
* http://blog.conan.io/2016/05/10/Programming-C++-with-the-4-Cs-Clang,-CMake,-CLion-and-Conan.html

## libc++

安装最新版本的 c++ 标准库

```
git clone https://github.com/llvm-mirror/llvm.git llvm-src
git clone https://github.com/llvm-mirror/libcxxabi.git llvm-src/projects
git clone https://github.com/llvm-mirror/libcxx.git llvm-src/projects
CC=clang CXX=clang++ cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release llvm-src
make cxx -j 8
sudo make install-libcxx install-libcxxabi
```

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

```c++
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
```

更详细的单元测试框架用法，参见 http://catch-lib.net