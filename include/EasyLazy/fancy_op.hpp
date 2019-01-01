#pragma once

#include "core.hpp"

#include <ostream>

#define FANCY_BINARY_OP(op, fn) \
    template <typename LHS,                                     \
              typename RHS,                                     \
              typename = std::enable_if_t<type_traits::is_expr_v<LHS> || type_traits::is_expr_v<RHS>>> \
    auto operator op (LHS && lhs, RHS && rhs) -> decltype(auto) \
    {                                                           \
        return fn(                                              \
            make_it_expr(std::forward<LHS>(lhs)),               \
            make_it_expr(std::forward<RHS>(rhs))                \
        );                                                      \
    }

FANCY_BINARY_OP(+, plus)
FANCY_BINARY_OP(-, minus)
FANCY_BINARY_OP(*, multiplies)
FANCY_BINARY_OP(/, divides)


/// Display of an expression
template <typename Expr, typename = std::enable_if_t<type_traits::is_expr_v<Expr>>>
std::ostream & operator<< (std::ostream & os, Expr && expr)
{
    display(os, expr);
    return os;
}

/// Display of a shape
template <std::size_t N>
std::ostream & operator<< (std::ostream & os, std::array<std::size_t, N> const& shape)
{
    display(os, shape);
    return os;
}

/// Assign an expression to an array
template <typename LHS, typename RHS, typename = std::enable_if_t<type_traits::is_expr_v<LHS>>>
LHS operator<< (LHS && lhs, RHS && rhs)
{
    assign(lhs, std::forward<RHS>(rhs));
    return std::forward<LHS>(lhs);
}

