# 文本处理

参考资料：http://www.tutorialspoint.com/python/python_strings.htm

## substring

Python 版本

```python
def test_substr(self):
    self.assertEqual('o w', 'hello world'[4:7])
```

C++ 版本

```c++
TEST_CASE("string view substr") {
  auto str1 = string_view("hello world");
  CHECK("o w" == (str1.substr(4, 3)));
}

TEST_CASE("string substr") {
  auto str1 = string("hello world");
  CHECK("o w" == (str1.substr(4, 3)));
}
```

## multiline

Python 版本

```python
def test_multiline(self):
    self.assertEqual('abc\ndef', """abc
def""")
```
C++ 版本

```c++
TEST_CASE("multiline") {
    auto str1 = string_view(R"!!(abc
def)!!");
    CHECK("abc\ndef" == str1);
}
```

## chinese

Python 版本

```python
def test_chinese(self):
    self.assertEqual(u'文', u'中文'[1])
    self.assertEqual(2, len(u'中文'))
    self.assertEqual(6, len('中文'))
```
C++ 版本

```c++
TEST_CASE("chinese") {
  auto str1 = u16string_view(u"中文");
  CHECK(u"文" == (str1.substr(1)));
  CHECK(2 == (str1.size()));
  CHECK(6 == (string_view("中文").size()));
}
```

## to utf8

Python 版本

```python
def test_utf8(self):
    self.assertEqual('中文', u'中文'.encode('utf8'))
```

C++ 版本

```c++
TEST_CASE("to utf8") {
  wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> convert;
  auto str2 = convert.to_bytes(u"中文");
  CHECK("中文" == str2);
}
```

## split

Python 版本

```python
def test_split(self):
    self.assertListEqual(['hello', 'world'], 'hello world'.split(' '))
```

C++ 

```c++
auto split(string_view input, string_view delimeter) {
  auto to_string_view = [](auto const &r) {
    return string_view(&*r.begin(), ranges::distance(r));
  };
  return view::split(input, delimeter) | view::transform(to_string_view);
}

TEST_CASE("split") {
  auto parts = split("hello world", " ") | to_vector;
  CHECK("hello" == parts[0]);
  CHECK("world" == parts[1]);
}
```