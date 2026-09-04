#pragma once

#include <cmath>

namespace sknight::utilities
{

    // Utilities: misc helpers used across the dsp classes.

    inline constexpr float kPi = 3.14159274f;
    /**
     * calls on_change and updates last_value if value has moved past threshold
     * @param threshold minimum change required to trigger on_change
     */
    template <typename T, typename Fn>
    void ValueChanged(T value, T &last_value, Fn &&on_change, const float threshold = 0.001f)
    {
        if (fabsf(static_cast<float>(value - last_value)) > threshold)
        {
            on_change(value);
            last_value = value;
        }
    }

} // namespace sknight::utilities
