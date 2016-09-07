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

C++ 版本

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

## build string

Python 版本

```python
def test_build_string(self):
    # small
    self.assertEqual('hello world', 'hello' + ' world')
    # large
    parts = []
    parts.append('h')
    parts.append('e')
    parts.append('ll')
    parts.append('o')
    self.assertEqual('hello', ''.join(parts))
```

C++ 版本

```c++
TEST_CASE("build string") {
  // concat two
  auto str1 = string_view("hello");
  auto str2 = string_view(" world");
  CHECK("hello world" == (view::concat(str1, str2) | to_<string>()));

  // concat many
  auto str3 = string_view("!");
  auto str123 = vector<string_view>{str1, str2, str3};
  CHECK("hello world!" == (str123 | view::join | to_<string>()));

  // string itself is mutable
  // think every string is java StringBuilder
  auto mut_s = string();
  mut_s.reserve(str1.size() + str2.size() + str3.size());
  mut_s.append(str1);
  mut_s.append(str2);
  mut_s.append(str3);
  CHECK("hello world!" == mut_s);
}
```

## format

Python 版本

```python
def test_format(self):
    # positional
    self.assertEqual('hello world', '{} {}'.format('hello', 'world'))
    # named
    self.assertEqual('hello world', '{v1} {v2}'.format(v1='hello', v2='world'))
    # format
    self.assertEqual('3.14', '{:.2f}'.format(3.1415))
```

C++ 版本

```c++
TEST_CASE("format") {
  // positional
  using namespace fmt::literals;
  CHECK("hello world" == ("{} {}"_format("hello", "world")));
  // named
  CHECK("hello world" == ("{v1} {v2}"_format("v1"_a="hello", "v2"_a="world")));
  // format
  CHECK("3.14" == ("{:.2f}"_format(3.14159)));
}
```

## lower/upper

Python 版本

```python
def test_lower_upper(self):
    self.assertEqual('hello world', 'Hello World'.lower())
    self.assertEqual('HELLO WORLD', 'Hello World'.upper())
```
C++ 版本

```c++
string str_tolower(string_view input, locale const &l = std::locale()) {
  return input | view::transform([&l](auto c) { return std::tolower(c, l); }) |
         to_<string>();
}

string str_toupper(string_view input, locale const &l = std::locale()) {
  return input | view::transform([&l](auto c) { return std::toupper(c, l); }) |
         to_<string>();
}

TEST_CASE("lower/upper") {
  CHECK("hello world" == (str_tolower("Hello World")));
  CHECK("HELLO WORLD" == (str_toupper("Hello World")));
}
```

## startswith

Python 版本

```python
def test_startswith(self):
    self.assertTrue('Hello World'.startswith('He'))
```

C++ 版本

```c++
bool str_startswith(string_view haystack, string_view needle) {
  return ranges::equal(haystack | view::slice(size_t(0), needle.size()),
                       needle);
}

TEST_CASE("startswith") {
  CHECK(str_startswith("Hello World", "He"));
  CHECK(!str_startswith("Hello World", "Hello World!"));
}
```

## endswith

Python 版本

```python
def test_endswith(self):
    self.assertTrue("Hello World".endswith('ld'))
```

C++ 版本

```c++
bool str_endswith(string_view haystack, string_view needle) {
  return ranges::equal(
      haystack | view::slice(ranges::end - needle.size(), ranges::end), needle);
}

TEST_CASE("endswith") {
  CHECK(str_endswith("Hello World", "ld"));
  CHECK(!str_endswith("Hello World", "Hello World!"));
}
```

## strip

Python

```python
def test_strip(self):
    self.assertEqual('hello world', ' hello world'.lstrip())
    self.assertEqual('hello world', 'hello world '.rstrip())
    self.assertEqual('hello world', ' hello world '.strip())
```

C++

```c++
string_view lstrip(string_view input, locale const &l = std::locale()) {
  auto begin = input.begin();
  auto left_iterator = ranges::find_if(
      begin, input.end(), [&l](auto const &c) { return !std::isspace(c, l); });
  return input.substr(left_iterator - begin);
}

string_view rstrip(string_view input, locale const &l = std::locale()) {
  auto rend = input.rend();
  auto right_iterator =
      ranges::find_if(input.rbegin(), rend,
                      [&l](auto const &c) { return !std::isspace(c, l); });
  return input.substr(0, rend - right_iterator);
}

string_view strip(string_view input, locale l = std::locale()) {
  return rstrip(lstrip(input, l), l);
}

TEST_CASE("strip") {
  CHECK(string_view("hello") == lstrip(" hello"));
  CHECK(string_view("hello") == rstrip("hello "));
  CHECK(string_view("hello") == strip(" hello "));
}
```
