#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <tuple>
#include <optional>
#include <memory>
#include <functional>
#include <list>
#include <map>
#include <array>

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

/////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename T2>
struct ValuePair
{
    T1 fst;
    T2 snd;

    template <typename U1, typename U2>
    ValuePair(U1&& f, U2&& s) : fst{std::forward<U1>(f)}, snd{std::forward<U2>(s)}
    {}
};

// deduction guide
template <typename T1, typename T2>
ValuePair(T1, T2) -> ValuePair<T1, T2>;

ValuePair(const char*, const char*) -> ValuePair<std::string, std::string>;

TEST_CASE("CTAD")
{
    ValuePair<int, double> vp1{42, 3.14};

    ValuePair vp3{42, 3.14};

    const int cx = 42;
    ValuePair vp4{"forty-two", cx}; // ValuePair<const char*, int>
 
    std::string text = "text";
    ValuePair vp5("text"s, text); // ValuePair<std::string, std::string>

    ValuePair vp6{"abc", "def"};    // ValuePair<std::string, std::string> - deduction guide 129
}

TEST_CASE("CTAD - special case")
{
    std::vector v = { 1, 2, 3 };      // std::vector<int>
    std::vector v1{1, 2, 3};          // std::vector<int>

    std::vector be_carefull1{ v };    // std::vector<int>
    std::vector be_carefull2{ v, v }; // std::vector<std::vector<int>>

    std::vector<std::vector<int>> no_ctad{v}; // no ctad deduction - param type passed explicitly
}

//////////////////////////////////////////////////

template <typename T>
struct Data
{   
    T value;
};

template <typename T> // since C++20 - obsolete
Data(T) -> Data<T>;

TEST_CASE("CTAD + aggregates")
{
    Data d1{42};
}

//////////////////////////////////////////////////
// CTAD & standard library

TEST_CASE("CTAD & standard library")
{
    SECTION("std::pair")
    {
        std::pair p1{42, 3.14}; // std::pair<int, double>
        std::pair p2{"text", "abc"s}; // std::pair<const char*, std::string>
    }

    SECTION("std::tuple")
    {
        std::tuple t1{1, 3.14, "text"s};  // std::tuple<int, double, std::string>
        std::tuple t2{t1};                // std::tuple<int, double, std::string>

        std::pair p{1, 3.14};
        std::tuple t3{p}; // std::tuple<int, double>
    }

    SECTION("std::optional")
    {
        std::optional<int> opt_int;
        REQUIRE(opt_int.has_value() == false);

        opt_int = 42;
        REQUIRE(opt_int.has_value() == true);
        REQUIRE(*opt_int == 42);

        std::optional opt1{42};    // optional<int>
        std::optional opt2 = 42;   // optional<int>

        std::optional opt3{opt1};  // optional<int>
    }

    SECTION("smart pointers")
    {
        auto uptr = std::make_unique<int>(13);
        auto sptr = std::make_shared<int>(42);

        std::shared_ptr other_sptr = std::move(uptr);
        std::weak_ptr wptr = sptr;
    }

    SECTION("std::function")
    {
        std::function f = [](int x) { return x * x; }; // std::function<int(int)>
    }

    SECTION("std containers")
    {
        std::vector vec = {1, 2, 3}; // std::vector<int>
        std::list lst = {5, 6, 7};

        std::vector data(lst.begin(), lst.end()); // std::vector<int>

        std::array arr = {1, 2, 3, 4, 5, 6, 7, 8};
    }
}
