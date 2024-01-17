#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <charconv>
#include <optional>
#include <string_view>


// TODO

[[nodiscard]] std::optional<int> to_int(std::string_view str)
{
    int value;

    auto start = str.data();
    auto end = str.data() + str.size();

    if (const auto [pos_end, error_code] = std::from_chars(start, end, value); error_code != std::errc{} || pos_end != end)
    {
        return std::nullopt;
    }

    return value;
}

TEST_CASE("to_int returning optional")
{
    SECTION("happy path")
    {
        using namespace std::literals;

        SECTION("string to int")
        {
            auto result = to_int("123"s);

            REQUIRE(result.has_value());
            REQUIRE(*result == 123);
        }

        SECTION("const char* to int")
        {
            auto result = to_int("123");

            REQUIRE(result.has_value());
            REQUIRE(*result == 123);
        }
    }

    SECTION("sad path")
    {
        SECTION("whole string invalid")
        {
            auto result = to_int("a");

            REQUIRE_FALSE(result.has_value());
        }

        SECTION("part of string is invalid")
        {
            using namespace std::literals;

            auto result = to_int("123a4"sv);

            REQUIRE_FALSE(result.has_value());
        }
    }
}