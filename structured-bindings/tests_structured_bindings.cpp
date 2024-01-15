#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>
#include <array>

namespace BeforeCpp17
{
    auto calc_stats(const std::vector<int>& data)
    {
        std::vector<int>::const_iterator min_pos, max_pos;
        std::tie(min_pos, max_pos) = std::minmax_element(data.begin(), data.end());

        double avg = std::accumulate(data.begin(), data.end(), 0.0) / data.size();

        return std::make_tuple(*min_pos, *max_pos, avg);
    }
}

template <typename TContainer>
auto calc_stats(const TContainer& data)
{
    auto [min_pos, max_pos] = std::minmax_element(std::begin(data), std::end(data));

    double avg = std::accumulate(std::begin(data), std::end(data), 0.0) / std::size(data); // since C++17 - std::size

    return std::tuple(*min_pos, *max_pos, avg);
}

TEST_CASE("Before C++17")
{
    std::vector<int> data = {4, 42, 665, 1, 123, 13};

    int min, max;
    //double avg;

    std::tie(min, max, std::ignore) = calc_stats(data);

    REQUIRE(min == 1);
    REQUIRE(max == Catch::Approx(665));
    //REQUIRE(avg == Catch::Approx(141.333));
}

TEST_CASE("Since C++17 - structured bindings")
{
    int data[] = {4, 42, 665, 1, 123, 13};

    auto [min, max, avg] = calc_stats(data);

    REQUIRE(min == 1);
    REQUIRE(max == Catch::Approx(665));
    REQUIRE(avg == Catch::Approx(141.333));
}

std::array<int, 3> get_pos()
{
    return {1, 2, 3};
}

struct ErrorCode
{
    int ec;
    const char* m;
};

ErrorCode open_file()
{
    return ErrorCode{13, "file not found!"};
}

TEST_CASE("structured bindings")
{
    SECTION("native arrays")
    {
        int pos[3] = {10, 20, 30};

        auto [x, y, z] = pos;

        x += 2;

        REQUIRE(x == 12);
        REQUIRE(y == 20);
        REQUIRE(z == 30);

        REQUIRE(pos[0] == 10);
    }

    SECTION("std::pair")
    {
        std::map<int, std::string> dict = {{1, "one"}, {2, "two"}};

        SECTION("before C++17")
        {
            std::map<int, std::string>::iterator pos;
            bool was_inserted;
            std::tie(pos, was_inserted) = dict.insert(std::make_pair(3, "three"));
        }

        SECTION("since C++17")
        {
            auto [pos, was_inserted] = dict.insert(std::pair(2, "TWO"));

            REQUIRE(was_inserted == false);
        }
    }

    SECTION("std::array")
    {
        auto [x, y, z] = get_pos();
        
        REQUIRE(x == 1);
        REQUIRE(y == 2);
        REQUIRE(z == 3);
    }

    SECTION("struct/class")
    {
        auto [error_code, error_message] = open_file();

        REQUIRE(error_code == 13);
        std::cout << error_message << "\n";
    }
}