# 工程管理

## 开发环境

* 操作系统: ubuntu 16.04
* 编译器 clang 4.0: http://apt.llvm.org/ 使用 c++ 17
* cmake 3.6: https://cmake.org/download/
* IDE qtcreator 4.1: http://download.qt.io/official_releases/qtcreator
* 包管理: git clone 依赖到自己的repository，所谓vendoring

### cmake

命令行参数设置cmake使用clang编译器
```
cmake
-D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++
```

### clang

设置clang 启用 C++ 14 和 libc++。其中 _vendor 目录是我们用于保存包含的第三方库的位置。
```
cmake_minimum_required(VERSION 3.6)
project(unit-testing)

include_directories(_vendor/catch/include)
include_directories(_vendor/range-v3/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -stdlib=libc++")

set(SOURCE_FILES main.cpp)
add_executable(unit-testing ${SOURCE_FILES})
```

### libc++

安装最新版本的 c++ 标准库

```
git clone https://github.com/llvm-mirror/llvm.git llvm-src
git clone https://github.com/llvm-mirror/libcxxabi.git llvm-src/projects/libcxxabi
git clone https://github.com/llvm-mirror/libcxx.git llvm-src/projects/libcxx
mkdir build
cd build
CC=clang CXX=clang++ cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release llvm-src
make cxx -j 8
sudo make install-libcxx install-libcxxabi
```

### qtcreator

选择qtcreator最主要的原因是因为它支持使用clang做语法补全。help => about plugins => ClangCodeModel


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

单元测试一个常见的烦恼是集合对象的对比。在 python 中我们会写

```python
import unittest

class FibTest(unittest.TestCase):
    def test_assert_list_equals(self):
        self.assertListEqual([1, 2, 3], [1, 2, 4])
```

C++ 版本

```c++
#include <vector>
#include <catch_with_main.hpp>

TEST_CASE("assert list equals") {
    auto v1 = std::vector<int>{1, 2, 3};
    auto v2 = std::vector<int>{1, 2, 4};
    REQUIRE(v1 == v2);
}
```

打印的错误消息还是很清楚的

```
  REQUIRE( v1 == v2 )
with expansion:
  { 1, 2, 3 } == { 1, 2, 4 }
```

自定义类型也是支持的

```c++
#include <catch_with_main.hpp>

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
```

提示的出错消息是

```
  REQUIRE( v1 == v2 )
with expansion:
  MyStruct{ 1 } == MyStruct{ 2 }
```

更详细的单元测试框架用法，参见 http://catch-lib.net

## clang modules

c++一个很大的烦恼是编译速度非常慢。其中一个重要原因是因为头文件不能作为模块化独立处理，而是要重复地被包含。clang modules提供了一种机制把一组
头文件打包成一个module，独立进行解析并保留为pcm文件。然后编译时遇到include就只需要把预编译好的pcm文件包含进去就行了。

比如我们有一个range-v3的lib是头文件提供的。它的文件结构如下
```
_vendor/range-v3/
├── include
│   ├── meta
│   │   ├── meta_fwd.hpp
│   │   └── meta.hpp
│   ├── module.modulemap
│   └── range
│       └── v3
└── update.py
```

提供的module.modulemap就把这个目录下的头文件都打包成一个module了。modulemap的内容非常简单

```
$ cat _vendor/range-v3/include/module.modulemap
module range_v3 {
    umbrella "."
    export *
}
```
在编译的时候加上一些编译选项 `-fmodules` 就可以启用模块的支持了。明显的感受当然是编译速度极大提升。当包含的头文件没有改动的时候，就不用每次
都编译它们了，所以变快了。modulemap还有更多的功能，可以选择性地一些实现细节，export公开的api。
