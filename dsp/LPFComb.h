#pragma once

#include "DelayLine.h"

namespace sknight
{
template <int MAX_SIZE>
class LPFComb
{
  public:
    LPFComb() {};
    ~LPFComb() {};

    void Init() { delay_line_.Init(); }

    float Process(float in) noexcept
    {
        float out = delay_line_.Read();
        float lpf = out + g2_coeff * last_sample_;
        delay_line_.Write(in + fb_ * lpf);

        last_sample_ = lpf;
        return out;
    }

    void SetDelay(float delay) { delay_line_.SetDelay(delay); }

    void SetFeedback(float fb)
    {
        fb_ = fb;
        SetDamping(damping_);
    }

    // damping is 0 - .9999
    void SetDamping(float damping)
    {
        damping_ = damping > 0.999f ? 0.999f : damping;
        g2_coeff = damping_ * (1.0f - fb_);
    }

  private:
    float                        fb_ = 0.0f;
    sknight::DelayLine<MAX_SIZE> delay_line_;
    float                        last_sample_ = 0.0f;
    float                        damping_     = 0.0f;
    float                        g2_coeff     = 0.0f;
};
} // namespace sknight