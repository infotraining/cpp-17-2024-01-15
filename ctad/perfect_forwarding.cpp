#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct Gadget
{
    std::string id;

    void use() // 1
    {
        std::cout << "Using gadget " << id << "\n";
    }

    void use() const // 2
    {
        std::cout << "Using gadget " << id << "\n";
    }
};

void use(Gadget& g)
{
    std::cout << "void use(Gadget& g)\n";
    g.use(); // 1
}

void use(const Gadget& g)
{
    std::cout << "void use(const Gadget& g)\n";
    g.use(); // 2
}

void use(Gadget&& g)
{
    std::cout << "void use(Gadget&& g)\n";
    g.use(); // 1
}

// void have_fun(Gadget& g)
// {
//     use(g);
// }

// void have_fun(const Gadget& g)
// {
//     use(g);
// }

// void have_fun(Gadget&& g)
// {
//     use(std::move(g));
// }

template <typename TGadget>
void have_fun(TGadget&& g)        // universal reference
{
    use(std::forward<TGadget>(g));
}

TEST_CASE("perfect forwarding")
{
    Gadget g{"g1"};
    const Gadget cg{"g2"};

    use(g);
    use(cg);
    use(Gadget{"temp_g"});

    std::cout << "\n----------------------\n";

    have_fun(g);  // lvalue
    have_fun(cg); // lvalue
    have_fun(Gadget{"temp_g"}); // rvalue
}