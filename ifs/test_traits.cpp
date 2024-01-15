#include <catch2/catch_test_macros.hpp>
#include <iostream>

int foo(int x)
{
    return x * 2;
}

template <size_t N>
struct Foo
{
    static constexpr size_t value = N * 2;
};

template <typename T>
struct Identity
{
    using type = T;
};

TEST_CASE("type traits")
{
    REQUIRE(foo(8) == 16);
    static_assert(Foo<8>::value == 16);

    Identity<int>::type x = 42;
    static_assert(std::is_same_v<Identity<int>::type, int>);
}

////////////////////////////////////////
// IsVoid

template <typename T, T v>
struct IntegralConstant
{
    static constexpr T value = v;
};

static_assert(IntegralConstant<int, 5>::value == 5);

template <bool Value>
using BoolConstant = IntegralConstant<bool, Value>;

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;
static_assert(TrueType::value == true);
static_assert(FalseType::value == false);

template <typename T>
struct IsVoid : FalseType
{
    //static constexpr bool value = false;
};

template <>
struct IsVoid<void> : TrueType
{
    //static constexpr bool value = true;
};

// template  variable
template <typename T>
constexpr bool IsVoid_v = IsVoid<T>::value;

TEST_CASE("is_void")
{
    using TypeA = int;
    using TypeB = void;

    static_assert(IsVoid<TypeA>::value == false);
    static_assert(IsVoid<TypeB>::value == true);
    static_assert(std::is_void_v<TypeB> == true);
}

/////////////////////////////////////////////////////

template <typename T>
struct IsPointer : FalseType
{};

template <typename T>
struct IsPointer<T*> : TrueType
{};

// template  variable
template <typename T>
constexpr bool IsPointer_v = IsPointer<T>::value;

template <typename T>
void print_value(T v)
{
    if constexpr(IsPointer_v<T>)
    {
        assert(v != nullptr);
        std::cout << "value: " << *v << "\n";
    }
    else
    {
        std::cout << "value: " << v << "\n";
    }
}

TEST_CASE("is_pointer")
{
    using TypeA = int;
    using TypeB = int*;
    using TypeC = std::string*;

    static_assert(IsPointer_v<TypeA> == false);
    static_assert(IsPointer_v<TypeB> == true);
    static_assert(IsPointer_v<TypeC> == true);

    int x = 10;
    int* ptr_x = &x;

    print_value(x);
    print_value(ptr_x);
}