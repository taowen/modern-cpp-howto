#include <catch_ext.hpp>
#include <catch_with_main.hpp>
#include <codecvt>
#include <experimental/string_view>
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
