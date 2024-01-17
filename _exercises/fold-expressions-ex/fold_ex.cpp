#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////
template <typename TContainer, typename... TArgs>
auto matches(TContainer& v, TArgs&&... args)
{
    return (... + std::count(std::begin(v), std::end(v), args));
}


TEST_CASE("matches - returns how many items is stored in a container")
{
    // Tip: use std::count() algorithm

    vector<int> v{1, 2, 3, 4, 5};

    REQUIRE(matches(v, 2, 5) == 2);
    REQUIRE(matches(v, 100, 200) == 0);
    REQUIRE(matches("abccdef", 'x', 'y', 'z') == 0);
    REQUIRE(matches("abccdef", 'a', 'c', 'f') == 4);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

class Gadget
{
public:
    virtual std::string id() const { return "a"; }
    virtual ~Gadget() = default;
};

class SuperGadget : public Gadget
{
public:
    std::string id() const override
    {
        return "b";
    }
};

// TODO
TEST_CASE("Intro")
{
    std::vector vec = {1, 2, 3, 4};
    std::vector<std::unique_ptr<int>> ptrs;
    ptrs.push_back(std::make_unique<int>(13));
    ptrs.push_back(std::make_unique<int>(14));
}

template <typename... TArgs>
auto make_vector(TArgs&&... args)
{
    using TValue = std::common_type_t<TArgs...>;

    std::vector<TValue> vec;
    vec.reserve(sizeof...(args));

    (..., vec.push_back(std::forward<TArgs>(args)));

    return vec;
}

TEST_CASE("make_vector - create vector from a list of arguments")
{
    // Tip: use std::common_type_t<Ts...> trait
    SECTION("ints")
    {
        std::vector<int> v = make_vector(1, 2, 3);

        REQUIRE(v == vector{1, 2, 3});
    }

    SECTION("unique_ptrs")
    {
        const auto ptrs = make_vector(make_unique<int>(5), make_unique<int>(6));

        REQUIRE(ptrs.size() == 2);
    }

    SECTION("unique_ptrs with polymorphic hierarchy")
    {
        auto gadgets = make_vector(make_unique<Gadget>(), make_unique<SuperGadget>(), make_unique<Gadget>());

        static_assert(is_same_v<decltype(gadgets)::value_type, unique_ptr<Gadget>>);

        vector<string> ids;
        transform(begin(gadgets), end(gadgets), back_inserter(ids), [](auto& ptr)
            { return ptr->id(); });

        REQUIRE(ids == vector{"a"s, "b"s, "a"s});
    }
}

// /////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void hash_combine(size_t& seed, const T& value)
{
    seed ^= hash<T>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename... Args>
size_t combined_hash(const Args&... value)
{
    size_t seed = 0;
    (hash_combine(seed, value), ...);
    return seed;
}

TEST_CASE("combined_hash - write a function that calculates combined hash value for a given number of arguments")
{
    using namespace std::literals;

    REQUIRE(combined_hash(1U) == 2654435770U);
    REQUIRE(combined_hash(1, 3.14, "string"s) == 10365827363824479057U);
    REQUIRE(combined_hash(123L, "abc"sv, 234, 3.14f) == 162170636579575197U);
}

////////////////////////////////////////////////////

template <typename... TArgs>
size_t hash_for_tuple(const std::tuple<TArgs...>& tpl)
{
    auto hasher = [](const auto&... args) {
        return combined_hash(args...);
    };

    return std::apply(hasher, tpl);
}

struct Person
{
    int id;
    std::string fname;
    std::string lname;

    auto tied() const
    {
        return std::tie(id, fname, lname);
    }

    bool operator==(const Person& other) const
    {
        //return this.id == other.id && this.fname == other.fname && this->lname == other.lname;
        return tied() == other.tied();
    }

    bool operator<(const Person& other) const
    {
        return tied() < other.tied();
    }

    size_t hash() const
    {
        return hash_for_tuple(tied());
    }
};


TEST_CASE("operators == != & hash")
{
    Person p1{1, "Jan", "Kowalski"};
    Person p2{1, "Jan", "Kowalski"};
    REQUIRE(p1 == p2);

    Person p3{2, "Adam", "Kowalski"};
    REQUIRE(p1 < p3);

    std::cout << "Hash for p1: " << p1.hash() << "\n";
}