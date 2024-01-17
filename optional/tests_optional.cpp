#include <algorithm>
#include <array>
#include <atomic>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <charconv>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

using namespace std;

TEST_CASE("optional")
{
    std::optional<int> opt_int;

    REQUIRE(opt_int.has_value() == false);
    REQUIRE(opt_int == std::nullopt);

    opt_int = 42;

    if (opt_int)
    {
        std::cout << "opt_int value: " << *opt_int << "\n";
    }
    else
    {
        std::cout << "opt_int is empty!!!";
    }

    SECTION("CTAD")
    {
        std::optional opt_name = "Jan"s;
    }

    SECTION("safe access")
    {
        std::optional opt_name = "Jan"s;
        REQUIRE(opt_name.value() == "Jan"s);

        opt_name = std::nullopt; // opt_name.reset(); 

        REQUIRE_THROWS_AS(opt_name.value(), std::bad_optional_access);
    }
}

std::optional<const char*> maybe_getenv(const char* n)
{
    if(const char* x = std::getenv(n))
        return x;
    else
        return std::nullopt;
}

struct Person
{
    std::string first_name;
    std::optional<std::string> middle_name;
    std::string last_name;
};

TEST_CASE("using optional")
{
    std::optional path = maybe_getenv("PATH");
    std::cout << "PATH: " << path.value_or("not found") << "\n";
}

TEST_CASE("weird stuff")
{
    std::optional<std::string> opt_name = "Jan";

    std::string target = std::move(*opt_name);
    opt_name.reset(); // for safety

    REQUIRE(opt_name.has_value() == false);

    SECTION("comparing")
    {
        std::optional opt_int = 42;
        REQUIRE(opt_int == 42);

        std::optional<bool> opt_flag = false;

        if (opt_flag)
        {
            std::cout << "opt_flag has state!\n";
        }

        if (opt_flag == false) // yields true
        {
            std::cout << "opt_flag has state that is false!\n";
        }

        std::optional<int*> opt_ptr1; // has no state
        std::optional<int*> opt_ptr2 = nullptr; // has state
        std::optional<int*> opt_ptr3 = std::nullopt; // has no state
    }
}