#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

template <typename... Ts>
struct Record
{
    std::tuple<Ts...> values;
};

template <typename TFunction, typename... TArgs>
auto call(TFunction f, TArgs&&... args)
{
    return f(std::forward<TArgs>(args)...);
}

int foo(int x)
{
    return x;
}

int bar(int x, int y)
{
    return x * y;
}

////////////////////////////////////////////////////////
// Head-Tail idiom

namespace BeforeCpp17
{
    void print()
    {
        std::cout << "\n";
    }

    template <typename THead, typename... TTail>
    void print(THead head, TTail... tail)
    {
        std::cout << head << " ";
        print(tail...);
    }
}

namespace HeadTail
{
    template <typename THead, typename... TTail>
    void print(const THead& head, const TTail&... tail)
    {
        std::cout << head << " ";

        if constexpr (sizeof...(tail) > 0)
            print(tail...);
        else
            std::cout << "\n";
    }

    template <typename THead, typename... TTail>
    auto sum(THead head, TTail... tail)
    {
        if constexpr (sizeof...(tail) > 0)
            return head + sum(tail...);
        else
            return head;
    }
}

template <typename... TArgs>
auto sum(const TArgs&... args) // sum(1, 2, 3, 4, 5)
{
    return (... + args); // left-fold: return ((((1 + 2) + 3) + 4) + 5);
}

template <typename... TArgs>
void print(const TArgs&... args)
{
    auto with_space = [is_first = true](const auto& item) mutable
    {
        if (!is_first)
            std::cout << " ";
        is_first = false;
        return item;
    };

    (std::cout << ... << with_space(args)) << "\n";
}

template <typename... TArgs>
void print_lines(const TArgs&... args)
{
    (..., (std::cout << args << "\n"));
}

template <typename... TArgs>
void print_separated(const TArgs&... args)
{
    (..., (std::cout << args << " ")) << "\n";
}

TEST_CASE("variadic templates")
{
    Record<int, double, std::string> r1;
    Record<> r2;

    REQUIRE(call(foo, 42) == 42);
    REQUIRE(call(bar, 67, 10) == 670);

    print(1, 3.14, "text");

    REQUIRE(sum(1, 2, 3, 4, 5) == 15);
    REQUIRE(sum(1, 6) == 7);

    std::vector vec = {1, 2, 3, 4, 5};
    REQUIRE(std::accumulate(vec.begin(), vec.end(), 0, std::plus{}) == 15);

    print_lines(1, 3.14, "text");
}

template <typename TFunction, typename... TArgs>
void apply_function(TFunction f, TArgs&&... args)
{
    (..., f(std::forward<TArgs>(args)));
}

TEST_CASE("apply function")
{
    apply_function([](const auto& item) { print(item); }, 1, 2, 3, 4, "text", 3.14);
}