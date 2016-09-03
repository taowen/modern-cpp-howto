#include <catch_with_main.hpp>
#include <range/v3/all.hpp>
#include <string>
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

File openHostsFile() { return File("/etc/hosts"); }

TEST_CASE("use factory") {
  //  won't compile with copy constructor disabled
  auto hostsFile = openHostsFile();
  cout << hostsFile.readAll().size() << endl;
}

void printFileContent(File file) { cout << file.readAll().size() << endl; }

TEST_CASE("move ownership") {
  auto hostsFile = openHostsFile();
  printFileContent(std::move(hostsFile));
  // can not access hostsFile again, as the resource ownership has been moved
}

class MySocket {
public:
  constexpr MySocket() = default;
  ~MySocket() = default;

protected:
  MySocket(const MySocket &) = delete;            // copy constructor
  MySocket &operator=(const MySocket &) = delete; // assignment constructor
};

TEST_CASE("transient object") {
  MySocket sock1, sock2;
  // sock1 == sock2; 没有默认实现的 ==，无需禁用
  // sock1 != sock2; 没有默认实现的 !=，无需禁用
  // unordered_map<MySocket, int> some_map{}; 编译错误，MySocket没有实现hash
  // auto sock3 = sock1; copy constructor 已经被禁用
  // sock2 = sock1; assignment constructor 已经被禁用
}
