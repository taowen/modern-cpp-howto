# list 和 map

本节例子选自：

* https://gist.github.com/JeffPaine/6213790
* http://preshing.com/20141202/cpp-has-become-more-pythonic/

对 python 这样的动态语言最直观的感受就是 list/map 两种数据结构打天下。php 和 lua 甚至把这两个都合并成一种数据结构了。
毋庸置疑，学会如何使用 list 和 map 是基础中的基础。

| | Python | C++ |
| -- | -- | -- |
| [for 循环](#foreach-lazy) | `for i in xrange(6)` | `for (auto i : view::ints(0, 6))` |
| [foreach](#foreach) | `for color in colors` | `for (const auto& color : colors)` |
| [foreach 倒序](#foreach-reversed) | `reversed(colors)` | `colors ` &#124; `view::reverse` |




## for 循环 {#foreach-lazy}

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

## foreach {#foreach}

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
根据GDB是 `std::initializer_list<const char*>`。只有 begin，end，size 几个函数。实际上类似于 python 的 tuple。
考虑到 python 的 list 类型是 mutable 的，所以更合适的实现是 std::vector。

```c++
#include <catch_with_main.hpp>

using namespace ranges;

TEST_CASE("foreach on vector") {
    auto colors = std::vector<const char*>{"red", "green", "blue", "yellow"};
    for(const auto& color : colors) {
        std::cout << color << std::endl;
    }
}
```

## foreach 倒序 {#foreach-reversed}

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
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("foreach reversed") {
    auto colors = std::vector<const char*>{"red", "green", "blue", "yellow"};
    for(const auto& color : colors | view::reverse) {
        std::cout << color << std::endl;
    }
}
```

这里使用了 range-v3 的 view 组合，类似 unix pipe 的语法。

## foreach 带下标

Python 版本

```python
import unittest

class Test(unittest.TestCase):
    def test_foreach_with_index(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for i, color in enumerate(colors):
            print(i, color)
```

C++ 版本

```c++
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("foreach with index") {
    auto colors = std::vector<const char*>{"red", "green", "blue", "yellow"};
    for(const auto& [i, color] : view::zip(view::iota(0), colors)) {
        std::cout << i << " " << color << std::endl;
    }
}
```

`view::iota`这个的意思是产生一个从n开始的逐个加一的view，类似python里的generator。然后zip是把两个view逐个对应起来合并成一个pair的view。
然后`const auto& [i, color]`是c++ 17的structured bindings的写法，和python解开tuple里的元素的做法是如出一辙的。

## zip

下面这个例子可以看得更清楚。 Python 版本

```python
import unittest
import itertools

class Test(unittest.TestCase):
    def test_zip(self):
        names = ['raymond', 'rachel', 'matthew']
        colors = ['red', 'green', 'blue', 'yellow']
        for name, color in itertools.izip(names, colors):
            print(name, color)
```

izip返回的是generator。zip返回都是list。C++ 版本

```c++
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("zip") {
    auto names = std::vector<const char*>{"raymond", "rachel", "matthew"};
    auto colors = std::vector<const char*>{"red", "green", "blue", "yellow"};
    for(const auto& [name, color] : view::zip(names, colors)) {
        std::cout << name << " " << color << std::endl;
    }
}
```

## sorted

Python 版本

```python
import unittest

class Test(unittest.TestCase):
    def test_sort(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for color in sorted(colors):
            print(color)
```

C++ 版本

```c++
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("sort") {
    auto colors = std::vector<std::string>{"red", "green", "blue", "yellow"};
    colors |= action::sort;
    for(const auto& color : colors) {
        std::cout << color << std::endl;
    }
}
```

这个例子里`const char*`换成了`std::string`，因为只有字符串类型才知道怎么比较，才能排序。
`action::sort`与view不同，它返回的是具体的container，而不再是view了。

如果要倒过来排序，再 python 中是这样的

```python
import unittest

class Test(unittest.TestCase):
    def test_sort_reverse(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for color in sorted(colors, reverse=True):
            print(color)
```

C++ 版本

```c++
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("sort reverse") {
    auto colors = std::vector<std::string>{"red", "green", "blue", "yellow"};
    colors |= action::sort(std::greater<std::string>());
    for(const auto& color : colors) {
        std::cout << color << std::endl;
    }
}
```

Python还支持指定属性去排序

```python
import unittest

class Test(unittest.TestCase):

    def test_custom_sort(self):
        colors = ['red', 'green', 'blue', 'yellow']
        for color in sorted(colors, key=lambda e: len(e)):
            print(color)
```

C++ 版本

```c++
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("custom sort") {
    auto colors = std::vector<std::string>{"red", "green", "blue", "yellow"};
    colors |= action::sort(std::less<std::string>(), [](const auto&e) {
        return e.size();
    });
    for(const auto& color : colors) {
        std::cout << color << std::endl;
    }
}
```

`sort`的第一个参数是comparator，第二个参数是projector。这里我们使用了一个lambda表达式，从字符串上取得其长度值，用长度去排序。

## any

看看 list 里是否有任意一个元素满足条件。Python 版本

```python
import unittest

class Test(unittest.TestCase):
    def test_any_of(self):
        colors = ['red', 'green', 'blue', 'yellow']
        print(any(color == 'green' for color in colors))
```

C++ 版本

```c++
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("any of") {
    auto colors = std::vector<std::string>{"red", "green", "blue", "yellow"};
    auto result = any_of(colors, [](const auto& e) {
        return e == "green";
    });
    std::cout << result << std::endl;
}
```

其中`any_of`来自于ranges这个namespace。作用和python的any稍微有点不一样。python的any不接受predicate作为参数，要求输入是boolean的
列表，但是any_of支持predicate参数。

## list comprehension

Python 的看家本领

```python
import unittest

class Test(unittest.TestCase):
    def test_list_comprehension(self):
        colors = ['red', 'green', 'blue', 'yellow']
        print([len(color) for color in colors])
```

C++ 版本

```c++
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("list comprehension") {
    auto colors = std::vector<std::string>{"red", "green", "blue", "yellow"};
    auto result = colors | view::transform([](const auto& e) { return e.size(); });
    std::cout << result << std::endl;
}
```

## slicing

Python 版本

```python
import unittest

class Test(unittest.TestCase):
    def test_slicing(self):
        colors = ['red', 'green', 'blue', 'yellow']
        print(colors[1:2])
        print(colors[:2])
        print(colors[1:])
        print(colors[:-1])
```

C++ 版本

```c++
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("slicing") {
    auto colors = std::vector<std::string>{"red", "green", "blue", "yellow"};
    auto colors_view = view::all(colors);
    std::cout << colors_view[{1, 2}] << std::endl;
    std::cout << colors_view[{0, 2}] << std::endl;
    std::cout << colors_view[{1, end}] << std::endl;
    std::cout << colors_view[{0, end-1}] << std::endl;
}
```

## foreach map keys

Python 版本

```python
import unittest

class Test(unittest.TestCase):
    def test_foreach_map_keys(self):
        d = {'matthew': 'blue', 'rachel': 'green', 'raymond': 'red'}
        for k in d.keys():
            if k.startswith('r'):
                del d[k]
        print(d.keys())
```

C++ 版本

```c++
#include <unordered_map>
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("foreach map keys") {
    auto d = std::unordered_map<std::string, std::string>{
            {"matthew", "blue"},
            {"rachel", "green"},
            {"raymond", "red"}
    };
    // 取出 keys，此处不是lazy操作
    auto keys = std::vector<std::string>{d | view::keys};
    for (const auto& k : keys) {
        if (k.find("r") == 0) {
            // 因为 keys 不是lazy操作，此处的删除不会影响遍历
            d.erase(k);
        }
    }
    std::cout << (d | view::keys) << std::endl;
}
```

两个版本主要的区别是python的keys不是lazy的，而c++版本的keys是lazy的。为了支持循环中删除key，必须先 materialize。
而上面的vector实例化就是为了这个目的。这个地方应该还有优化的空间。

# foreach map key/value

这个应该是最常用的map操作了吧。Python 版本

```python
import unittest

class Test(unittest.TestCase):
    def test_foreach_key_value(self):
        d = {'matthew': 'blue', 'rachel': 'green', 'raymond': 'red'}
        for k, v in d.iteritems():
            print(k, v)
```

C++版本

```c++
#include <unordered_map>
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("foreach key/value") {
    auto d = std::unordered_map<std::string, std::string>{
            {"matthew", "blue"},
            {"rachel", "green"},
            {"raymond", "red"}
    };
    for (const auto& [k, v] : d) {
        std::cout << k << " " << v << std::endl;
    }
}
```

## list 构造 map

使用pair构造map, python版本

```python
import unittest

class Test(unittest.TestCase):
    def test_construct_map_by_paris(self):
        colors = ['red', 'green', 'blue', 'yellow']
        print(dict((color, len(color)) for color in colors).values())
```

C++ 版本

```c++
#include <unordered_map>
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

TEST_CASE("construct map by pairs") {
    auto colors = std::vector<std::string>{"red", "green", "blue", "yellow"};
    auto d = std::unordered_map<std::string, int>{
        colors | view::transform([](const auto& e) {
            return std::make_pair(e, e.size());
        })
    };
    std::cout << (d | view::values) << std::endl;
}
```

