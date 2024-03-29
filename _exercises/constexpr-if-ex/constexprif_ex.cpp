#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace std;

// https://en.cppreference.com/w/cpp/iterator/iterator_traits



template <typename Iterator>
auto advance_it(Iterator& it, size_t n)
{
    using IteratorCategoryTag = typename std::iterator_traits<Iterator>::iterator_category;

    if constexpr (std::is_same_v<IteratorCategoryTag, std::random_access_iterator_tag>)
    //if constexpr (std::random_access_iterator<Iterator>) // C++20
    {
        it += n;
        return std::random_access_iterator_tag{};
    }
    else
    {
        while (n-- > 0) 
        {
            ++it;
        };
        return std::input_iterator_tag{};
    }
}


TEST_CASE("constexpr-if with iterator categories")
{
    SECTION("random_access_iterator - it += n")
    {
        vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        auto it = data.begin();

        std::random_access_iterator_tag result = advance_it(it, 3);

        REQUIRE(*it == 4);
    }

    SECTION("input_iterator - n-times ++it")
    {
        list<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        auto it = data.begin();

        std::input_iterator_tag result = advance_it(it, 3);

        REQUIRE(*it == 4);
    }
}