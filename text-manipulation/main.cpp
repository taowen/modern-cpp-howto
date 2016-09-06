#include <catch_ext.hpp>
#include <catch_with_main.hpp>
#include <codecvt>
#include <experimental/string_view>
#include <fmt/all.hpp>
#include <locale>
#include <range/v3/all.hpp>
#include <string>

const int abc = std::__1::numeric_limits<int>::digits;

using namespace std;
using namespace ranges;

TEST_CASE("string view substr") {
  auto str1 = string_view("hello world");
  CHECK("o w" == (str1.substr(4, 3)));
}

TEST_CASE("string substr") {
  auto str1 = string("hello world");
  CHECK("o w" == (str1.substr(4, 3)));
}

TEST_CASE("multiline") {
  auto str1 = string_view(R"!!(abc
def)!!");
  CHECK("abc\ndef" == str1);
}

TEST_CASE("chinese") {
  auto str1 = u16string_view(u"中文");
  CHECK(u"文" == (str1.substr(1)));
  CHECK(2 == (str1.size()));
  CHECK(6 == (string_view("中文").size()));
}

TEST_CASE("to utf8") {
  wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> convert;
  auto str2 = convert.to_bytes(u"中文");
  CHECK("中文" == str2);
}

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

TEST_CASE("format") {
  // positional
  using namespace fmt::literals;
  CHECK("hello world" == ("{} {}"_format("hello", "world")));
  // named
  CHECK("hello world" == ("{v1} {v2}"_format("v1"_a="hello", "v2"_a="world")));
  // format
  CHECK("3.14" == ("{:.2f}"_format(3.14159)));
}
