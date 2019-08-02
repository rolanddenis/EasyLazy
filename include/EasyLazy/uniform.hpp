/** Uniform array expression
 */

#pragma once

#include "core.hpp"
#include "broadcast.hpp"

/// Uniform array expression
template <typename T, typename... Sizes>
auto uniform(T && value, Sizes const&... sizes)
{
    return broadcast(std::forward<T>(value), sizes...);
}

