#pragma once

#include <cmath>

namespace sknight
{
namespace utilities
{

template <typename T, typename Fn>
void ValueChanged(T value, T &last_value, Fn &&on_change, float threshold = 0.001f)
{
    if(fabsf(static_cast<float>(value - last_value)) > threshold)
    {
        on_change(value);
        last_value = value;
    }
}

} // namespace utilities
} // namespace sknight
