#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <numeric>
#include <algorithm>
#include <execution>


TEST_CASE("parallel-stl")
{
    std::vector<int> raw_data(1'000'000);
    std::iota(raw_data.begin(), raw_data.end(), 0);

    std::random_device rd;
    std::mt19937 rnd_gen(rd());
    std::shuffle(raw_data.begin(), raw_data.end(), rnd_gen);

    std::sort(std::execution::par_unseq, raw_data.begin(), raw_data.end());
    REQUIRE(std::is_sorted(raw_data.begin(), raw_data.end()));
}