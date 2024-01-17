#include <algorithm>
#include <array>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
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

TEST_CASE("string_view")
{
    std::string_view empty;
    REQUIRE(empty.data() == nullptr);
    REQUIRE(empty.size() == 0); 

    std::string str = "hello world";
    std::string_view str_sv = str; // implicit conversion: string -> string_view
    REQUIRE(str_sv == "hello world"sv);

    const char* cstr = "hello world";
    std::string_view cstr_sv = cstr;
    REQUIRE(cstr_sv == "hello world"sv); // implicit conversion: const char* -> string_view

    std::vector vec = {1, 2, 3};

    std::string prefix = "vec";
    print_all(vec, prefix);

    print_all(vec, "vec"); // better performance with std::string_view

    std::string text(str_sv);
}

TEST_CASE("string_view is not null terminated")
{
    std::array token = { 'a', 'b', 'c' }; 

    std::string_view token_sv{token.data(), token.size()};

    REQUIRE(token_sv == "abc");
}

void disaster_with_string_view(std::string_view file_name)
{
    FILE* f = fopen(file_name.data(), "rw"); // DISASTER - beware - fopen requires passing null-terminated string
}

TEST_CASE("beware with null terminated strings")
{
    disaster_with_string_view("abc");

    // std::array token = { 'a', 'b', 'c' }; 
    // std::string_view filename{token.data(), token.size()};
    // disaster_with_string_view(filename);
}

string_view start_from_word(string_view text, string_view word)
{
      return text.substr(text.find(word));
}

std::string get_line()
{
    return "HELLOWORLD!!!";
}

TEST_CASE("tokenizing & string_view")
{
    std::string text = "abc def ghi";

    std::string_view one(text.data(), 3);
    REQUIRE(one == "abc");

    std::string_view two(text.data() + 4, 3);
    REQUIRE(two == "def");

    auto token = start_from_word("HELLOWORLD!!!", "WORLD");
    REQUIRE(token == "WORLD!!!");

    token = start_from_word(get_line(), "WORLD"); // token has dangling pointer - UB!!!
    REQUIRE(token == "WORLD!!!");

    std::string_view evil = "text"s; // UB!!! - dangling pointer
    std::cout << evil << "\n";
}

TEST_CASE("string_view & sort")
{
    const std::vector<std::string> words = { "zero", "one", "two", "four", "three" };

    std::vector<std::string_view> words_sv(words.begin(), words.end());
    std::sort(words_sv.begin(), words_sv.end());

    print_all(words_sv, "sorted words");
}