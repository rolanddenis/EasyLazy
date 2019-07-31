#include "EasyLazy/core.hpp"
#include "EasyLazy/range.hpp"
#include "EasyLazy/fancy_op.hpp"

#include "tools.hpp"

int main()
{
    {
        auto r = range(4);
        auto r_ref = array({0, 1, 2, 3});
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto r = range(-1, 4);
        auto r_ref = array({-1, 0, 1, 2, 3});
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto r = range(-1, 4, 2);
        auto r_ref = array({-1, 1, 3});
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto r = range(3, -2, -2);
        auto r_ref = array({3, 1, -1});
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto r = range(3, -1, 1);
        auto r_ref = array(std::array<int, 0>{});
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    return return_code();
}
