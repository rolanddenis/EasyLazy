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

// Display of a shape
template <std::size_t N>
std::ostream & operator<< (std::ostream & os, std::array<std::size_t, N> const& shape)
{
    os << "[";
    for (std::size_t i = 0; i < N; ++i)
        os << (i > 0 ? ", " : "") << shape[i];
    os << "]";
    return os;
}
