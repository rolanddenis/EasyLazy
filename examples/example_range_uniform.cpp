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

    // TODO: streaming array
    std::cout << "Expr = [";
    for (std::size_t i = 0; i < shape[0]; ++i)
    {
        std::cout << "[";
        for (std::size_t j = 0; j < shape[1]; ++j)
            std::cout << expr(visitor::evaluator{i, j}) << (j < shape[1]-1 ? ", " : "");
        std::cout << "]" << (i < shape[0]-1 ? ", " : "");
    }
    std::cout << "]" << std::endl;

    return 0;
}
