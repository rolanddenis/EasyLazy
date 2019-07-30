#include <iostream>
#include <vector>
#include <EasyLazy/core.hpp>        // Core functions: array, cst, visitors, and arithmetic op as binary functions
#include <EasyLazy/fancy_op.hpp>    // Arithmetic op as binary operators (+-*/) and stream operator for shape

int main()
{
    // A storage
    std::vector<double> u = {0.1, 0., 0.3, 0.4};

    // Viewing it as a 2D array
    auto array_u = array(u, 2, 2);
    std::cout << "Shape of array_u = " << array_u(visitor::shape{}) << std::endl;
    array_u(visitor::evaluator{0, 1}) = 0.2;
    std::cout << "array_u[0, 1] = " << array_u(visitor::evaluator{0, 1}) << std::endl;
    std::cout << "u[1] = " << u[1] << std::endl;
    std::cout << std::endl;

    // Building an expression with this array
    auto expr = array_u * array(std::array{2, 3}, 2, 1) + 1.5;
    auto shape = expr(visitor::shape{});
    std::cout << "Shape of the expression = " << shape << std::endl;
    std::cout << "Expr = " << expr << std::endl;
    return 0;
}
