/** Uniform array expression
 */

#pragma once

#include "broadcast.hpp"

/// Uniform array expression
template <typename T, typename... Sizes>
auto uniform(T && value, Sizes const&... sizes)
{
    return broadcast_to(std::forward<T>(value), sizes...);
}

/// Array filled with zeros
template <typename T, typename... Sizes>
auto zeros(Sizes const&... sizes)
{
    return uniform(T(0), sizes...);
}

/// Array filled with ones
template <typename T, typename... Sizes>
auto ones(Sizes const&... sizes)
{
    return uniform(T(1), sizes...);
}

