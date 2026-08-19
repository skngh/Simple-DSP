#pragma once

#include <algorithm>

namespace sknight::dsp
{
class APF final
{
  public:
    APF() {}
    ~APF() {}

    void Init()
    {
        last_sample_ = 0.0f;
        gain_        = 0.7f;
    }

    [[nodiscard]] float Process(const float in) noexcept
    {
        float delayed     = last_sample_;
        float delay_input = in + gain_ * delayed; // w(n)
        last_sample_      = delay_input;
        return (-gain_ * delay_input) + delayed;
    }

    // gain must stay within (-1, 1) for stability
    void SetGain(const float gain) { gain_ = std::clamp(gain, -0.999f, 0.999f); }

  private:
    float last_sample_ = 0.0f;
    float gain_        = 0.7f;
};
} // namespace sknight::dsp