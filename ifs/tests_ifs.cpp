#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <map>
#include <mutex>
#include <numeric>
#include <queue>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

TEST_CASE("if with initializer")
{
    std::vector vec = {1, 2, 543, 235, 534, 665, 654, 3456};

    if (auto pos = std::find(begin(vec), end(vec), 665); pos != end(vec))
    {
        std::cout << "Found: " << *pos << "\n";
    }
    else
    {
        std::cout << "Item not found!\n";
        assert(pos == end(vec));
    }
}

TEST_CASE("if with initializers & structured bindings")
{
    std::map<int, std::string> dict = {{1, "one"}};

    if (auto [pos, was_inserted] = dict.emplace(2, "two"); was_inserted)
    {
        const auto& [key, value] = *pos;
        std::cout << "Item: " << key << " - " << value << " was inserted\n";
    }
    else
    {
        std::cout << "Insert was not executed...\n";
    }
}

TEST_CASE("ifs with mutex")
{
    std::queue<std::string> qmsg;
    std::mutex mtx_qmsg;

    SECTION("thread#1")
    {
        std::lock_guard lk{mtx_qmsg};
        qmsg.push("START");
    }

    SECTION("thread#2")
    {
        std::string msg;

        if (std::lock_guard lk{mtx_qmsg}; !qmsg.empty())
        {
            msg = qmsg.front();
            qmsg.pop();
        }

        std::cout << msg << "\n";
    }
}

//////////////////////////////////////////////////////////////////////////////////
// constexpr if

template <typename T>
bool is_power_of_2(T value)
{
    return value > 0 && (value & (value - 1)) == 0;
}

bool is_power_of_2(double value)
{
    int exponent;
    const double mantissa = std::frexp(value, &exponent);
    return mantissa == double(0.5);
}

bool is_power_of_2(float value)
{
    int exponent;
    const float mantissa = std::frexp(value, &exponent);
    return mantissa == float(0.5);
}

TEST_CASE("constexpr if")
{
    REQUIRE(is_power_of_2(8));
    REQUIRE(is_power_of_2(16));
    REQUIRE(is_power_of_2(128));
    REQUIRE(is_power_of_2(129) == false);

    REQUIRE(is_power_of_2(8.0));
    REQUIRE(is_power_of_2(8.0f));
}