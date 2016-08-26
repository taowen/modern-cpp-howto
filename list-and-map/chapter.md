# list 和 map

本节例子选自：https://gist.github.com/JeffPaine/6213790

对 python 这样的动态语言最直观的感受就是 list/map 两种数据结构打天下。php 和 lua 甚至把这两个都合并成一种数据结构了。
毋庸置疑，学会如何使用 list 和 map 是基础中的基础。

## for 循环

Python 版本

```
import unittest


class Test(unittest.TestCase):
    def test_foreach_on_lazy_range(self):
        for i in xrange(6):
            print i ** 2
```

C++ 版本

内置并没有xrange的实现，需要用boost

```
下载并解压缩 bootstrap
./bootstrap.sh --prefix=/usr/local
./b2 install toolset=clang cxxflags="-stdlib=libc++" linkflags="-stdlib=libc++"
```

```
#define CATCH_CONFIG_MAIN
#include <boost/range/irange.hpp>
#include <iostream>

#include "catch.hpp"

TEST_CASE("foreach on lazy range") {
    for(auto x : boost::irange(0, 6)) {
        std::cout << x * x << std::endl;
    }
}
```

更常规的写法，还是直接写普通的for循环

```
#define CATCH_CONFIG_MAIN
#include <iostream>

#include "catch.hpp"

TEST_CASE("foreach on lazy range in real world") {
    for(auto x = 0; x < 6; x++) {
        std::cout << x * x << std::endl;
    }
}
```

## foreach

Python 版本

```
import unittest


class Test(unittest.TestCase):
    def test_foreach_on_list(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for color in colors:
            print color
```

C++ 版本

```
#define CATCH_CONFIG_MAIN
#include <iostream>

#include "catch.hpp"

TEST_CASE("foreach on list") {
    auto colors = {"red", "green", "blue", "yellow"};
    for(auto color : colors) {
        std::cout << color << std::endl;
    }
}
```

与python不同，c++没有所谓的默认的list类型。上面的写法是最简洁的写法。colors 变量的实际类型
根据GDB是 `std::initializer_list<char const*>`。只有 begin，end，size 几个函数。实际上类似于 python 的 tuple。
考虑到 python 的 list 类型是 mutable 的，所以更合适的实现是 std::vector。

```
#define CATCH_CONFIG_MAIN
#include <iostream>
#include <vector>

#include "catch.hpp"

TEST_CASE("foreach on vector") {
    auto colors = std::vector<char const*>{"red", "green", "blue", "yellow"};
    for(auto color : colors) {
        std::cout << color << std::endl;
    }
}
```
