# value 还是 reference

java 程序员是幸福的，在 java 的世界里当设计一个 class，实例化一个 object 时需要考虑的东西远比 c++ 要小，所谓“心智负担”。作为一个 python
程序员一直搞不懂什么时候使用什么 c++ 的语法特性。这个不仅仅是垃圾收集的问题，还要考虑到拷贝构造的成本等很多东西。我们把这些“心智负担”做一个
不完整的列表：

* 被建模的概念自身的语义：value v.s entity v.s transient
* 对象被使用方式：by-value v.s reference v.s pointer
* 物理存在：compile-time v.s stack v.s heap v.s thread-local
* 堆上对象的拥有方式: unique_ptr v.s shared_ptr v.s container
* 是否只读: const v.s non-const

当我们需要在代码里对一个真实世界的概念进行建模的时候，需要考虑如此多的方面，难怪c++比java难用那么多。

## 语义

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