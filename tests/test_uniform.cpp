#include "EasyLazy/core.hpp"
#include "EasyLazy/uniform.hpp"
#include "EasyLazy/fancy_op.hpp"

#include "tools.hpp"

int main()
{
    {
        auto r = uniform(2., 2, 3);
        auto r_ref = array({2., 2.}) * array({1, 1, 1}, 1, 3);
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto r = uniform(2., 2, 3);
        auto r_ref = uniform(2., 2) * uniform(1, 1, 3);
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto r = uniform(1);
        auto r_ref = cst(1);
        std::cout << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    return return_code();
}

