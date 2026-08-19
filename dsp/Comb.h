#pragma once

#include "DelayLine.h"

namespace sknight
{
template <int MAX_SIZE>
class Comb
{
  public:
    Comb() {};
    ~Comb() {};

    void Init() { delay_line_.Init(); }

    [[nodiscard]] float Process(float in) noexcept
    {
        float out = delay_line_.Read();
        delay_line_.Write(in + fb_ * out);
        return out;
    }

    void SetDelay(float delay) { delay_line_.SetDelay(delay); }

    void SetFeedback(float fb) { fb_ = fb; }

  private:
    float                        fb_ = 0.0f;
    sknight::DelayLine<MAX_SIZE> delay_line_;
};
} // namespace sknight