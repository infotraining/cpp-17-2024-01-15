#include "gadget.hpp"

#include <algorithm>
#include <array>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

template <typename TContainer>
void print(const TContainer& container, string_view prefix)
{
    std::cout << prefix << ": [ ";
    for (const auto& item : container)
        std::cout << item << " ";
    std::cout << "]\n";
}

TEST_CASE("static inline")
{
    Gadget g1;
    REQUIRE(g1.id() == 1);

    Gadget g2;
    REQUIRE(g2.id() == 2);
}

struct Aggregate1
{
    int a, b;
    int coord[3];
    std::string name;

    void print() const
    {
        std::cout << "Aggregate1{" << a << ", " << b;
        std::cout << ", { ";
        for(const auto& item : coord)
            std::cout << item << " ";
        std::cout << "}, " << name << "}\n";
    }
};

struct Aggregate2 : Aggregate1, std::string
{
    std::vector<int> data;

    void print() const
    {
        Aggregate1::print();
        std::cout << "str: " << c_str() << "\n";
        ::print(data, "data");
    }
};

TEST_CASE("aggregates")
{
    Aggregate1 agg1{};
    agg1.print();

    Aggregate1 agg2{42, 665, {1}};
    agg2.print();

    static_assert(std::is_aggregate_v<decltype(agg2)>);

    Aggregate2 super_agg{{1, 2}, {"text"}, {1, 2, 3, 4}};
    super_agg.print();
    REQUIRE(super_agg.size() == 4);
    REQUIRE(super_agg.data == std::vector{1, 2, 3, 4});
}

enum class Coffee
{
    espresso = 1,
    cappucino [[deprecated]] = espresso
};

TEST_CASE("deprecated attribute")
{
    Coffee c = Coffee::espresso;
}