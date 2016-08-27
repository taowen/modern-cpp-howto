# list 和 map

本节例子选自：https://gist.github.com/JeffPaine/6213790

对 python 这样的动态语言最直观的感受就是 list/map 两种数据结构打天下。php 和 lua 甚至把这两个都合并成一种数据结构了。
毋庸置疑，学会如何使用 list 和 map 是基础中的基础。

## for 循环

Python 版本

```python
import unittest

class Test(unittest.TestCase):
    def test_foreach_on_lazy_range(self):
        for i in xrange(6):
            print i ** 2
```

C++ 版本

```c++
#include <iostream>
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("foreach on lazy range") {
    for(const auto& x : view::ints(0, 6)) {
        std::cout << x * x << std::endl;
    }
}
```

注意到 `const auto&` 的写法，这个表示我对这个变量进行只读的使用。只要是能用const的地方就用const（
http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#a-namerconst-immutableacon1-by-default-make-objects-immutable）。
为什么还需要加上reference？因为非reference的版本默认的语义我要拥有这个变量（make a copy）。而在for循环下我们显然是只打算使用这个变量，
而不是去拥有一份。为什么不用指针而用引用？因为指针可空，reference不可空。

`view::ints` 是 range-v3 这个库提供的，作用等同于xrange。将来 range-v3 会成为标准库的一部分。

## foreach

Python 版本

```python
import unittest

class Test(unittest.TestCase):
    def test_foreach_on_list(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for color in colors:
            print color
```

C++ 版本

```c++
#include <iostream>
#include <catch_with_main.hpp>

using namespace ranges;

TEST_CASE("foreach on list") {
    auto colors = {"red", "green", "blue", "yellow"};
    for(const auto& color : colors) {
        std::cout << color << std::endl;
    }
}
```

与python不同，c++没有所谓的默认的list类型。上面的写法是最简洁的写法。colors 变量的实际类型
根据GDB是 `std::initializer_list<char const*>`。只有 begin，end，size 几个函数。实际上类似于 python 的 tuple。
考虑到 python 的 list 类型是 mutable 的，所以更合适的实现是 std::vector。

```c++
#include <iostream>
#include <vector>
#include <catch_with_main.hpp>

using namespace ranges;

TEST_CASE("foreach on vector") {
    auto colors = std::vector<char const*>{"red", "green", "blue", "yellow"};
    for(const auto& color : colors) {
        std::cout << color << std::endl;
    }
}
```

## foreach 倒序

Python 版本

```python
import unittest

class Test(unittest.TestCase):

    def test_foreach_reversed(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for color in reversed(colors):
            print(color)
```

C++ 版本

```c++
#include <iostream>
#include <vector>
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("foreach reversed") {
    auto colors = std::vector<char const*>{"red", "green", "blue", "yellow"};
    for(const auto& color : colors | view::reverse) {
        std::cout << color << std::endl;
    }
}
```

这里使用了 range-v3 的 view 组合，类似 unix pipe 的语法。

