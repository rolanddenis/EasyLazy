#pragma once

#include "core.hpp"

/// Uniform matrix tag
struct uniform_tag : expr_tag {};

/// Uniform matrix expression
template <typename T, typename... Sizes>
auto uniform(T value, Sizes const&... sizes)
{
    return [value,
            shape = std::array<std::size_t, sizeof...(Sizes)>{static_cast<std::size_t>(sizes)...}]
        (auto && visitor) -> decltype(auto)
        {
            return std::forward<decltype(visitor)>(visitor)(uniform_tag{}, value, shape);
        };
}

// Extending core visitors
namespace visitor
{
    template <typename T, typename S>
    auto visit(visitor::shape const&, uniform_tag, T, S const& s)
    {
        return s;
    }

    template <std::size_t N, typename T, typename S>
    auto visit(visitor::evaluator<N> const&, uniform_tag, T value, S)
    {
        return value;
    }
} // namespace visitor
