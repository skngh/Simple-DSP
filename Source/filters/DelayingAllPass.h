#pragma once

#include <algorithm>
#include "Source/effects/DelayLine.h"

namespace sknight::filters
{
  // DelayingAllPass<MAX_SIZE>: Schroeder allpass filter built around a DelayLine, for reverb diffusion.
  //
  //   SetDelay(delay)
  //   SetGain(gain)
  template <int MAX_SIZE>
  class DelayingAllPass final
  {
  public:
    /** initialize delayingallpass */
    void Init() { Reset(); }

    /** reset delayingallpass */
    void Reset() { delay_line_.Reset(); }

    /** process delayingallpass */
    [[nodiscard]] float Process(const float in) noexcept
    {
      float delayed = delay_line_.Read();
      float delay_input = in + gain_ * delayed; // w(n)
      delay_line_.Write(delay_input);
      return (-gain_ * delay_input) + delayed;
    }

    /**
     * set delay
     * @param delay in samples
     */
    void SetDelay(const float delay) { delay_line_.SetDelay(delay); }

    /** set gain */
    void SetGain(const float gain) { gain_ = std::clamp(gain, -0.999f, 0.999f); }

  private:
    effects::DelayLine<MAX_SIZE> delay_line_;
    float gain_ = 0.7f;
  };
} // namespace sknight::filters
