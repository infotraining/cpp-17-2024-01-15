#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <variant>
#include <vector>

using namespace std;

struct NoDefault
{
    int value;

    NoDefault(int v) : value{v}
    {}
};

TEST_CASE("variant")
{
    std::variant<int, double, std::string> var1;

    REQUIRE(std::holds_alternative<int>(var1));
    REQUIRE(std::get<int>(var1) == 0);

    var1 = 42;
    var1 = 3.14;
    var1 = "text"s;

    auto& data = std::get<std::string>(var1);
    REQUIRE(data == "text"s);

    REQUIRE_THROWS_AS(std::get<double>(var1), std::bad_variant_access);

    std::string* ptr_data = std::get_if<std::string>(&var1);

    if (ptr_data)
    {
        std::cout << "data: " << *ptr_data << "\n";
    }
    else
    {
        std::cout << "data holds diffent type\n";
    }
}

TEST_CASE("monostate")
{
    std::variant<std::monostate, NoDefault, int> var;

    REQUIRE(std::holds_alternative<std::monostate>(var));
}

/////////////////////////////////////////////////////////////////////////////////

// Visitor
struct Printer
{
    void operator()(int v) const
    {
        std::cout << "int: " << v << "\n";
    }

    void operator()(const std::string& s) const
    {
        std::cout << "string: " << s << "\n";
    }

    void operator()(const std::vector<int>& v) const
    {
        std::cout << "vec: { ";
        for(const auto& item : v)
            std::cout << item << " ";
        std::cout << "}\n";
    }
};

struct Lambda_7234762567
{
    auto operator()(int x) const { return x * x; }
};

//////////////////////////////////////
// overload
template <typename... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};

// deduction guide
template <typename... Ts>
overload(Ts...) -> overload<Ts...>;

TEST_CASE("visiting variants")
{
    std::variant<int, std::string, std::vector<int>> var;
    var = 42;
    var = "text"s;
    var = std::vector{1, 2, 3};

    std::visit(Printer{}, var);

    auto printer = overload{
        [](int v) { std::cout << "int: " << v << "\n"; },
        [](const std::string& s) { std::cout << "string: " << s << "\n"; },
        [](const std::vector<int>& v) {
            std::cout << "vec: { ";
            for(const auto& item : v)
                std::cout << item << " ";
            std::cout << "}\n";
        }
    };

    std::visit(printer, var);

    size_t result = std::visit(overload{
        [](int v) -> size_t { return v; },
        [](const std::string& s) { return s.size(); },
        [](const std::vector<int>& v) { return v.size(); }
    }, var);
}