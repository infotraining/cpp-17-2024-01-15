#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

using namespace std;

struct Circle
{
    int radius;

    void draw() const
    {
        std::cout << "Drawing Circle with r: " << radius << "\n";
    }
};

struct Rectangle
{
    int width, height;

    void draw() const
    {
        std::cout << "Drawing Rectangle with w: " << width << " & h: " << height << "\n";
    }
};

struct Square
{
    int size;

    void draw() const
    {
        std::cout << "Drawing Square with size: " << size << "\n";
    }
};

TEST_CASE("visit a shape variant and calculate area")
{
    using Shape = variant<Circle, Rectangle, Square>;

    vector<Shape> shapes = {Circle{1}, Square{10}, Rectangle{10, 1}};

    double total_area{};

    // TODO

    REQUIRE_THAT(total_area, Catch::Matchers::WithinRel(113.14, 0.01));
}