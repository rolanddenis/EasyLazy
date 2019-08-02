/** Range expression
 */

#pragma once

#include "core.hpp"
#include "broadcast.hpp"
#include "map.hpp"

/// Range expression from start, stop (not reached) and optionally the step
template <typename T>
auto range(T start, T stop, T step = 1)
{
    static_assert(std::is_integral<T>::value, "Range must have integer value type");

    std::size_t size;
    if constexpr (std::is_signed_v<T>)
        size = std::max(T(0), T(1) + (stop - start - T(step > 0) + T(step < 0))/step);
    else
        size = std::max(T(0), T(1) + (stop - start - T(1))/step);

    return broadcast(
        map_with_idx(
            [start, step] (auto idx)
            {
                return start + (std::size(idx) > 0 ? T(idx[0]) : 0)*step;
            }
        ),
        size
    );
}

/// Range expression from 0 to stop (not reached)
template <typename T>
auto range(T stop)
{
    return range(T(0), stop);
}

