#pragma once

#include <algorithm>
#include "DelayLine.h"

namespace sknight::dsp
{
template <int MAX_SIZE>
class Comb final
{
  public:
    Comb() {}
    ~Comb() {}

    void Init() { delay_line_.Init(); }

    [[nodiscard]] float Process(const float in) noexcept
    {
        float out = delay_line_.Read();
        delay_line_.Write(in + fb_ * out);
        return out;
    }

    void SetDelay(const float delay) { delay_line_.SetDelay(delay); }

    // feedback must stay within (-1, 1) for stability
    void SetFeedback(const float fb) { fb_ = std::clamp(fb, -0.999f, 0.999f); }

  private:
    float               fb_ = 0.0f;
    DelayLine<MAX_SIZE> delay_line_;
};
} // namespace sknight::dsp