#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

void foo(int)
{ }

template <typename T>
void deduce1(T arg)
{
    puts(__PRETTY_FUNCTION__);
}

template <typename T>
void deduce2(T& arg)
{
    puts(__PRETTY_FUNCTION__);
}

template <typename T>
void deduce3(T&& arg)
{
    puts(__PRETTY_FUNCTION__);
}

TEST_CASE("Template Argument Deduction - case 1")
{
    int x = 10;
    const int cx = 10;
    int& ref_x = x;
    const int& cref_x = cx;
    int tab[10];

    auto ax1 = x;      // int
    deduce1(x);        // deduce<int>(int arg)
    auto ax2 = cx;     // int
    deduce1(cx);       // deduce<int>(int arg)
    auto ax3 = ref_x;  // int
    deduce1(ref_x);    // deduce<int>(int arg)
    auto ax4 = cref_x; // int
    deduce1(cref_x);   // deduce<int>(int arg) 
    auto ax5 = tab;    // int* -> array decays into pointer
    deduce1(tab);      // deduce<int*>(int* arg)    
    auto ax6 = foo;    // void(*ax6)(int)
    deduce1(foo);      // deduce<void(*)(int)>(void(*arg)())
}

TEST_CASE("Template Argument Deduction - case 2")
{
    int x = 10;
    const int cx = 10;
    int& ref_x = x;
    const int& cref_x = cx;
    int tab[10];

    auto& ax1 = x;      // int&
    deduce2(x);         // deduce<int>(int&)
    auto& ax2 = cx;     // const int&
    deduce2(cx);        // deduce<const int>(const int&)
    auto& ax3 = ref_x;  // int&
    deduce2(ref_x);     // deduce<int>(int&)
    auto& ax4 = cref_x; // const int&
    deduce2(cref_x);    // deduce<const int>(const int&)
    auto& ax5 = tab;    // int(&ax5)[10] - ref to array
    deduce2(tab);       // deduce<int[10]>(int(&arg)[10])
    auto& ax6 = foo;    // void(&ax6)(int)
    deduce2(foo);       // deduce<void(int)>(void(&arg)(int))
}

TEST_CASE("Template Argument Deduction - case 3")
{
    int x = 10;
    const int cx = 10;

    auto&& ax1 = x;  // int&
    deduce3(x);      // deduce<int&>(int& && -> int&)
    auto&& ax2 = cx; //  const int&
    deduce3(cx);     // deduce<const int&>(const int& && -> const int&)

    auto&& ax3 = 10; // int&&
    deduce3(10);     // deduce<int>(int&&)
}

namespace Explain
{
    template <typename T>
    size_t size(const T& container)
    {
        return container.size();
    }

    template <typename T, size_t N>
    size_t size(T(&container)[N])
    {
        return N;
    }
}

TEST_CASE("std::size - explain")
{
    std::vector vec = {1, 2, 3};
    REQUIRE(Explain::size(vec) == 3);

    int tab[10];
    REQUIRE(Explain::size(tab) == 10);
}