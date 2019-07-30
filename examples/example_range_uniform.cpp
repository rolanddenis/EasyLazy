#include <iostream>
#include <vector>
#include <EasyLazy/core.hpp>        // Core functions: array, cst, visitors, and arithmetic op as binary functions
#include <EasyLazy/fancy_op.hpp>    // Arithmetic op as binary operators (+-*/) and stream operator for shape
#include <EasyLazy/range.hpp>       // Ranges
#include <EasyLazy/uniform.hpp>     // Uniform matrix

int main()
{
    auto expr = range(0, 4) + uniform(1, 1, 2);

    auto shape = expr(visitor::shape{});
    std::cout << "Shape of the expression = " << shape << std::endl;
    std::cout << "Expr = " << expr << std::endl;

    return 0;
}
