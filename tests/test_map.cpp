#include <functional>

#include "EasyLazy/core.hpp"
#include "EasyLazy/map.hpp"
#include "EasyLazy/fancy_op.hpp"

#include "tools.hpp"

int main()
{
    {
        auto a = array({1, 2, 3}, 1, 3);
        auto b = array({1, 2}, 2, 1);
        auto r = map(std::plus<>{}, a, b);
        auto r_ref = a + b;
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto r = map(std::plus<>{}, array({1, 2, 3}, 1, 3), array({1, 2}, 2, 1));
        auto r_ref = array({1, 2, 3}, 1, 3) + array({1, 2}, 2, 1);
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto a = array({1, 2, 3}, 1, 3);
        auto b = array({1, 2}, 2, 1);
        auto c = array({3, 4}, 1, 1, 2);
        auto r = map([] (auto u, auto v, auto w) { return u + v - w; }, a, b, c);
        auto r_ref = a + b - c;
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto a = array({1, 2, 3, 4, 5, 6}, 2, 3);
        auto r = map_with_idx([] (auto idx, auto v) { return int(idx[0]) + int(idx[1]) + v; }, a);
        auto r_ref = array({0, 1}, 2, 1) + array({0, 1, 2}, 1, 3) + a;
        std::cout << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto a = array({1, 2, 3, 4, 5, 6}, 2, 3);
        auto r = map_with_idx([] (auto idx) { return int(idx[0]) + int(idx[1]); }) + a; // Can be used without any expression but it is to the user to check that evaluation context will be valid (i.e. at least 2D here).
        auto r_ref = array({0, 1}, 2, 1) + array({0, 1, 2}, 1, 3) + a;
        std::cout << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    return return_code();
}

