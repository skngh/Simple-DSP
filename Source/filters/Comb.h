#pragma once

#include <algorithm>
#include "../effects/DelayLine.h"

namespace sknight::filters
{
  // Comb<MAX_SIZE>: basic feedback comb filter.
  //
  //   SetDelay(delay)
  //   SetFeedback(fb)
  template <int MAX_SIZE>
  class Comb final
  {
  public:
    /** initialize comb */
    void Init() { Reset(); }

    /** reset comb */
    void Reset() { delay_line_.Reset(); }

    /** process comb */
    [[nodiscard]] float Process(const float in) noexcept
    {
        const float out = delay_line_.Read();
      delay_line_.Write(in + fb_ * out);
      return out;
    }

    /**
     * set delay
     * @param delay in samples
     */
    void SetDelay(const float delay) { delay_line_.SetDelay(delay); }

    /** set feedback */
    void SetFeedback(const float fb) { fb_ = std::clamp(fb, -0.999f, 0.999f); }

  private:
    float fb_ = 0.0f;
    effects::DelayLine<MAX_SIZE> delay_line_;
  };
} // namespace sknight::filters
