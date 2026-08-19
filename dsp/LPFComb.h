#pragma once

#include <algorithm>
#include "DelayLine.h"

namespace sknight::dsp
{
template <int MAX_SIZE>
class LPFComb final
{
  public:
    LPFComb() {}
    ~LPFComb() {}

    void Init() { delay_line_.Init(); }

    [[nodiscard]] float Process(const float in) noexcept
    {
        float out = delay_line_.Read();
        float lpf = out + g2_coeff * last_sample_;
        delay_line_.Write(in + fb_ * lpf);

        last_sample_ = lpf;
        return out;
    }

    void SetDelay(const float delay) { delay_line_.SetDelay(delay); }

    // feedback must stay within (-1, 1) for stability
    void SetFeedback(const float fb)
    {
        fb_ = std::clamp(fb, -0.999f, 0.999f);
        SetDamping(damping_);
    }

    // damping is 0 - .9999
    void SetDamping(const float damping)
    {
        damping_ = std::clamp(damping, 0.0f, 0.999f);
        g2_coeff = damping_ * (1.0f - fb_);
    }

  private:
    float               fb_ = 0.0f;
    DelayLine<MAX_SIZE> delay_line_;
    float               last_sample_ = 0.0f;
    float               damping_     = 0.0f;
    float               g2_coeff     = 0.0f;
};
} // namespace sknight::dsp