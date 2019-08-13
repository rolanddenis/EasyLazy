#pragma once

#include "core.hpp"

/// Map tag
struct map_tag : expr_tag {};

/** Map a function to given expressions depending on the index
 *
 * @note Can be used without any associated expression.
 */
template <typename Function, typename... Expr>
auto map_with_idx(Function && function, Expr &&... expr)
{
    return
        [fn = std::forward<Function>(function), operands = hold_args(make_it_expr(std::forward<Expr>(expr))...)]
        (auto && visitor) mutable -> decltype(auto)
        {
            return std::forward<decltype(visitor)>(visitor)(map_tag{}, fn, dispatch_visitor(visitor, operands));
        };
}

/// Map a function to given expressions
template <typename Function, typename... Expr>
auto map(Function && function, Expr &&... expr)
{
    return map_with_idx(
        [fn = std::forward<Function>(function)]
        (auto const&, auto &&... v)
        {
            return fn(std::forward<decltype(v)>(v)...);
        },
        std::forward<Expr>(expr)...
    );
}

namespace
{
    template <std::size_t N, typename Function, typename Values, std::size_t... I>
    auto map_with_idx_evaluation(visitor::evaluator<N> const& e, Function && fn, Values && values, std::index_sequence<I...>)
    {
       return std::forward<Function>(fn)(e.idx, std::get<I>(values)...);
    }
}

// Extending core visitors
namespace visitor
{
    /// Shape of the result of a component-wise function mapping
    template <typename Function, typename Shapes>
    auto visit(visitor::shape const&, map_tag, Function &&, Shapes const& shapes)
    {
        return broadcast_shape(shapes);
    }

    /// Evaluation of the result of a component-wise function mapping at a given index
    template <std::size_t N, typename Function, typename Values>
    auto visit(visitor::evaluator<N> const& e, map_tag, Function && fn, Values && values)
    {
        return map_with_idx_evaluation(e, std::forward<Function>(fn), std::forward<Values>(values), std::make_index_sequence<std::tuple_size_v<Values>>{});
    }
}

