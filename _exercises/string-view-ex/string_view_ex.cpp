#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

template <typename Container>
void print_all(const Container& container, std::string_view prefix)
{
    cout << prefix << ": [ ";
    for (const auto& item : container)
        cout << item << " ";
    cout << "]\n";
}

std::vector<std::string_view> split_text(std::string_view text, std::string_view pattern = ", ")
{
    std::vector<std::string_view> result{};

    size_t start{};
    size_t end{};

    while (end != std::string_view::npos)
    {
        end = text.find_first_of(pattern, start);
        std::string_view token = text.substr(start, end - start);
        result.push_back(token);
        start = text.find_first_not_of(pattern, end);
    }

    return result;
}

namespace Cpp20
{
    std::vector<std::string_view> split_text(std::string_view text, std::string_view pattern = " ")
    {
        std::vector<std::string_view> result{};

        auto tokens = std::views::split(text, pattern);

        for (auto&& token : tokens)
        {
            std::string_view token_sv{token.begin(), token.size()};
            result.push_back(token_sv);
        }

        return result;
    }
}

TEST_CASE("split with spaces")
{
    const char* text = "one two three four";

    std::vector<std::string_view> words = Cpp20::split_text(text);

    print_all(words, "words");

    auto expected = {"one", "two", "three", "four"};

    REQUIRE(equal(begin(expected), end(expected), begin(words)));
}

TEST_CASE("split with commas")
{
    string text = "one,two,three,four";

    auto words = Cpp20::split_text(text, ",");

    print_all(words, "words");

    auto expected = {"one", "two", "three", "four"};

    REQUIRE(equal(begin(expected), end(expected), begin(words)));
}
