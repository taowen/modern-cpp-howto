#include <catch_ext.hpp>
#include <catch_with_main.hpp>
#include <codecvt>
#include <experimental/string_view>
#include <fmt/all.hpp>
#include <locale>
#include <range/v3/all.hpp>
#include <regex>
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
  CHECK("hello world" ==
        ("{v1} {v2}"_format("v1"_a = "hello", "v2"_a = "world")));
  // format
  CHECK("3.14" == ("{:.2f}"_format(3.14159)));
}

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

bool str_startswith(string_view haystack, string_view needle) {
  return ranges::equal(haystack | view::slice(size_t(0), needle.size()),
                       needle);
}

TEST_CASE("startswith") {
  CHECK(str_startswith("Hello World", "He"));
  CHECK(!str_startswith("Hello World", "Hello World!"));
}

bool str_endswith(string_view haystack, string_view needle) {
  return ranges::equal(
      haystack | view::slice(ranges::end - needle.size(), ranges::end), needle);
}

TEST_CASE("endswith") {
  CHECK(str_endswith("Hello World", "ld"));
  CHECK(!str_endswith("Hello World", "Hello World!"));
}

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
  CHECK(string_view("hello world") == lstrip(" hello world"));
  CHECK(string_view("hello world") == rstrip("hello world "));
  CHECK(string_view("hello world") == strip(" hello world "));
}

TEST_CASE("find") {
  auto str = string_view("hello");
  CHECK(2 == str.find("l"));
  CHECK(string::npos == str.find("!"));
  CHECK(3 == str.rfind("l"));
}

string str_replace(string_view haystack, string_view needle,
                   string_view replacement,
                   size_t count = std::numeric_limits<size_t>::max()) {
  auto needle_size = needle.size();
  auto replaced = string();
  replaced.reserve(haystack.size());
  auto pos = 0;
  while (count > 0) {
    auto next = haystack.find(needle, pos);
    if (next == string::npos) {
      replaced.append(haystack.substr(pos));
      return replaced;
    } else {
      replaced.append(haystack.substr(pos, next - pos));
      replaced.append(replacement);
      pos = next + needle_size;
      count -= 1;
    }
  }
  replaced.append(haystack.substr(pos));
  return replaced;
}

TEST_CASE("replace") {
  auto str = string_view("hello");
  CHECK("he__o" == str_replace(str, "l", "_"));
  CHECK("he_lo" == str_replace(str, "l", "_", 1));
  CHECK("he__o" == str_replace(str, "l", "_", -1));
}

TEST_CASE("regex search") {
  auto m = smatch();
  auto e = regex(R"!!(e\wl)!!", std::regex_constants::icase);
  auto s = string("HELLO");
  CHECK(regex_search(s, m, e));
  CHECK("ELL" == m[0]);
}

namespace std {

template <class BidirIt, class Traits, class CharT, class UnaryFunction>
std::basic_string<CharT>
regex_replace(BidirIt first, BidirIt last,
              const std::basic_regex<CharT, Traits> &re, UnaryFunction f) {
  std::basic_string<CharT> s;

  typename std::match_results<BidirIt>::difference_type positionOfLastMatch = 0;
  auto endOfLastMatch = first;

  auto callback = [&](const std::match_results<BidirIt> &match) {
    auto positionOfThisMatch = match.position(0);
    auto diff = positionOfThisMatch - positionOfLastMatch;

    auto startOfThisMatch = endOfLastMatch;
    std::advance(startOfThisMatch, diff);

    s.append(endOfLastMatch, startOfThisMatch);
    s.append(f(match));

    auto lengthOfMatch = match.length(0);

    positionOfLastMatch = positionOfThisMatch + lengthOfMatch;

    endOfLastMatch = startOfThisMatch;
    std::advance(endOfLastMatch, lengthOfMatch);
  };

  std::sregex_iterator begin(first, last, re), end;
  std::for_each(begin, end, callback);

  s.append(endOfLastMatch, last);

  return s;
}

template <class Traits, class CharT, class UnaryFunction>
std::string regex_replace(const std::string &s,
                          const std::basic_regex<CharT, Traits> &re,
                          UnaryFunction f) {
  return regex_replace(s.cbegin(), s.cend(), re, f);
}

} // namespace std

TEST_CASE("regex replace") {
  auto e = regex(R"!!([h|l]+)!!");
  auto s = string("hello");
  auto result = regex_replace(
      s, e, [](const std::smatch &m) { return str_toupper(m.str(0)); });
  CHECK("HeLLo" == result);
}

TEST_CASE("numeric conversion") {
  CHECK(42 == stoi("42"));
  CHECK(4.2f == stof("4.2"));
}
