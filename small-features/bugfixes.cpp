#include <array>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <span>
#include <iomanip>
#include <numeric>

enum class Coffee : uint8_t
{
    espresso,
    cappucino,
    flat_white
};

TEST_CASE("enums - direct init")
{
    Coffee coffee = Coffee::espresso;
    auto index = static_cast<std::underlying_type_t<Coffee>>(coffee);

    Coffee other_coffee{1}; // direct init - since C++17 OK
}

void print(float const x, std::span<const std::byte> const bytes)
{
    std::cout << std::setprecision(6) << std::setw(8) << x << " = { "
              << std::hex << std::uppercase << std::setfill('0');

    for (auto const b : bytes)
        std::cout << std::setw(2) << std::to_integer<int>(b) << ' ';
    std::cout << std::dec << "}\n";
}

TEST_CASE("C++ & byte")
{
    unsigned char legacy_byte = 65;

    legacy_byte += 42;

    std::cout << legacy_byte << "\n";

    SECTION("std::byte - since C++17")
    {
        std::byte b1{65};
        std::byte b2{0b01010101};

        auto result = (b1 & b2) << 2;

        std::cout << "result in bytes: " << std::to_integer<int>(result) << "\n";

        std::array<std::byte, 1024> buffer{};
    }

    SECTION("std::as_bytes - C++20")
    {
        float data[1]{3.141592f};

        auto const const_bytes = std::as_bytes(std::span{data});

        print(data[0], const_bytes);

        auto const writable_bytes = std::as_writable_bytes(std::span{data});

        // Change the sign bit that is the MSB (IEEE 754 Floating-Point Standard).
        writable_bytes[3] |= std::byte{0B1000'0000};

        print(data[0], const_bytes);
    }
}

TEST_CASE("auto + {}")
{
    int x1 = 42;
    int x2(42);
    int x3{42};

    auto ax1 = 42; // int
    auto ax2(42);  // int
    auto ax3{42};  // std::initializer_list<int> -> int - since C++17
    auto lst = {1, 2, 3}; // std::initializer_list<int>
}

void foo()
{
    std::cout << "foo()\n";
}

void bar() noexcept
{
    std::cout << "bar() noexcept\n";
}

namespace BeforeCpp17
{
    template <typename F>
    void call(F a, F b)
    {
        a();
        b();
    }
}

namespace SinceCpp17
{
    template <typename F1, typename F2>
    void call(F1 a, F2 b)
    {
        a();
        b();
    }
}

TEST_CASE("noexcept as type specifier")
{
    static_assert(!std::is_same_v<decltype(foo), decltype(bar)>);

    //BeforeCpp17::call(foo, bar);
    SinceCpp17::call(foo, bar);

    void (*ptr_fun)() noexcept = nullptr;
    ptr_fun = bar;
    // ptr_fun = foo;
}

std::vector<int> create_vector_rvo(size_t n)
{
    return std::vector<int>(n, -1);  // prvalue
}

std::vector<int> create_vector_nrvo(size_t n)
{
    std::vector<int> vec(n);
    std::iota(begin(vec), end(vec), 0);
    
    return vec; // lvalue
}

TEST_CASE("rvo vs. nrvo")
{
    std::vector<int> data = create_vector_rvo(1'000'000);
}

struct CopyMoveDisabled
{
    std::vector<int> data;

    CopyMoveDisabled(std::initializer_list<int> lst)
        : data {lst}
    {
    }

    CopyMoveDisabled(const CopyMoveDisabled&) = delete;
    CopyMoveDisabled& operator=(const CopyMoveDisabled&) = delete;
    CopyMoveDisabled(CopyMoveDisabled&&) = delete;
    CopyMoveDisabled& operator=(CopyMoveDisabled&&) = delete;
    ~CopyMoveDisabled() = default;
};

CopyMoveDisabled create_impossible()
{
    return CopyMoveDisabled{1, 2, 3}; // prvalue
}

void use_impossible(CopyMoveDisabled arg)
{
    std::cout << "Using object: ";
    for(const auto& item : arg.data)
        std::cout << item << " ";
    std::cout << "\n";
}

TEST_CASE("returning & passing CopyMoveDisabled")
{
    CopyMoveDisabled cmd = create_impossible(); // rvo

    REQUIRE(cmd.data == std::vector{1, 2, 3});

    use_impossible(create_impossible());
}