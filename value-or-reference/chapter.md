# value 还是 reference

参考资料

* [Back to the Basics! Essentials of Modern C++ Style - Herb Sutter - CppCon 2014](https://github.com/CppCon/CppCon2014/blob/master/Presentations/Back%20to%20the%20Basics!%20Essentials%20of%20Modern%20C%2B%2B%20Style/Back%20to%20the%20Basics!%20Essentials%20of%20Modern%20C%2B%2B%20Style%20-%20Herb%20Sutter%20-%20CppCon%202014.pdf)
* [Writing Good C++14 - Bjarne Stroustrup - CppCon 2015](https://github.com/CppCon/CppCon2015/blob/master/Keynotes/Writing%20Good%20C%2B%2B14/Writing%20Good%20C%2B%2B14%20-%20Bjarne%20Stroustrup%20-%20CppCon%202015.pdf)
* [Writing Good C++14 By Default - Herb Sutter - CppCon 2015](https://github.com/CppCon/CppCon2015/blob/master/Keynotes/Writing%20Good%20C%2B%2B14%20By%20Default/Writing%20Good%20C%2B%2B14%20By%20Default%20-%20Herb%20Sutter%20-%20CppCon%202015.pdf)
* [More C++ Idioms](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms)

java 程序员是幸福的，在 java 的世界里当设计一个 class，实例化一个 object 时需要考虑的东西远比 c++ 要小，所谓“心智负担”。作为一个 python
程序员一直搞不懂什么时候使用什么 c++ 的语法特性。这个不仅仅是垃圾收集的问题，还要考虑到拷贝构造的成本等很多东西。我们把这些“心智负担”做一个
不完整的列表：

* 被建模的概念自身的语义：value v.s entity v.s transient
* 对象被使用方式：by-value v.s reference v.s pointer
* 物理存在：compile-time v.s stack v.s heap v.s thread-local
* 堆上对象的拥有方式: unique_ptr v.s shared_ptr v.s container
* 是否只读: const v.s non-const

当我们需要在代码里对一个真实世界的概念进行建模的时候，需要考虑如此多的方面，难怪c++比java难用那么多。

## 对象自身的语义

参见 http://martinfowler.com/bliki/EvansClassification.html。java开发者，习惯把对象分为三类：

* value object，值对象
* entity object，实体对象
* transient object，不可持久化的对象

### Value Object

Values are usually little things like Date, Money, Database Query.
A Value Object is an object that describes some characteristic or attribute but carries no concept of identity.
不精确的说法是当实现equals相等的时候，value需要所有的属性值相等。
当我们保存value object到关系数据库的时候，不会给value object自己独立的行，而是作为entity object的一部分保存。

### Entity Object

Entities are usually big things like Customer, Ship, Rental Agreement.
Objects that have a distinct identity that runs through time and different representations.
You also hear these called "reference objects".
不精确的说法是当实现equals相等的时候，只需要两个对象的id相等。
当我们保存entity object到关系数据库的时候，每个entity都是独立的一行。

### Transient Object

有一些对象是不会被序列化的，也不会被保存到数据库里的。transient表示它们是短暂地存在于计算机的内存中的，对于真实世界来说没有长远的影响。

## 传参方式

### 单个原生值

单个原生值传递没有什么异议，基本上所有的语言行为都一样。

Python

```python
import unittest

class Test(unittest.TestCase):
    def test_pass_single_value(self):
        local_variable = 1
        print(id(local_variable))
        modify_single_value(local_variable)
        print(local_variable)  # 1，值未被修改

def modify_single_value(single):
    print(id(local_variable)) # 和传入前的地址一样
    single = 2
```

PHP

```php
<?php

function modify_single_value($single)
{
    $single = 2;
}

$local_variable = 1;
modify_single_value($local_variable);
var_dump($local_variable); // 1
```

Go

```go
package main

import (
	"fmt"
)

func main() {
    local_variable := 1
    modify_single_value(local_variable)
    fmt.Println(local_variable) // 1
}

func modify_single_value(single int) {
	single = 2
}
```

C++

```c++
#include <catch_with_main.hpp>

void modify_single_value(int single) {
    single = 2;
}

TEST_CASE("pass single value") {
    auto local_variable = 1;
    modify_single_value(local_variable);
    std::cout << local_variable << std::endl; // 1
}
```

### 单个原生值的引用

Python，啥引用？没这东西。也不支持指针。

PHP

```php
<?php

function modify_single_reference(&$single)
{
    $single = 2;
}

$local_variable = 1;
modify_single_reference($local_variable);
var_dump($local_variable); // 2
```

GO 不支持引用。但是支持指针

```go
package main

import (
	"fmt"
)

func main() {
	{
		local_variable := 1
		modify_single_pointer(&local_variable)
		fmt.Println(local_variable) // 2
	}
}
func modify_single_pointer(single *int) {
	*single = 2
}
```

C++

```c++
#include <catch_with_main.hpp>

void modify_single_reference(int& single) {
    single = 2;
}

TEST_CASE("pass single reference") {
    auto local_variable = 1;
    modify_single_reference(local_variable);
    std::cout << local_variable << std::endl; // 1
}
```

可以看出，php和c++的语法是比较类似的。指针和引用的行为还是差别很大的，饮用可以local_variable这个符号所直接指代的东西给改掉。
而指针不能直接改掉local_variable这个符号的含义。

C++ 同时也支持指针。

```c++
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

void modify_single_pointer(int* single) {
    *single = 2;
}

TEST_CASE("pass single pointer") {
    auto local_variable = 1;
    modify_single_pointer(&local_variable);
    std::cout << local_variable << std::endl; // 1
}
```

根据前辈的说法，总是使用引用，直到无法使用为止。引用相当于不可nullptr的指针。指针和引用就是支持和不支持两种，一旦支持了行为都是一样的。
后面我们主要关注直接传值时各种语言的默认行为。

### list值

Python

```python
import unittest

class Test(unittest.TestCase):
    def test_pass_list_value(self):
        local_variable = [1, 1]
        modify_list_value(local_variable)
        print(local_variable)  # [1, 2]

def modify_list_value(list):
    list[1] = 2
```

在python中传递list的行为类似一个传递指针。虽然这个值本身是按value来传递的。但是其所指向的内存区域是同一个。
所以对传入list的修改，在被调方也是可见的。这种行为是主流编程语言普遍接受的行为，比如 python 和 java。

PHP

```php
<?php

function modify_list_value($list)
{
    $list[1] = 2;
}

$local_variable = [1, 1];
modify_list_value($local_variable);
var_dump($local_variable); // [1, 1]
```

然后 php 却是非常反传统的以拷贝的方式来处理list的传递。

Go

```go
package main

import (
	"fmt"
)

func main() {
	{
		local_variable := []int{1, 1}
		modify_list_value(local_variable)
		fmt.Println(local_variable) // []int{1, 2}
	}
}

func modify_list_value(list []int) {
	list[1] = 2
}
```

对于传递 list 这个事情上，go/java/python 的行为是大多数。

C++

```c++
#include <catch_with_main.hpp>
#include <range/v3/all.hpp>

using namespace ranges;

void modify_list_value(std::vector<int> list) {
    list[1] = 2;
}

TEST_CASE("pass list value") {
    auto local_variable = std::vector<int>{1, 1};
    modify_list_value(local_variable);
    std::cout << (local_variable | view::all) << std::endl; // 1
}
```

C++的语义很清楚，就是copy by default。只要是传值就拷贝一份。这个例子里和 php 行为相同。

### struct值

一般list和map这些都是语言内置，或者标准库支持的。而struct是给我们自己扩充模型的。两者的行为未必相同。先看python的

```python
import unittest

class MyStruct:
    def __init__(self):
        self.field = None


class Test(unittest.TestCase):
    def test_pass_struct_value(self):
        local_variable = MyStruct()
        local_variable.field = 1
        modify_struct_value(local_variable)
        print(local_variable.field)  # 2

def modify_struct_value(struct):
    struct.field = 2
```

到此我们可以断言，python里就没有默认拷贝这样的行为。所有的东西，只要不是原生值，传递的都是指针。对于指向内容的修改，对调用方都是可见的。
简单，清楚，一致。java也是如此，没有任何心智负担。只要传参，就是拷贝一个指针的动作。

PHP

```php
<?php

class MyStruct {
    public $field;
}

function modify_struct_value($struct)
{
    $struct->field = 2;
}

$local_variable = new MyStruct();
$local_variable->field = 1;
modify_struct_value($local_variable);
var_dump($local_variable); // 2, 和 array 不同，object 是不拷贝的
```

php传递一个object和传递一个array居然不一样！传array是拷贝的行为，而传object却只是传递了指针。我又多了一个黑php的理由了。

Go

```go
package main

import (
	"fmt"
)

type MyStruct struct {
	field int
}


func main() {
	{
		local_variable := MyStruct{1}
		local_variable.modify_struct_value()
		fmt.Println(local_variable) // {1}
	}
}

// 何时使用指针
// https://golang.org/doc/faq#methods_on_values_or_pointers
func (self MyStruct) modify_struct_value() {
	self.field = 2
}
```

golang的行为和php一样琢磨不定。之前传递list的行为，貌似和java，python相同，就是拷贝一个指针过去。
但是对于struct，其默认行为又变成了对struct进行浅拷贝。如果想要避免拷贝参数，一般就要加上指针了。除了以下三种情况

* int 等原生类型
* map/list 等原生集合
* interface

golang这样的选择也是不得已为之。因为golang支持stack allocation，如果struct的传递的默认行为是传指针的话，那么任何对象传递出去基本上都算是
escape出这个函数的作用域了，无法优化为stack allocation。所以golang为了保证小的struct仍然可以在栈上分配，迫使大部分程序员在使用struct
时候都要用指针。而java则是另外一个极端，直接把栈上分配内存这个选项给去掉了。但是考虑到golang的用户群基本上和java重叠，需要在乎小struct
的内存分配的用户，估计也不会去用golang吧。所以实际上的结果就是你可以看到的golang代码，大部分都是struct指针的。

C++

```c++
#include <catch_with_main.hpp>

using namespace ranges;

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
```

可以看到c++的行为其实是高度一致的。所有的传值的地方，都是浅拷贝的语义。
而且可以通过拷贝构造函数把浅拷贝变成深拷贝。
我们可以做一个表格对比

| 传值行为 | Python/Java | PHP | Go | C++ |
| --     | --          | -- |  -- |  --  |
| 集合类型 | 拷贝指针 | 拷贝值 | 拷贝指针 | 拷贝值 |
| 自定义对象 | 拷贝指针 | 拷贝指针 | 拷贝值 | 拷贝值 |

python和java，全部都是拷贝指针的行为。
c++全部是默认拷贝值。

## copy v.s const

在 value/entity/transient 的分类下，对于 entity 和 transient 两类对象我们基本上都不需要拷贝的语义。
在go和php这样支持拷贝语义的语言中，我们是无法阻止拷贝的。但是C++可以，方法是把copy constructor关掉。
参见 https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-copyable_Mixin 。

那么对于value object来说，我们就一定使用拷贝么？在理想的世界里，value object全部都是immutable的。所有的传递 value object 都是拷贝，
完全没有引用这样的东西。这样的设计是有其价值的。但是拷贝是有成本的，大型一点的immutable设计都需要有copy on write这样的优化，
而不是无脑地eager copy。显然，c++ 并没有意图在这个方面提供太多的优化。

更常见的 value object 设计是 mutable 的。但是允许只读的访问视图，这个就是 const 。
它可以在不创建新的拷贝的情况下，保证对该变量的访问是只读的。

综上，有这么几种class

* transient，不支持 equals，不能拷贝
* entity, id equals，不能拷贝
* mutable value，支持 equals，可以拷贝
* immutable value，支持 equals，可以拷贝，修改返回新对象

在设计一个class的时候，思考其究竟应该属于哪一种。然后根据下面的样式仿照着来写，而不是使用默认的c++编译器的行为。
java/python 默认支持equals，有时应该禁用。而c++默认支持拷贝构造，有时也应该禁用。
语言的默认是不够的，默认行为应该根据不同的class而分为四种默认行为。

### transient

Python 版本

```python
# coding=utf-8
import copy

# 网络 socket 显然应该属于 transient object

class MySocket(object):
    def __eq__(self, other):
        raise Exception('transient object does not support ==')

    def __ne__(self, other):
        raise Exception('transient object does not support !=')

    def __hash__(self):
        raise Exception('transient object does not support hash')

    def __copy__(self):
        raise Exception('transient object does not support shallow copy')

    def __deepcopy__(self, memo):
        raise Exception('transient object does not support deep copy')

sock1 = MySocket()
sock2 = MySocket()
# print(sock1 == sock2) 会抛异常
# print(sock1 != sock2) 会抛异常
# print(sock1 <> sock2) 会抛异常
# some_map = {sock1: None} 会抛异常
# copy.copy(sock1) 会抛异常
# copy.deepcopy(sock1) 会抛异常
```

C++ 版本

```c++
#include <unordered_map>
#include <catch_with_main.hpp>

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
```

## Copy or Not

何时使用拷贝，何时使用引用，何时使用指针？