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
    std::cout << "array_u = " << array_u << std::endl;

    // Creating a view on it
    auto expr = view(array_u + 1.5, 0, newaxis(), all(), uniform(1, 2, 2));
    auto shape = expr(visitor::shape{});
    std::cout << "Shape of the expression = " << shape << std::endl;
    std::cout << expr(visitor::evaluator{0, 1, 0, 0}) << std::endl;
    std::cout << expr << std::endl;

    auto expr2 = view(array_u, 0, 0, 0);
    std::cout << expr2(visitor::shape{}) << std::endl;
    std::cout << type_traits::dim(expr2) << std::endl;
    std::cout << expr2(visitor::evaluator{}) << std::endl;
    std::cout << expr2 << std::endl;

    auto expr3 = view(array_u + 1.5, all(), newaxis(), all(), uniform(1, 2, 2));
    std::cout << "shape(expr3) = " << expr3(visitor::shape{}) << std::endl;
    std::cout << "expr3 = " << expr3 << std::endl;

    auto expr4 = view(array_u + 1.5, 0);
    std::cout << "shape(expr4) = " << expr4(visitor::shape{}) << std::endl;
    std::cout << "expr4 = " << expr4 << std::endl;

    std::vector<double> u2(16);
    view(array(u2, 2, 2, 2, 2), all(), newaxis(), all(), all(), all()) << expr3;
    for (auto const& value : u2)
        std::cout << value << " ";
    std::cout << std::endl;

    array_u << view(range(2), all(), newaxis(), newaxis());
    std::cout << array_u << std::endl;

    array_u << 3;
    std::cout << array_u << std::endl;

    auto A = array({1, 2, 3, 4}, 2, 2);
    std::cout << A << std::endl;

    auto B = array(expr4);
    std::cout << B << std::endl;
    view(B, 0) << 3;
    std::cout << B << std::endl;

    {
      std::vector<int> data{1, 2, 3, 4, 5, 6};
      auto a = array(data, 2, 3); // 2D array of shape (2, 3)
      display(std::cout, a);
      a(visitor::evaluator{0, 2}) = 10;
      std::cout << data[2] << std::endl;
    }
    
    {
      std::vector<int> data;
      auto a = array(data, 2, 3);
      std::cout << "data.size() = " << data.size() << std::endl;
      std::cout << "a = " << a << std::endl;
      auto b = array<int>({}, 2, 3); // Empty initializer list of given type
      std::cout << "b = " << b << std::endl;
    }

    {
        std::vector<int> data;
        auto a = array(data, cst(3));
        std::cout << a << std::endl;
        std::cout << data.size() << std::endl;
        std::cout << shape_to_size(a(visitor::shape{})) << std::endl;
    }
    return 0;
}
