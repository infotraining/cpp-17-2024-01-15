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

// TODO

// TEST_CASE("matches - returns how many items is stored in a container")
// {
//     // Tip: use std::count() algorithm

//     vector<int> v{1, 2, 3, 4, 5};

//     REQUIRE(matches(v, 2, 5) == 2);
//     REQUIRE(matches(v, 100, 200) == 0);
//     REQUIRE(matches("abccdef", 'x', 'y', 'z') == 0);
//     REQUIRE(matches("abccdef", 'a', 'c', 'f') == 4);
// }

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

// TEST_CASE("make_vector - create vector from a list of arguments")
// {
//     // Tip: use std::common_type_t<Ts...> trait
//     SECTION("ints")
//     {
//         std::vector<int> v = make_vector(1, 2, 3);
//         //std::vector v = {1, 2, 3};

//         REQUIRE(v == vector{1, 2, 3});
//     }

//     SECTION("unique_ptrs")
//     {
//         const auto ptrs = make_vector(make_unique<int>(5), make_unique<int>(6));

//         REQUIRE(ptrs.size() == 2);
//     }

//     SECTION("unique_ptrs with polymorphic hierarchy")
//     {
//         auto gadgets = make_vector(make_unique<Gadget>(), make_unique<SuperGadget>(), make_unique<Gadget>());

//         static_assert(is_same_v<decltype(gadgets)::value_type, unique_ptr<Gadget>>);

//         vector<string> ids;
//         transform(begin(gadgets), end(gadgets), back_inserter(ids), [](auto& ptr)
//             { return ptr->id(); });

//         REQUIRE(ids == vector{"a"s, "b"s, "a"s});
//     }
// }

// /////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void hash_combine(size_t& seed, const T& value)
{
    seed ^= hash<T>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

// TODO

// TEST_CASE("combined_hash - write a function that calculates combined hash value for a given number of arguments")
// {
//     REQUIRE(combined_hash(1U) == 2654435770U);
//     REQUIRE(combined_hash(1, 3.14, "string"s) == 10365827363824479057U);
//     REQUIRE(combined_hash(123L, "abc"sv, 234, 3.14f) == 162170636579575197U);
// }