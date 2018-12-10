#include <iostream>
#include <vector>
#include <EasyLazy/core.hpp>        // Core functions: array, cst, visitors, and arithmetic op as binary functions
#include <EasyLazy/fancy_op.hpp>    // Arithmetic op as binary operators (+-*/) and stream operator for shape
#include <EasyLazy/range.hpp>       // Ranges
#include <EasyLazy/uniform.hpp>     // Uniform matrix
#include <EasyLazy/view.hpp>        // View on expression

int main()
{
    // A storage
    std::vector<int> u = {1, 2, 3, 4, 5, 6, 7, 8};

    // Viewing it as an array
    auto array_u = array(u, 2, 2, 2);

    // Creating a view on it
    auto expr = view(array_u + 1.5, 0, newaxis(), all(), uniform(0, 4));
    auto shape = expr(visitor::shape{});
    std::cout << "Shape of the expression = " << shape << std::endl;

    return 0;
}
