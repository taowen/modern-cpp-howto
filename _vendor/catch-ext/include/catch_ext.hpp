#include <string>
#include <unordered_map>

namespace Catch {

    using namespace std;

    string describe(const string& str) {
        return "\"" + str + "\"";
    }

    string describe(const char* str) {
        return "\"" + string(str) + "\"";
    }

    template<typename T>
    string describe(const T& obj) {
        return to_string(obj);
    }

    template<typename key_type, typename value_type> struct StringMaker<unordered_map<key_type, value_type>> {
        static std::string convert( unordered_map<key_type, value_type> const& value ) {
            auto desc = string{"{ "};
            for (const auto& e : value) {
                desc += "{ ";
                desc += describe(e.first);
                desc += ", ";
                desc += describe(e.second);
                desc += " } ";
            }
            desc += "}";
            return desc;
        }
    };

    template<typename key_type, typename value_type> struct StringMaker<pair<key_type, value_type>> {
        static std::string convert(pair<key_type, value_type> const &value) {
            return "{ " + describe(value.first) + ", " + describe(value.second) + " }";
        }
    };
}