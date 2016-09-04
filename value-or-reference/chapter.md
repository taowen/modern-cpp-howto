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

* [对象的业务含义](#business-meaning)
* [c++是值拷贝的语言](#value-copy)
* [正确使用值对象的姿势](#value-object-howto)
* [使用c++的语义满足业务建模的需求](#application)

## 对象的业务含义 {#business-meaning}

无论是传值还是传引用，不管用什么语言，编程总是用来解决真实世界中的问题的。我们要在程序语言中建模的对象无非这么几种。
参见 http://martinfowler.com/bliki/EvansClassification.html 。Java企业应用开发者，习惯于把对象分为三类：

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

| 对象类型 | 拷贝 | 比较是否相等 | 
| --      | --   | --       |
| value   | 可以 | 所有值相等 |
| entity  | 不可 | id相等    |
| transient | 不可 | 不可    |

然后我们来看在c++中应该怎么样把其名目繁多的语法映射到我们的需求上来。

## c++是值拷贝的语言 {#value-copy}

不同语言在参数传递的时候行为有很大的不同。理解C++对于其他语言背景的开发一个很大的挑战在于其默认的value copy的行为。
这个行为可以通过以下一系列和其他语言的对比清楚地展现出来：

### 传递单个原生值

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

### 传递单个原生值的引用

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

### 传递list类型的值

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

### 传递struct类型值

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

## 正确使用值对象的姿势 {#value-object-howto}

值拷贝是非常昂贵的，值对像对于初学者来说非常容易误用。这种误用是语言的，而不是人的问题。

### 使用 const & 代表我只是使用者

```c++
class Name {
public:
  string firstName;
  string lastName;

  // no arg default constructor
  Name() = default;

  // contructor for actual name
  Name(string_view firstName, string_view lastName)
      : firstName(firstName), lastName(lastName) {}
};

void printFriends(vector<Name> friends) {
  Name theName;
  for (size_t i = 0; i < friends.size(); i++) {
    theName = friends[i];
    cout << theName.firstName << " " << theName.lastName << endl;
  }
}

TEST_CASE("expensive copy") {
  auto friends = vector<Name>{
      {"Meliton", "Soso"}, {"Bidzina", "Iona"}, {"Archil", "Revazi"}};
  printFriends(friends);
}
```

这段代码表面上看过去没有问题。编译正常，执行之后输出是

```
Meliton Soso
Bidzina Iona
Archil Revazi    
```

对于python和java这样不是默认值拷贝的语言来说，写法是完全ok的。但是这里面隐藏了很多次不必要的值拷贝过程：

```c++
class Name {
public:
  string firstName;
  string lastName;

  // no arg default constructor
  Name() = default;

  // contructor for actual name
  Name(string_view firstName, string_view lastName)
      : firstName(firstName), lastName(lastName) {}

  // copy constructor
  Name(Name const &that) : firstName(that.firstName), lastName(that.lastName) {
    cout << "copied" << endl;
  }

  // copy assignment
  Name &operator=(Name const &that) {
    firstName = that.firstName;
    lastName = that.lastName;
    cout << "copy assigned" << endl;
    return *this;
  }
};

void printFriends(vector<Name> friends) {
  cout << "=== 3 ===" << endl;
  Name theName;
  cout << "=== 4 ===" << endl;
  for (size_t i = 0; i < friends.size(); i++) {
    theName = friends[i];
    cout << theName.firstName << " " << theName.lastName << endl;
  }
  cout << "=== 5 ===" << endl;
}

TEST_CASE("expensive copy") {
  cout << "=== 1 ===" << endl;
  auto friends = vector<Name>{
      {"Meliton", "Soso"}, {"Bidzina", "Iona"}, {"Archil", "Revazi"}};
  cout << "=== 2 ===" << endl;
  printFriends(friends);
}
```

输出是
```
=== 1 ===
copied
copied
copied
=== 2 ===
copied
copied
copied
=== 3 ===
=== 4 ===
copy assigned
Meliton Soso
copy assigned
Bidzina Iona
copy assigned
Archil Revazi
=== 5 ===
```
要把这些拷贝过程都去掉，正确的写法是
```c++
void printFriendsByReference(vector<Name> const &friends) {
  for (size_t i = 0; i < friends.size(); i++) {
    Name const &theName = friends[i];
    cout << theName.firstName << " " << theName.lastName << endl;
  }
}

TEST_CASE("without expensive copy") {
  auto friends = vector<Name>();
  friends.reserve(3); // to avoid resize vector causing copy
  friends.emplace_back("Meliton", "Soso");
  friends.emplace_back("Bidzina", "Iona");
  friends.emplace_back("Archil", "Revazi");
  printFriendsByReference(friends);
}
```
这里遇到了以下几个问题
* 如果不是用emplace系列的方法把新建对象插入到容器里就会产生拷贝行为，emplace新建的对象是直接把内存分配在容器内部的
* 默认传参行为是拷贝，而传递一个vector就会拷贝每一个元素。所以当对象不是你拥有的，你只是使用者时把类型标记为 const & 
* 对值对象的复制会产生拷贝赋值行为，当一个变量只是在使用而不是拥有时，把类型标记为 const &

值对象好难用啊。是不是能不用值对象，而只用指针，然后就能像python和java一样呢？如果是那样的话，直接去用python或者java好了。
值对象是c++的基础。

### 直接使用值，代表我是其拥有者

c++需要你手工控制对象的所有权。当我们要表示我需要拥有这个对象时，直接持有值。

```c++
class File {
public:
  File(string_view fileName) { fileHandle = fopen(fileName.data(), "r"); }
  ~File() { fclose(fileHandle); }
  string readAll() {
    fseek(fileHandle, 0, SEEK_END);
    auto size = static_cast<size_t>(ftell(fileHandle));
    auto buf = string(size, ' ');
    fseek(fileHandle, 0, SEEK_SET);
    fread(&buf[0], 1, size, fileHandle);
    return buf;
  }

private:
  File(File const &) = delete;            // disable copy
  File &operator=(File const &) = delete; // disable copy assign
  FILE *fileHandle;
};

TEST_CASE("read_file") {
  File hostsFile("/etc/hosts");
  cout << hostsFile.readAll() << endl;
}
```

当我们拥有hostsFile这个变量的时候，我们也拥有了这个变量所指向的资源（堆上的内存，文件句柄，网络socket等）。
在当前函数退出的时候，这个变量会被析构，从而释放拥有的资源。所有值对象，都代表了我是其使用者。

### 工厂方法

如果值对象代表了所有权。那么如何实现工厂方法呢？

```c++
auto openHostsFile() { return File("/etc/hosts"); }

TEST_CASE("use factory") {
  //  won't compile with copy constructor disabled
  auto hostsFile = openHostsFile();
  cout << hostsFile.readAll().size() << endl;
}
```

目测这里会拷贝两次File对象啊。既然我们把拷贝构造函数禁用了，这里自然不编译通过了。如果把拷贝构造函数打开
```c++
// copy constructor
  File(File const &that) {
    fileHandle = that.fileHandle;
    cout << "copied\n";
  }
```
这段代码是错误的，因为拷贝的结果是两个File握有同一个句柄，导致一个fclose之后另外一个无法使用。但是这不是重点。关键是copied并没有被输出。
也就是工厂方法可以直接返回值本身，不用担心拷贝的问题。

一次构造两个对象也是可以的

```c++
auto giveTwoNames() {
  return make_pair(Name("Meliton", "Soso"), Name("Bidzina", "Iona"));
}

TEST_CASE("return tuple") {
  auto[name1, name2] = giveTwoNames();
  cout << name1.firstName << " " << name1.lastName << endl;
  cout << name2.firstName << " " << name2.lastName << endl;
}
```

成本只有make_pair是发生的两次move构造。如果想把这两次move的成本也省去也是可以的：

```c++
auto giveTwoNamesWithoutMove() {
  // 内存直接分配在pair上，而不是先构造好一个对象再move到pair内部
  // emplace 的增强版，piecewise construct
  return pair<Name, Name>(std::piecewise_construct,
                          std::forward_as_tuple("Meliton", "Soso"),
                          std::forward_as_tuple("Bidzina", "Iona"));
}

TEST_CASE("return tuple more efficiently") {
  auto[name1, name2] = giveTwoNamesWithoutMove();
  cout << name1.firstName << " " << name1.lastName << endl;
  cout << name2.firstName << " " << name2.lastName << endl;
}
```

这下就完全没有额外的成本了。

### 所有权转移

```c++
class File {
public:
  File(string_view fileName) { fileHandle = fopen(fileName.data(), "r"); }
  ~File() {
    if (nullptr != fileHandle) {
      fclose(fileHandle);
    }
  }

  // move constructor
  File(File &&that) {
    this->fileHandle = that.fileHandle;
    that.fileHandle = nullptr;
    cout << "moved\n";
  }

  string readAll() {
    fseek(fileHandle, 0, SEEK_END);
    auto size = static_cast<size_t>(ftell(fileHandle));
    auto buf = string(size, ' ');
    fseek(fileHandle, 0, SEEK_SET);
    fread(&buf[0], 1, size, fileHandle);
    return buf;
  }

private:
  File(File const &) = delete;            // disable copy
  File &operator=(File const &) = delete; // disable copy assign
  FILE *fileHandle;
};

File openHostsFile() { return File("/etc/hosts"); }

void printFileContent(File &&file) { cout << file.readAll().size() << endl; }

TEST_CASE("move ownership") {
  auto hostsFile = openHostsFile();
  printFileContent(std::move(hostsFile));
  // can not access hostsFile again, as the resource ownership has been moved
}
```

当我们传递参数的时候，如果用 const & 表示只是给你用。如果传递的是原值，这个时候会出发拷贝构造函数，表示共享资源的所有权。
如果对象不允许这样的共享，应该禁用拷贝构造函数。如果我们传递的是 std::move() 这个时候表示的是我放弃对这个资源的所有权，它以后就是你的了。
如果这份资源是堆上的内存的话，copy的意思是复制整个堆上的内容，move的意义就是把指针交给你，我的指针变成nullptr。

在上面的例子里，调用printFileContent的时候，是把hostsFile的所有权转移走了。当然这个例子并不好，因为实际上传 const & 是更符合的做法。
std::move的使用表示我们可以这么做。`printFileContent(File &&file)` 这个写法表示我需要拥有一份这个资源。
事实上因为有了右值引用，我们可以永远不使用直接接受值的方法了。当我们可以写

```
void f(A&& a)
```

为什么还需要？

```
void f(A a)
```

使用右值引用，所有权是显式转移的，比如下面的写法是不work的

```
A a;
f(a); // 语法错误，a是一个左值 
```

如果我们需要共享所有权给f，那么就复制
```
A a;
f(copy(a)); // 复制一份成为右值
```

要么就释放自己的所有权，转移给f
```
A a;
f(std::move(a)); // 所有权转移
```

一下子变清楚了，比`f(a)`这种写法清楚多了。因为`f(a)`不是给f使用a的语义，其实是复制一份所有权的意思，大部分情况下不是我们想要的。
而使用 && 右值引用，调用方无论是拷贝一份给我们，还是转移自己的给我们，都可以，使用上也更灵活。
所以`void f(A a)` 这样的写法可以退出历史舞台了。


### defaults

因为 c++ 语言的设计缺陷，使得我们需要用大量的惯用法来克服语言自身设计的缺陷。比如大部分场景上我们只需要 const 使用，为什么 const 不是默认行为？
大部分情况下我们只是希望拿到一个引用，而值拷贝为什么是默认行为而不是取引用？ const & 是对计算机来说成本最低的实现，而对程序员来说使用
const & 需要多打好多个字符呢。

何时使用value，何时使用pointer？ 自己写的代码里基本上不需要pointer，全部用value都可以。因为大部分指针都包含在vector这样的容器内部了。
一个自定义类型很难写出非常多的field导致拷贝成本很高的。一般来说只有vector这样动态变长的东西，才需要使用指针。
而标准库已经把这些不好写的家伙都写好了。在 modern c++ 中可以严肃地考虑一下全部使用value和reference完成编码了，
而不再需要直接操作pointer。

下面是拼凑出来的一个查找表，用于减少心智负担。当看到了左边的写法的时候，看看右边就知道在表达什么意思了。

| 场景 | 写法 | 含义 |
| --  |  -- | -- |
| 用函数生命周期持有资源| void f() { A a; ... } | 在f函数的生命周期内，拥有a所持有的资源。f是a的owner |
| 用对象持有资源| obj.field = some_value // field 是一个值类型 | 拷贝一份some_value，obj和some_value共享对底层资源的所有权 |
| 用对象持有资源 | obj.field = std::move(some_value) | 由obj开始负责some_value原来负责的资源 |
| 函数签名 | void f(A &&a) | 我需要拥有这个参数，是否是独占的取决传入方 |
| 函数签名 | void f(A const &a) | 我只是一个普通的使用者而已，a不可空 |
| 调用函数 | f(copy(a)) | 复制一份a的所有权，交给f |
| 调用函数 | f(std::move(a)) | 放弃对a的所有权，移交给f |
| 调用函数 | f(a) | 假定已经没有隐式拷贝构造和隐式类型转换这两个邪恶的存在的话，我们可以认为是把a给f用一用，但是f并不拥有a |
| 返回值 | A f() { return A(); } | 用值类型返回是非常高效的 |
| 返回值 | void f(A &a) | a既是参数又是返回值 |

纵观上面的表格，什么时候需要使用copy constructor？ copy constructor 就不应该存在，是语言的一个bug。
所有的拷贝都应该是显式的方法调用的形式。
什么时候需要`void f(A a)`写法了？没有一处。要么用 && 要么用 const &。

对于拷贝构造函数，这段chrome项目里的注释说明了一种态度：
```
// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) 
  TypeName(const TypeName&);               
  void operator=(const TypeName&)

// An older, deprecated, politically incorrect name for the above.
// NOTE: The usage of this macro was baned from our code base, but some
// third_party libraries are yet using it.
// TODO(tfarina): Figure out how to fix the usage of this macro in the
// third_party libraries and get rid of it.
#define DISALLOW_EVIL_CONSTRUCTORS(TypeName) DISALLOW_COPY_AND_ASSIGN(TypeName)

class Foo {
public:
  int bar();
private:
DISALLOW_EVIL_CONSTRUCTORS(Foo);
};
```
拥有默认拷贝构造函数，以及没有右值引用的c++ 98是丑陋的。如果我们可以从现在开始抛弃拷贝构造函数，拥抱右值引用，modern c++可以变得非常可爱。
当我们看一眼函数的签名，就知道这些参数是被怎样使用的，是非常爽的事情。

## 通过智能指针持有资源 {#smart-pointer}

前面看到的所有资源都是被value持有的。在c++里，持有资源还有另外两种方式，unique_ptr 和 shared_ptr。

| 写法 | 含义 |
| --  |  -- |
| A a | 在函数生命周期持有资源 |
| obj.field = a | 在对象的生命周期持有资源 |
| vector<A> aList | 利用vector代持资源 |
| unique_ptr<A> a | a通过指针唯一持有资源 |
| shared_ptr<A> a | a通过指针共享资源，智能计数 |
| vector<unique_ptr<A>> aList | 利用vector代持资源，支持继承多态 |

什么情况下pointer比reference有优势？原则上尽量使用value和reference，除非以下三种情况：
* 值可空，因为reference不可空。同时可以考虑用std::optional替代
* 资源需要共享使用，利用shared_ptr引用计数
* `vector<unique_ptr<A>>`，如果vector里放的是具体类型的话，就不支持多态了

### 引用计数

```
auto openFile(string_view fileName) { return make_unique<File>(fileName); }

auto printInNewThread(shared_ptr<File> file) {
  cout << file->readAll() << endl;
}

TEST_CASE("sharing resource") {
  // make resource shared
  auto etcHostsFile = shared_ptr<File>(std::move(openFile("/etc/hosts")));
  thread thread1(printInNewThread, etcHostsFile);
  thread thread2(printInNewThread, etcHostsFile);
  etcHostsFile = nullptr; // release my copy
  thread1.join();
  thread2.join();
}
```

这个例子里 `shared_ptr<File>` 记录了有多少个线程还持有了它的资源。当最后一个线程退出的时候才会释放资源。
利用shared_ptr，我们所有的资源管理类都具有了引用计数的功能。而unique_ptr大部分时候可以被value替代。

### 容器元素的多态

```
class Employee {
public:
  Employee() = default;
  virtual ~Employee() {}
  virtual void calculateSalary() const = 0;
};

class RegularEmployee : public Employee {
  void calculateSalary() const override { cout << "regular employee\n"; }
};

class HourlyEmployee : public Employee {
  void calculateSalary() const override { cout << "hourly employee\n"; }
};

TEST_CASE("container element polymorphism") {
  auto employeeList = vector<unique_ptr<Employee>>();
  employeeList.push_back(make_unique<RegularEmployee>());
  employeeList.push_back(make_unique<HourlyEmployee>());
  employeeList.push_back(make_unique<RegularEmployee>());
  for (auto const &employee : employeeList) {
    employee->calculateSalary();
  }
}
```

用一个容器来保存不同类型的元素，并且具有多态的行为，这个是value和reference无法做到的。

```
TEST_CASE("container element polymorphism by reference") {
  auto regularEmployeeList = vector<RegularEmployee>();
  regularEmployeeList.emplace_back();
  auto hourlyEmployeeList = vector<HourlyEmployee>();
  hourlyEmployeeList.emplace_back();
  hourlyEmployeeList.emplace_back();
  for (Employee const &employee : regularEmployeeList) {
    employee.calculateSalary();
  }
  for (Employee const &employee : hourlyEmployeeList) {
    employee.calculateSalary();
  }
}
```

具体类型容器的主要不便利的地方在于无法保存不同子类的元素。

## 使用c++的语义满足业务建模的需求 {#application}

在 value/entity/transient 的分类下，对于 entity 和 transient 两类对象我们基本上都不需要拷贝的语义。
在go和php这样支持拷贝语义，但是配套工具并不齐全的语言中，我们是无法阻止拷贝的。但是C++可以，方法是把copy constructor关掉。
参见 https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-copyable_Mixin 。

那么对于value object来说，我们就一定使用拷贝么？在理想的世界里，value object全部都是immutable的。所有的传递 value object 都是拷贝，
完全没有引用这样的东西。这样的设计是有其价值的。但是拷贝是有成本的，大型一点的immutable设计都需要有copy on write这样的优化，
而不是无脑地eager copy。显然，c++ 并没有意图在这个方面提供太多的优化。

更常见的 value object 设计是 mutable 的。但是允许只读的访问视图，这个就是 const 。
它可以在不创建新的拷贝的情况下，保证对该变量的访问是只读的。

综上，有这么几种class，我们需要来看在c++中怎么实现

* transient，不支持 equals，不能拷贝
* entity, id equals，不能拷贝
* mutable value，支持 equals，可以拷贝
* immutable value，支持 equals，可以拷贝，修改返回新对象。在python的list是mutable的，tuple是immutable的。list不可以作为hash key，
而tuple可以。

| 分类 | copyable | movable |  overload == | std::hash | 例子 | 
| --  |  --       | --      | --          | -- |  -- |
| transient | NO  | YES    | NO  |  NO | 网络socket，文件句柄 |
| entity    | NO  | YES    | YES (by id) | NO | 司机，乘客，账户，订单 |
| mutable value  | YES | YES | YES (by values) | NO | 各种容器，vector，unordered_map |
| immutable value | YES | NO | YES (by values) | YES | 基本值，int，long，日期，钱 |

### transient

| 分类 | copyable | movable |  overload == | std::hash | 例子 | 
| --  |  --       | --      | --          | -- |  -- |
| transient | NO  | YES    | NO  |  NO | 网络socket，文件句柄 |

Python 版本

```python
# coding=utf-8
import copy

class File(object):
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

file1 = File()
file2 = File()
# print(file1 == file2) 会抛异常
# print(file1 != file2) 会抛异常
# some_map = {file1: None} 会抛异常
# copy.copy(file1) 会抛异常
# copy.deepcopy(file1) 会抛异常
```

C++ 版本

```c++
class File {
public:
  File(string_view fileName) { fileHandle = fopen(fileName.data(), "r"); }
  ~File() {
    if (nullptr != fileHandle) {
      fclose(fileHandle);
    }
  }

  // move constructor
  File(File &&that) {
    this->fileHandle = that.fileHandle;
    that.fileHandle = nullptr;
    cout << "moved\n";
  }

private:
  File(File const &) = delete;            // disable copy
  File &operator=(File const &) = delete; // disable copy assign
  FILE *fileHandle;
};

TEST_CASE("transient object") {
  File file1("/etc/hosts");
  File file2("/etc/hosts");
  // file1 == file2; 没有默认实现的 ==，无需禁用
  // file1 != file2; 没有默认实现的 !=，无需禁用
  // unordered_map<File, int> some_map{}; 编译错误，MySocket没有实现hash
  // auto file3 = file; copy constructor 已经被禁用
  // file2 = file1; assignment constructor 已经被禁用
  auto file3 = std::move(file1); // file1的资源所有权转移给了file3
}
```

### entity


| 分类 | copyable | movable |  overload == | std::hash | 例子 | 
| --  |  --       | --      | --          | -- |  -- |
| entity    | NO  | YES    | YES (by id) | NO | 司机，乘客，账户，订单 |

Python 版本

```
class Order(object):
    def __init__(self, order_id, *order_lines):
        self.order_id = order_id
        self.order_lines = list(order_lines)

    def __eq__(self, other):
        return self.order_id == other.order_id

    def __ne__(self, other):
        return not (self == other)

    def __hash__(self):
        return hash(self.order_id)

    def __copy__(self):
        raise Exception('entity object does not support shallow copy')

    def __deepcopy__(self, memo):
        raise Exception('entity object does not support deep copy')

order1 = Order(101, 'one fish', 'one apple')
order2 = Order(101, 'one Fish')
print(order1 == order2) # True
print(order1 != order2) # False
some_map = {order1: None} # works
# copy.copy(order1) 会抛异常
# copy.deepcopy(order1) 会抛异常
```

C++版本

```
class Order {
public:
  Order(int orderId, initializer_list<string_view> orderLines)
      : orderId(orderId) {
    for (auto const &orderLine : orderLines) {
      this->orderLines.emplace_back(orderLine);
    }
  }
  // move constructor
  Order(Order &&that) {
    orderId = that.orderId;
    that.orderId = 0;
    orderLines = std::move(that.orderLines);
  }

  auto getOrderId() const { return orderId; }

private:
  // disable copy constructor
  Order(Order const &that) = delete;
  // disable copy assignment
  Order &operator=(Order const &that) = delete;

  int orderId;
  vector<string> orderLines;
};

bool operator==(Order const &left, Order const &right) {
  return left.getOrderId() == right.getOrderId();
}

bool operator!=(Order const &left, Order const &right) {
  return !(left == right);
}

namespace std {
template <> struct hash<Order> {
  size_t operator()(Order const &x) const {
    return hash<int>()(x.getOrderId());
  }
};
}

TEST_CASE("entity object") {
  auto order1 = Order(101, {"one fish", "one apple"});
  auto order2 = Order(101, {"one Fish"});
  cout << (order1 == order2) << endl;
  cout << (order1 != order2) << endl;
  auto some_map = unordered_map<Order, int>{};
  some_map.emplace(std::move(order2), 1);
  // auto order3 = order1; copy constructor deleted
  // order2 = order1; copy assignment deleted
  auto order3 = std::move(order1); // move is ok
}
```

这里使用了std::hash支持了object作为map的key来使用。
但是因为entity是non copyable的，所以只能用emplace插入元素。


### mutable value

| 分类 | copyable | movable |  overload == | std::hash | 例子 | 
| --  |  --       | --      | --          | -- |  -- |
| mutable value  | YES | YES | YES (by values) | NO | 各种容器，vector，unordered_map |

PurchaseItems 是一个订单的值，而不是订单本身。其自身是没有id的，两个人是不是买了同样的东西，比较PurchaseItems就可以了。
因为PurchaseItems本身是一个容器，所以mutable的行为也很正常。

Python 版本

```
class PurchaseItems(object):
    def __init__(self):
        self.items = [] # list of (sku, quantity)
        self.coupon_id = None # the coupon to use

    def __eq__(self, other):
        return self.items == other.items and self.coupon_id == other.coupon_id

    def __ne__(self, other):
        return not (self == other)

    def __hash__(self):
        raise Exception('mutable do not support hash')

    def __copy__(self):
        copied = PurchaseItems()
        copied.items = list(self.items)
        copied.coupon_id = self.coupon_id
        return copied

    def __deepcopy__(self, memo):
        copied = PurchaseItems()
        copied.items = copy.deepcopy(self.items, memo)
        copied.coupon_id = self.coupon_id
        return copied

p1 = PurchaseItems()
p1.items.append(('S102', 1))
p1.items.append(('S159', 2))
p1.coupon_id = 10001
p2 = PurchaseItems()
p2.items.append(('S102', 1))
p2.items.append(('S159', 2))
p2.coupon_id = 10001
print(p1 == p2) # True
print(p1 != p2) # False
# some_map = {p1: None} # exception
copy.copy(p1)
copy.deepcopy(p1)
```

C++ 版本

```
class PurchaseItems {
public:
  vector<pair<string, int>> items;
  int couponId;
  PurchaseItems() = default;
  // move constructor
  PurchaseItems(PurchaseItems &&that) {
    items = std::move(that.items);
    couponId = that.couponId;
  }

private:
  PurchaseItems(PurchaseItems const &) = delete;
  PurchaseItems &operator=(PurchaseItems const &that) = delete;
};

bool operator==(PurchaseItems const &left, PurchaseItems const &right) {
  return left.items == right.items && left.couponId == right.couponId;
}

bool operator!=(PurchaseItems const &left, PurchaseItems const &right) {
  return !(left == right);
}

PurchaseItems duplicate(PurchaseItems const &copyFrom) {
  PurchaseItems copied;
  copied.items = copyFrom.items;
  copied.couponId = copyFrom.couponId;
  return copied;
}

TEST_CASE("mutable value object") {
  PurchaseItems p1;
  p1.items.emplace_back("S102", 1);
  p1.items.emplace_back("S159", 2);
  PurchaseItems p2;
  p2.items.emplace_back("S102", 1);
  p2.items.emplace_back("S159", 2);
  cout << (p1 == p2) << endl;
  // unordered_map<PurchaseItems, int> some_map{};
  // 编译错误，PurchaseItems没有实现hash
  auto p3 = duplicate(p1);
  auto p4 = std::move(p3);
}
```

虽然mutable value是可以拷贝的，我们仍然选择把copy constructor禁用了。这样 `auto p3 = p1` 就是不合法的了，
必须写`auto p3 = duplicate(p1)`。明显后者更能体现真实发生情况。

### immutable value

| 分类 | copyable | movable |  overload == | std::hash | 例子 | 
| --  |  --       | --      | --          | -- |  -- |
| immutable value | YES | NO | YES (by values) | YES | 基本值，int，long，日期，钱 |

```ES
class Point(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __ne__(self, other):
        return not (self == other)

    def __hash__(self):
        return 31 * hash(self.x) + hash(self.y)

    def __copy__(self):
        return self

    def __deepcopy__(self, memo):
        return self

    def move(self, offset_x, offset_y):
        return Point(self.x + offset_x, self.y + offset_y)

p1 = Point(100, 50)
p2 = Point(100, 70)
print(p1 == p2)
print(p1 != p2.move(0, -20))
some_map = {p1: None} # works
copy.copy(p1)
copy.deepcopy(p1)
```

C++ 版本

```
class Point {
public:
  Point(int x, int y) : x(x), y(y) {}
  // copy constructor, to enable return by value
  Point(Point const &that) {
    x = that.x;
    y = that.y;
  }
  Point &operator=(Point const &that) {
    x = that.x;
    y = that.y;
    return *this;
  }

  int getX() const { return x; }
  int getY() const { return y; }
  Point move(int offset_x, int offset_y) {
    return Point(x + offset_x, y + offset_y);
  }

private:
  int x, y;
};

bool operator==(Point const &left, Point const &right) {
  return left.getX() == right.getX() && left.getY() == right.getY();
}

bool operator!=(Point const &left, Point const &right) {
  return !(left == right);
}

namespace std {
template <> struct hash<Point> {
  size_t operator()(Point const &p) const {
    return hash<int>()(p.getX()) * 31 + hash<int>()(p.getY());
  }
};
}

TEST_CASE("immutable value object") {
  auto p1 = Point(100, 50);
  auto p2 = Point(100, 70);
  cout << (p1 == p2) << endl;
  cout << (p1 != p2.move(0, -20)) << endl;
  auto some_map = unordered_map<Point, int>();
  auto p3 = p1; // copy constructor
  p3 = p2;      // copy assignment
}
```

因为immutable value和原生的数值类型一样，没有资源所有权之类的问题。copy constructor 和 copy assignment 的语义都是ok的。