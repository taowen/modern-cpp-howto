# 文本处理

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
```
C++ 版本

```c++
TEST_CASE("chinese") {
  auto str1 = u16string_view(u"中文");
  CHECK(u"文" == (str1.substr(1)));
}
```

### to utf8

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