#include <string>
#include <unordered_map>
#include <range/v3/all.hpp>

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

namespace mcht {
    using namespace std;
    using namespace ranges;
    template<typename E>
    struct as_vector : pipeable<as_vector<E>>
    {
    private:

        /// If it's a view already, pass it though.
        template<typename T>
        static vector<E> from_range(T && t, concepts::View*)
        {
            return t;
        }

    public:
        template<typename T,
            CONCEPT_REQUIRES_(Range<T>())>
        auto operator()(T && t) const ->
            decltype(as_vector::from_range(std::forward<T>(t), view_concept<T>()))
        {
            return as_vector::from_range(std::forward<T>(t), view_concept<T>());
        }

        template<typename T,
            CONCEPT_REQUIRES_(Range<T &>())>
        ranges::reference_wrapper<T> operator()(std::reference_wrapper<T> ref) const
        {
            return ranges::ref(ref.get());
        }
    };
}
