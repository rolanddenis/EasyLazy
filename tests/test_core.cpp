#include <vector>

#include "EasyLazy/core.hpp"
#include "EasyLazy/fancy_op.hpp"

#include "tools.hpp"

int main()
{
    ///////////////////////////////////////////////////////////////////////////
    // Apply
    std::cout << "Checking apply:" << std::endl;
    {
        auto r = array({1, 2, 3, 4, 5, 6}, 2, 3);
        auto r_ref = std::vector{1, 2, 3, 4, 5, 6};

        auto it = r_ref.begin();
        auto cmp = true;
        apply([&it, &cmp] (auto a) { cmp &= a == *it++; }, r);
        CHECK( cmp );
        CHECK( it == r_ref.end() );
    }

    {
        auto r1 = array({1, 2, 3}, 1, 3);
        auto r2 = array({1, 2}, 2, 1);
        auto r_ref = std::vector{2, 3, 4, 3, 4, 5};

        auto it = r_ref.begin();
        auto cmp = true;
        apply([&it, &cmp] (auto a, auto b) { cmp &= a + b == *it++; }, r1, r2);
        CHECK( cmp );
        CHECK( it == r_ref.end() );
    }
    std::cout << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    // Strict array equality
    std::cout << "Checking array_equal:" << std::endl;
    {
        auto r = array({1, 2, 3, 4, 5, 6}, 2, 3);
        auto r_ref = array({1, 2, 3, 4, 5, 6}, 2, 3);

        CHECK( array_equal(r, r_ref) );
        CHECK( array_equal(r, r_ref, r) );
        r(visitor::evaluator{0, 2}) = 7;
        CHECK( !array_equal(r, r_ref) );
        CHECK( !array_equal(r, r_ref, r) );
    }

    {
        auto r = array({1, 2, 3}, 1, 3);
        auto r_ref = array({1, 2, 3, 1, 2, 3}, 2, 3);

        CHECK( !array_equal(r, r_ref) );
    }
    std::cout << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    // Array equality with broadcasting
    std::cout << "Checking array_equal_equiv:" << std::endl;
    {
        auto r = array({1, 2, 3, 4, 5, 6}, 2, 3);
        auto r_ref = array({1, 2, 3, 4, 5, 6}, 2, 3);

        CHECK( array_equal_equiv(r, r_ref) );
        CHECK( array_equal_equiv(r, r_ref, r) );
        r(visitor::evaluator{0, 2}) = 7;
        CHECK( !array_equal_equiv(r, r_ref) );
        CHECK( !array_equal_equiv(r, r_ref, r) );
    }

    {
        auto r = array({1, 2, 3}, 1, 3);
        auto r_ref = array({1, 2, 3, 1, 2, 3}, 2, 3);

        CHECK( array_equal_equiv(r, r_ref) );
        CHECK( array_equal_equiv(r_ref, r, r_ref) );
    }
    std::cout << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    // Assignment
    std::cout << "Checking assignment:" << std::endl;
    {
        std::vector<double> data;
        auto r_ref = array({1, 2, 3, 4, 5, 6}, 2, 3);
        auto r = array(data, 2, 3);
        r << r_ref;
        std::cout << "r = " << r << std::endl;

        CHECK( array_equal(r, r_ref) );
    }
    std::cout << std::endl;

    return 0;
}
