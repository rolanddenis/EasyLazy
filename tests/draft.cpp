#include "EasyLazy/core.hpp"
#include "EasyLazy/range.hpp"
#include "EasyLazy/uniform.hpp"
#include "EasyLazy/view.hpp"
#include "EasyLazy/fancy_op.hpp"

int main()
{
    auto l = plus(cst(3), cst(2));
    std::cout << l(visitor::evaluator{}) << std::endl;
    std::cout << shape_to_size(l(visitor::shape{})) << std::endl;

    std::array v = {1, 2, 3, 4};
    auto e = plus(array(std::array{0.1, 0.2, 0.3, 0.4}, 4, 1), array(v, 1, 4));
    for (std::size_t i = 0; i < 4; ++i)
    {
        for (std::size_t j = 0; j < 4; ++j)
            std::cout << e(visitor::evaluator{i,j}) << " ";
        std::cout << std::endl;
    }

    auto ee = array(std::array{0.1, 0.2, 0.3, 0.4}, 4, 1) + array(v, 1, 4) * 2;
    for (std::size_t i = 0; i < 4; ++i)
    {
        for (std::size_t j = 0; j < 4; ++j)
            std::cout << ee(visitor::evaluator{i,j}) << " ";
        std::cout << std::endl;
    }

    std::cout << e(visitor::evaluator{}) << std::endl;
    //auto e = array<double>(3, 2);
    for ( auto const& s : e(visitor::shape{}) )
        std::cout << s << " x ";
    std::cout << std::endl;
    std::cout << shape_to_size(e(visitor::shape{})) << std::endl;

    auto r = range(0, 10);
    for ( std::size_t i = 0; i < 10; ++i )
        std::cout << r(visitor::evaluator{i,2}) <<  " ";
    std::cout << std::endl;
    std::cout << r(visitor::evaluator{}) << std::endl;

    using type_traits::is_expr;
    std::cout << "is_expr:" << std::endl;
    std::cout << is_expr(r) << std::endl;
    std::cout << is_expr(3) << std::endl;
    std::cout << is_expr([] (auto && visitor) {return visitor(true);}) << std::endl;
    std::cout << is_expr([] () {return true;}) << std::endl;
    std::cout << is_expr(cst(3)) << std::endl;
    std::cout << std::endl;

    using type_traits::dim;
    std::cout << dim(e) << std::endl;
    std::cout << std::endl;

    std::cout << "Test assignator" << std::endl;
    auto va = array(v, 2, 2);
    std::cout << va(visitor::evaluator{0, 1}) << std::endl;
    va(visitor::evaluator{0, 1}) = 4;
    std::cout << va(visitor::evaluator{0, 1}) << std::endl;
    std::cout << std::endl;

    using view_type = std::tuple<int, decltype(ee), all, int, newaxis, decltype(r)>;

    std::cout << "Test view output dim" << std::endl;
    std::cout << view_out_cumul_dim_v<view_type, 0> << std::endl;
    std::cout << view_out_cumul_dim_v<view_type, 1> << std::endl;
    std::cout << view_out_cumul_dim_v<view_type, 2> << std::endl;
    std::cout << view_out_cumul_dim_v<view_type, 3> << std::endl;
    std::cout << view_out_cumul_dim_v<view_type, 4> << std::endl;
    std::cout << view_out_cumul_dim_v<view_type, 5> << std::endl;
    std::cout << view_out_cumul_dim_v<view_type, 6> << std::endl;
    std::cout << "Full dim: " << view_out_dim_v<view_type> << std::endl;
    std::cout << std::endl;

    std::cout << "Test view input dim" << std::endl;
    std::cout << view_in_cumul_dim_v<view_type, 0> << std::endl;
    std::cout << view_in_cumul_dim_v<view_type, 1> << std::endl;
    std::cout << view_in_cumul_dim_v<view_type, 2> << std::endl;
    std::cout << view_in_cumul_dim_v<view_type, 3> << std::endl;
    std::cout << view_in_cumul_dim_v<view_type, 4> << std::endl;
    std::cout << view_in_cumul_dim_v<view_type, 5> << std::endl;
    std::cout << view_in_cumul_dim_v<view_type, 6> << std::endl;
    std::cout << "Full dim: " << view_in_dim_v<view_type> << std::endl;
    std::cout << std::endl;

    std::cout << "Test view index from output dim" << std::endl;
    std::cout << view_index_pos_from_out_dim_v<view_type, 0> << std::endl;
    std::cout << view_index_pos_from_out_dim_v<view_type, 1> << std::endl;
    std::cout << view_index_pos_from_out_dim_v<view_type, 2> << std::endl;
    std::cout << view_index_pos_from_out_dim_v<view_type, 3> << std::endl;
    std::cout << view_index_pos_from_out_dim_v<view_type, 4> << std::endl;
    std::cout << std::endl;

    std::cout << "Test view index shape" << std::endl;
    auto array5d = array(std::vector<double>(120), 1, 2, 3, 4, 5);
    view_type view_test{0, ee, all(), 2, newaxis(), r};
    auto array5d_shape = array5d(visitor::shape{});
    std::cout << view_index_shape<0>(visitor::shape{}, array5d_shape, view_test) << std::endl;
    std::cout << view_index_shape<1>(visitor::shape{}, array5d_shape, view_test) << std::endl;
    std::cout << view_index_shape<2>(visitor::shape{}, array5d_shape, view_test) << std::endl;
    std::cout << view_index_shape<3>(visitor::shape{}, array5d_shape, view_test) << std::endl;
    std::cout << view_index_shape<4>(visitor::shape{}, array5d_shape, view_test) << std::endl;
    std::cout << view_index_shape<5>(visitor::shape{}, array5d_shape, view_test) << std::endl;
    std::cout << std::endl;

    std::cout << "Test view size along dim" << std::endl;
    std::tuple array5d_view_shapes = {
        view_index_shape<0>(visitor::shape{}, array5d_shape, view_test),
        view_index_shape<1>(visitor::shape{}, array5d_shape, view_test),
        view_index_shape<2>(visitor::shape{}, array5d_shape, view_test),
        view_index_shape<3>(visitor::shape{}, array5d_shape, view_test),
        view_index_shape<4>(visitor::shape{}, array5d_shape, view_test),
        view_index_shape<5>(visitor::shape{}, array5d_shape, view_test)
    };
    std::cout << view_size_at_dim<view_type, 0>(array5d_view_shapes) << std::endl;
    std::cout << view_size_at_dim<view_type, 1>(array5d_view_shapes) << std::endl;
    std::cout << view_size_at_dim<view_type, 2>(array5d_view_shapes) << std::endl;
    std::cout << view_size_at_dim<view_type, 3>(array5d_view_shapes) << std::endl;
    std::cout << view_size_at_dim<view_type, 4>(array5d_view_shapes) << std::endl;
    std::cout << std::endl;

    std::cout << view(array5d, 0, ee, all(), 2, newaxis(), r)(visitor::shape{}) << std::endl;

    /*
    std::cout << view_out_dim_v<int, all, int, newaxis, decltype(r)> << std::endl;
    std::cout << view_out_cumul_dim_v<3, decltype(e), int, all, int, newaxis, decltype(r)> << std::endl;
    std::cout << view_in_dim_v<int, all, int, newaxis, decltype(r)> << std::endl;
    std::cout << view_in_cumul_dim_v<3, decltype(e), int, all, int, newaxis, decltype(r)> << std::endl;
    */
    std::cout << std::endl;

    std::cout << "Test uniform" << std::endl;
    auto u = uniform(1.5, 2, 2);
    std::cout << u(visitor::evaluator{0, 1}) << std::endl;
    std::cout << std::endl;

    return 0;
}
