#include <catch_with_main.hpp>
#include <experimental/optional>
#include <range/v3/all.hpp>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

using namespace std;
using namespace ranges;

void modify_single_value(int single) { single = 2; }

TEST_CASE("pass single value") {
  auto local_variable = 1;
  modify_single_value(local_variable);
  cout << local_variable << endl; // 1
}

void modify_single_reference(int &single) { single = 2; }

TEST_CASE("pass single reference") {
  auto local_variable = 1;
  modify_single_reference(local_variable);
  cout << local_variable << endl; // 1
}

void modify_single_pointer(int *single) { *single = 2; }

TEST_CASE("pass single pointer") {
  auto local_variable = 1;
  modify_single_pointer(&local_variable);
  cout << local_variable << endl; // 1
}

void modify_list_value(vector<int> list) { list[1] = 2; }

TEST_CASE("pass list value") {
  auto local_variable = vector<int>{1, 1};
  modify_list_value(local_variable);
  cout << (local_variable | view::all) << endl; // 1
}

class MyStruct {
public:
  int field;
};

void modify_struct_value(MyStruct struct_) { struct_.field = 2; }

TEST_CASE("pass struct value") {
  auto local_variable = MyStruct{1};
  modify_struct_value(local_variable);
  cout << local_variable.field << endl; // 1
}

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
    cout << "copied\n";
  }

  // move constructor
  Name(Name &&that)
      : firstName(std::move(that.firstName)),
        lastName(std::move(that.lastName)) {
    cout << "moved\n";
  }

  // copy assignment
  Name &operator=(Name const &that) {
    firstName = that.firstName;
    lastName = that.lastName;
    cout << "copy assigned\n";
    return *this;
  }

  // move assignment
  Name &operator=(Name &&that) {
    firstName = std::move(that.firstName);
    lastName = std::move(that.lastName);
    cout << "move assigned\n";
    return *this;
  }
};

void printFriendsByCopy(vector<Name> friends) {
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
  printFriendsByCopy(friends);
}

void printFriendsByReference(vector<Name> const &friends) {
  cout << "=== 3 ===" << endl;
  cout << "=== 4 ===" << endl;
  for (size_t i = 0; i < friends.size(); i++) {
    Name const &theName = friends[i];
    cout << theName.firstName << " " << theName.lastName << endl;
  }
  cout << "=== 5 ===" << endl;
}

TEST_CASE("without expensive copy") {
  cout << "=== 1 ===" << endl;
  auto friends = vector<Name>();
  friends.reserve(3);
  friends.emplace_back("Meliton", "Soso");
  friends.emplace_back("Bidzina", "Iona");
  friends.emplace_back("Archil", "Revazi");
  cout << "=== 2 ===" << endl;
  printFriendsByReference(friends);
}

auto giveTwoNames() {
  return make_pair(Name("Meliton", "Soso"), Name("Bidzina", "Iona"));
}

TEST_CASE("return tuple") {
  auto[name1, name2] = giveTwoNames();
  cout << name1.firstName << " " << name1.lastName << endl;
  cout << name2.firstName << " " << name2.lastName << endl;
}

auto giveTwoNamesWithoutMove() {
  return pair<Name, Name>(std::piecewise_construct,
                          std::forward_as_tuple("Meliton", "Soso"),
                          std::forward_as_tuple("Bidzina", "Iona"));
}

TEST_CASE("return tuple more efficiently") {
  auto[name1, name2] = giveTwoNamesWithoutMove();
  cout << name1.firstName << " " << name1.lastName << endl;
  cout << name2.firstName << " " << name2.lastName << endl;
}

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

TEST_CASE("read_file") {
  File hostsFile("/etc/hosts");
  cout << hostsFile.readAll().size() << endl;
}

auto openHostsFile() { return File("/etc/hosts"); }

TEST_CASE("use factory") {
  //  won't compile with copy constructor disabled
  auto hostsFile = openHostsFile();
  cout << hostsFile.readAll().size() << endl;
}

void printFileContent(File &&file) { cout << file.readAll().size() << endl; }

TEST_CASE("move ownership") {
  auto hostsFile = openHostsFile();
  printFileContent(std::move(hostsFile));
  // can not access hostsFile again, as the resource ownership has been moved
}

TEST_CASE("transient object") {
  File file1("/etc/hosts");
  File file2("/etc/hosts");
  // file1 == file2; 没有默认实现的 ==，无需禁用
  // file1 != file2; 没有默认实现的 !=，无需禁用
  // unordered_map<File, int> some_map{}; 编译错误，MySocket没有实现hash
  // auto file3 = file; copy constructor 已经被禁用
  // file2 = file1; assignment constructor 已经被禁用
  auto file3 = std::move(file1);
}

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
