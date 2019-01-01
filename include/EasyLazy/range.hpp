#pragma once

#include "core.hpp"

/// Range tag
struct range_tag : expr_tag {};

/// Range expression
template <typename T>
auto range(T start, T stop, T step = 1)
{
    static_assert(std::is_integral<T>::value, "Range must have integer value type");
    return [=] (auto && visitor) -> decltype(auto)
    {
        return std::forward<decltype(visitor)>(visitor)(range_tag{}, start, stop, step);
    };
}

template <typename T>
auto range(T stop)
{
    return range(0, stop);
}

// Extending core visitors
namespace visitor
{
    template <typename T>
    auto visit(visitor::shape const&, range_tag, T start, T stop, T step)
    {
        return std::array{std::max<std::size_t>(0, (stop-start-1)/step)}; 
    }

    template <std::size_t N, typename T>
    auto visit(visitor::evaluator<N> const& e, range_tag, T start, T, T step)
    {
        return start + (N > 0 ? e.idx[0] : 0)*step;
    }
} // namespace visitor

