#include "EasyLazy/core.hpp"
#include "EasyLazy/broadcast.hpp"
#include "EasyLazy/fancy_op.hpp"

#include "tools.hpp"

int main()
{
    {
        auto a = 2;
        auto r = broadcast(a, 2, 3);
        auto r_ref = array({a, a, a, a, a, a}, 2, 3);
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto a = array({1, 2});
        auto r = broadcast(a, 2, 3);
        auto r_ref = array({1, 1, 1, 2, 2, 2}, 2, 3);
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto r = broadcast(array({1, 2}), 2, 3);
        auto r_ref = array({1, 1, 1, 2, 2, 2}, 2, 3);
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    return return_code();
}
