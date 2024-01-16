#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Gadget
{
    std::string id;
    int usage_counter = 0;

    explicit Gadget(std::string id)
        : id{std::move(id)}
    {
        std::cout << "Gadget(" << this->id << " - " << this << ")\n";
    }

    Gadget(const Gadget& source)
        : id{source.id}
    {
        std::cout << "Gadget(cc: " << this->id << " - " << this << ")\n";
    }

    Gadget(Gadget&& source) noexcept
        : id{move(source.id)}
    {
        std::cout << "Gadget(mv: " << this->id << " - " << this << ")\n";
    }

    ~Gadget()
    {
        std::cout << "~Gadget(" << this->id << " - " << this << ")\n";
    }

    void use()
    {
        ++usage_counter;

        auto printer = [this]() { std::cout << "Using " << id << "\n"; };
        printer();
    }

    auto get_reporter()
    {
        return [*this] { std::cout << "Report from Gadget(" << id << ")\n"; };
    }
};

TEST_CASE("capturing [*this]")
{
    std::function<void()> reporter;

    {
        Gadget g{"ipad"};
        reporter = g.get_reporter();
    }
    
    reporter();
}

/////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("lambda as constexpr")
{
    auto square = [](int x) { return x * x; };

    int data[square(32)] = {};

    static_assert(std::size(data) == square(32));
}

template <size_t N>
constexpr auto create_array()
{
    auto square = [](int x) { return x * x; };

    std::array<int, N> data = { square(0), square(1), square(2)};

    for(size_t i = 3; i < N; ++i)
    {
        data[i] = square(i);
    }

    return data;
}

namespace Cpp20
{
    template <typename Iter, typename Predicate>
    constexpr Iter find_if(Iter first, Iter last, Predicate pred)
    {
        bool is_found = false;
        Iter it = first;
        for(; it != last; ++it)
        {
            if (pred(*it))
            {
                is_found = true;
                break;
            }
        }

        return it;
    }
}

TEST_CASE("constexpr in C++17")
{
    constexpr auto lookup_table = create_array<100>();

    constexpr int lt_1000 = *Cpp20::find_if(lookup_table.begin(), lookup_table.end(), [](int x) { return x > 1000; });

    static_assert(lt_1000 == 1024);
}