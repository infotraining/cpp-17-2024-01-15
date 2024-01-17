#include <algorithm>
#include <any>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

struct Data
{
    int a, b;
};

TEST_CASE("any")
{
    std::any anything;

    REQUIRE(anything.has_value() == false);

    anything = 42;
    anything = 3.14;
    anything = "text"s;
    anything = Data{1, 2};
    anything = std::vector{1, 2, 3};

    SECTION("access to copy")
    {
        auto vec = std::any_cast<std::vector<int>>(anything);
        REQUIRE(vec == std::vector{1, 2, 3});

        REQUIRE_THROWS_AS(std::any_cast<Data>(anything), std::bad_any_cast);
    }

    SECTION("access to original object")
    {
        if (auto* vec_ptr = std::any_cast<std::vector<int>>(&anything); vec_ptr)
        {
            REQUIRE(*vec_ptr == std::vector{1, 2, 3});
        }
        else
        {
            REQUIRE(vec_ptr == nullptr);
        }
    }

    SECTION("type")
    {
        anything = Data{42, 665};

        const std::type_info& type_desc = anything.type();
        std::cout << "anything stores " << type_desc.name() << "\n";
    }
}


TEST_CASE("dynamic map")
{
    DynamicMap dm;

    dm.insert("age", 42);
    dm.insert("name", "Jan"s);
    dm.insert("salary", 10'000.99);

    REQUIRE(dm.get<int>("age") == 42);
    REQUIRE(dm.get<std::string>("name") == "Jan");
    REQUIRE_THROWS_AS(dm.get<int>("name"), std::bad_any_cast);
    REQUIRE(dm.get<double>("salary") == 10'000.99);
    
}