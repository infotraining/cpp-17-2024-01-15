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

namespace BeforeCpp17
{
    template <typename T>
    auto is_power_of_2(T value) -> std::enable_if_t<std::is_integral_v<T>, bool>
    {
        return value > 0 && (value & (value - 1)) == 0;
    }

    template <typename T>
    auto is_power_of_2(T value) -> std::enable_if_t<std::is_floating_point_v<T>, bool>
    {
        int exponent;
        const T mantissa = std::frexp(value, &exponent);
        return mantissa == T(0.5);
    }
}

template <typename T>
bool is_power_of_2(T value)
{
    if constexpr(std::is_floating_point_v<T>)
    {
        int exponent;
        const T mantissa = std::frexp(value, &exponent);
        return mantissa == T(0.5);
    } 
    else 
    {
        return value > 0 && (value & (value - 1)) == 0;
    }
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

template <size_t Size>
auto create_buffer()
{
    if constexpr(Size < 512)
    {
        return std::array<int, Size>{};
    }
    else
    {
        return std::vector<int>(Size);
    }
}

TEST_CASE("auto + constexpr if")
{
    auto buffer_A = create_buffer<128>();   // std::array<int, 128>{}
    auto buffer_b = create_buffer<1024>();  // std::vector<int>(128)
}