#pragma once

#include <cmath>

namespace sknight::utilities
{

    // Utilities: misc helpers used across the dsp classes.
    // ValueChanged:
    // utilities::ValueChanged(hw.adc.GetFloat(0), last_frequency_, [&](float v) { lpf.SetFreq(v); }, 0.001f);

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

    /** convert from decibels to linear scale
     * @param db decibels
     */
    inline float DbToLin(const float db)
    {
        return std::pow(10.0f, db / 20.0f);
    }

    /** convert from decibels to linear scale. clamps at 0.0001 for safety
     * @param lin lin
     */
    inline float LinToDb(const float lin)
    {
        float clamp = (std::max)(lin, 0.0001f);
        return 20.0f * std::log10(clamp);
    }

    /** one pole filter */
    inline void SmoothingOnePole(float &out, float in, float coeff)
    {
        out += coeff * (in - out);
    }

} // namespace sknight::utilities
