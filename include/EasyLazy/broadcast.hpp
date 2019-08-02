/** Broadcast an expression to a given shape
 *
 */

#pragma once

#include "core.hpp"

/// Broadcast tag
struct broadcast_tag : expr_tag {};

/// Broadcast expression
template <typename Expr, typename... Sizes>
auto broadcast(Expr && expression, Sizes const&... sizes)
{
    return
        [ expr = make_it_expr(std::forward<Expr>(expression)),
          shape = std::array<std::size_t, sizeof...(Sizes)>{static_cast<std::size_t>(sizes)...}
        ]
        (auto && visitor) mutable -> decltype(auto)
        {
            return std::forward<decltype(visitor)>(visitor)(broadcast_tag{}, expr, shape);
        };
}

// Extending core visitors
namespace visitor
{
    template <typename Expr, typename Shape>
    auto visit(visitor::shape const& shape_visitor, broadcast_tag, Expr && expr, Shape const& shape)
    {
        return broadcast_shape(
            std::forward<Expr>(expr)(shape_visitor),
            shape
        );
    }

    template <std::size_t N, typename Expr, typename Shape>
    auto visit(visitor::evaluator<N> const& eval_visitor, broadcast_tag, Expr && expr, Shape const&)
    {
        return std::forward<Expr>(expr)(eval_visitor);
    }
} // namespace visitor

