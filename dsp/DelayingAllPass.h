#pragma once

#include <algorithm>
#include "DelayLine.h"

namespace sknight::dsp
{
template <int MAX_SIZE>
class DelayingAllPass final
{
  public:
    DelayingAllPass() {}
    ~DelayingAllPass() {}

    void Init() { delay_line_.Init(); }

    [[nodiscard]] float Process(const float in) noexcept
    {
        float delayed     = delay_line_.Read();
        float delay_input = in + gain_ * delayed; // w(n)
        delay_line_.Write(delay_input);
        return (-gain_ * delay_input) + delayed;
    }

    void SetDelay(const float delay) { delay_line_.SetDelay(delay); }

    // gain must stay within (-1, 1) for stability
    void SetGain(const float gain) { gain_ = std::clamp(gain, -0.999f, 0.999f); }

  private:
    DelayLine<MAX_SIZE> delay_line_;
    float               gain_ = 0.7f;
};
} // namespace sknight::dsp