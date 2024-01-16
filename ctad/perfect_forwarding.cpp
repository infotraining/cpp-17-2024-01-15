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


struct Client
{
    std::string fname, lname;
    std::vector<std::string> data;

    // Client(const std::string& f, const std::string& l, const std::vector<std::string>& d) : fname(f), lname(l), data(d)
    // {}

    Client(std::string f, std::string l, std::vector<std::string> d) : fname(std::move(f)), lname(std::move(l)), data(std::move(d))
    {}

    // template<typename TArg1, typename TArg2, typename TArg3>
    // Client(TArg1&& f, TArg2&& l, TArg3&& d) : fname(std::forward<TArg1>(f)), lname(std::forward<TArg2>(l)), data(std::forward<TArg3>(d))
    // {}

    // void push(const std::string& item)
    // {
    //     data.push_back(item);
    // }

    // void push(std::string&& item)
    // {
    //     data.push_back(std::move(item));
    // }

    template <typename TItem>
    void push(TItem&& item)
    {
        data.push_back(std::forward<TItem>(item));
    }
};

TEST_CASE("passing params to constructors")
{
    std::string name = "John";
    
    Client c1{name, "Smith", std::vector<std::string>{"1", "2", "3", "4"}};
    c1.push(name);
    c1.push("text");
}

struct Base
{
    std::string base;

    Base(std::string b) : base{std::move(b)}
    {}
};

struct Derived : Base
{
    std::string derived;

    Derived(std::string b, std::string d) : Base(std::move(b)), derived(std::move(d))
    {}
};

