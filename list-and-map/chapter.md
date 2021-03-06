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
| [倒序](#reverse) | `reversed(colors)` | `colors ` &#124; `view::reverse` |
| [zip](#zip) | `zip(names, colors)` | `view::zip(names, colors)` |
| [foreach 带下标](#enumerate) | `for i, color in enumerate(colors)` | `for(const auto& [i, color] : view::zip(view::iota(0), colors))` |
| [foreach map](#foreach-map) | `for k, v in d.iteritems()` | `for (const auto& [k, v] : d)` |
| [排序](#sort) | `sorted(colors)` | `action::sort(colors)` |
| [倒序排序](#sort-reverse) | `sorted(colors, reverse=True)` | `action::sort(colors, greater<string>())` |
| [按自定义属性排序](#sort-by-lambda) | `sorted(colors, key=lambda e: len(e))` | ```action::sort(colors, less<int>(), [](const auto& e) { return e.size(); })``` |
| [list comprehension](#list-comprehension) | `[len(color) for color in colors]` | `colors` &#124; `view::transform([](const auto& e) { return e.size(); })` |
| [any](#any) | `any(color == 'green' for color in colors)` | `ranges::any_of(colors, [](const auto& e) { return e == "green"; })` |
| [slicing](#slicing) | `colors[:-1]` | `colors_view[{0, ranges::end-1}]` |
| [list 构造 map](#create-map) | `dict((color, len(color)) for color in colors)` | `unordered_map<string, int>{ colors` &#124; `view::transform([](const auto& e) { return make_pair(e, e.size()); }) }`|

## for 循环 {#foreach-lazy}

Python 版本

```python
def test_foreach_on_lazy_range(self):
    for i in xrange(6):
        print i ** 2
```

C++ 版本

```c++
TEST_CASE("foreach on lazy range") {
    for(const auto& x : view::ints(0, 6)) {
        cout << x * x << endl;
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
def test_foreach_on_list(self):
    colors = ['red', 'green', 'blue', 'yellow']
    for color in colors:
        print color
```

C++ 版本

```c++
TEST_CASE("foreach on list") {
    auto colors = {"red", "green", "blue", "yellow"};
    for(const auto& color : colors) {
        cout << color << endl;
    }
}
```

与python不同，c++没有所谓的默认的list类型。上面的写法是最简洁的写法。colors 变量的实际类型
根据GDB是 `initializer_list<const char*>`。只有 begin，end，size 几个函数。实际上类似于 python 的 tuple。
考虑到 python 的 list 类型是 mutable 的，所以更合适的实现是 vector。

```c++
TEST_CASE("foreach on vector") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    for(const auto& color : colors) {
        cout << color << endl;
    }
}
```


## 倒序 {#reverse}

Python 版本

```python
def test_reversed(self):
    colors = ['red', 'green', 'blue', 'yellow']
    self.assertListEqual(['yellow', 'blue', 'green', 'red'], list(reversed(colors)))
```

C++ 版本

```c++
TEST_CASE("reverse") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    CHECK((vector<string>{"yellow", "blue", "green", "red"})
          ==
          (colors | view::reverse | to_vector));
}
```

这里使用了 range-v3 的 view 组合，类似 unix pipe 的语法。

## zip {#zip}

下面这个例子可以看得更清楚。 Python 版本

```python
def test_zip(self):
    names = ['raymond', 'rachel', 'matthew']
    colors = ['red', 'green', 'blue', 'yellow']
    self.assertListEqual([
        ('raymond', 'red'),
        ('rachel', 'green'),
        ('matthew', 'blue')
    ], zip(names, colors))
```

C++ 版本

```c++
TEST_CASE("zip") {
    auto names = vector<string>{"raymond", "rachel", "matthew"};
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto zipped = view::zip(names, colors);
    CHECK((vector<pair<string, string>>{
               {"raymond", "red"}, {"rachel", "green"}, {"matthew", "blue"}
           })
          ==
          (zipped | to_vector));
    for(const auto& [name, color] : zipped) {
        cout << name << " " << color << endl;
    }
}
```

## foreach 带下标 {#enumerate}

Python 版本

```python
def test_foreach_with_index(self):
    colors = ['red', 'green', 'blue', 'yellow']
    for i, color in enumerate(colors):
        print(i, color)
```

C++ 版本

```c++
TEST_CASE("foreach with index") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    for(const auto& [i, color] : view::zip(view::iota(0), colors)) {
        cout << i << " " << color << endl;
    }
}
```

`view::iota`这个的意思是产生一个从n开始的逐个加一的view，类似python里的generator。然后zip是把两个view逐个对应起来合并成一个pair的view。
然后`const auto& [i, color]`是c++ 17的structured bindings的写法，和python解开tuple里的元素的做法是如出一辙的。

# foreach map {#foreach-map}

这个应该是最常用的map操作了吧。Python 版本

```python
def test_foreach_map(self):
    d = {'matthew': 'blue', 'rachel': 'green', 'raymond': 'red'}
    for k, v in d.iteritems():
        print(k, v)
```

C++版本

```c++
TEST_CASE("foreach map") {
    auto d = unordered_map<string, string>{
            {"matthew", "blue"},
            {"rachel", "green"},
            {"raymond", "red"}
    };
    for (const auto& [k, v] : d) {
        cout << k << " " << v << endl;
    }
}
```

## 排序 {#sort}

Python 版本

```python
def test_sort(self):
    colors = ['red', 'green', 'blue', 'yellow']
    self.assertListEqual(['blue', 'green', 'red', 'yellow'], sorted(colors))
```

C++ 版本

```c++
TEST_CASE("sort") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto sorted = action::sort(colors);
    CHECK((vector<string>{"blue", "green", "red", "yellow"})
          ==
          (sorted | to_vector));
}
```

`action::sort`与view不同，它返回的是具体的container，而不再是view了。

## 倒序排序 {#sort-reverse}

如果要倒过来排序，再 python 中是这样的

```python
def test_sort_reverse(self):
    colors = ['red', 'green', 'blue', 'yellow']
    self.assertListEqual(['yellow', 'red', 'green', 'blue'], sorted(colors, reverse=True))
```

C++ 版本

```c++
TEST_CASE("sort reverse") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto sorted = action::sort(colors, greater<string>());
    CHECK((vector<string>{"yellow", "red", "green", "blue"})
          ==
          (sorted | to_vector));
}
```

## 按自定义属性排序 {#sort-by-lambda}

Python还支持指定属性去排序

```python
def test_sort_by_lambda(self):
    colors = ['red', 'green', 'blue', 'yellow']
    self.assertListEqual(['red', 'blue', 'green', 'yellow'], sorted(colors, key=lambda e: len(e)))
```

C++ 版本

```c++
TEST_CASE("sort by lambda") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto sorted = action::sort(colors, less<int>(), [](const auto& e) {
        return e.size();
    });
    CHECK((vector<string>{"red", "blue", "green", "yellow"})
          ==
          (sorted | to_vector));
}
```

`sort`的第一个参数是comparator，第二个参数是projector。这里我们使用了一个lambda表达式，从字符串上取得其长度值，用长度去排序。

## list comprehension {#list-comprehension}

Python 的看家本领

```python
def test_list_comprehension(self):
    colors = ['red', 'green', 'blue', 'yellow']
    self.assertListEqual([3, 5, 4, 6], [len(color) for color in colors])
```

C++ 版本

```c++
TEST_CASE("list comprehension") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto actual = vector<int>{
        colors | view::transform([](const auto& e) { return e.size(); })
    };
    CHECK((vector<int>{3, 5, 4, 6}) == actual);
}
```

## any {#any}

看看 list 里是否有任意一个元素满足条件。Python 版本

```python
def test_any_of(self):
    colors = ['red', 'green', 'blue', 'yellow']
    self.assertTrue(any(color == 'green' for color in colors))
```

C++ 版本

```c++
TEST_CASE("any of") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    CHECK(ranges::any_of(colors, [](const auto& e) {
        return e == "green";
    }));
}
```

其中`any_of`来自于ranges这个namespace。作用和python的any稍微有点不一样。python的any不接受predicate作为参数，要求输入是boolean的
列表，但是any_of支持predicate参数。


## slicing {#slicing}

Python 版本

```python
def test_slicing(self):
    colors = ['red', 'green', 'blue', 'yellow']
    self.assertListEqual(['green'], colors[1:2])
    self.assertListEqual(['red', 'green'], colors[:2])
    self.assertListEqual(['green', 'blue', 'yellow'], colors[1:])
    self.assertListEqual(['red', 'green', 'blue', 'yellow'], colors[:-1])
```

C++ 版本

```c++
TEST_CASE("slicing") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto colors_view = colors | view::all;
    CHECK((vector<string>{"green"})
          ==
          (colors_view[{1, 2}] | to_vector));
    CHECK((vector<string>{"red", "green"})
          ==
          (colors_view[{0, 2}] | to_vector));
    CHECK((vector<string>{"green", "blue", "yellow"})
          ==
          (colors_view[{1, ranges::end}] | to_vector));
    CHECK((vector<string>{"red", "green", "blue"})
          ==
          (colors_view[{0, ranges::end-1}] | to_vector));
}
```

## list 构造 map {#create-map}

使用pair构造map, python版本

```python
def test_construct_map_by_paris(self):
    colors = ['red', 'green', 'blue', 'yellow']
    self.assertDictEqual({
        'red': 3,
        'green': 5,
        'blue': 4,
        'yellow': 6
    }, dict((color, len(color)) for color in colors))
```

C++ 版本

```c++
TEST_CASE("construct map by pairs") {
    auto colors = vector<string>{"red", "green", "blue", "yellow"};
    auto d = unordered_map<string, int>{
        colors | view::transform([](const auto& e) {
            return make_pair(e, e.size());
        })
    };
    CHECK((unordered_map<string, int>{
                    {"red", 3},
                    {"green", 5},
                    {"yellow", 6},
                    {"blue", 4}
        }) == d);
}
```

