#pragma once

#include <algorithm>

namespace sknight::filters
{
  // APF: single-sample first-order allpass filter (Schroeder allpass).
  //
  //   SetGain(gain)
  class APF final
  {
  public:
    /** initialize apf */
    void Init()
    {
      gain_ = 0.7f;
      Reset();
    }

    /** reset apf */
    void Reset() { last_sample_ = 0.0f; }

    /** process apf */
    [[nodiscard]] float Process(const float in) noexcept
    {
      const float delayed = last_sample_;
      const float delay_input = in + gain_ * delayed; // w(n)
      last_sample_ = delay_input;
      return (-gain_ * delay_input) + delayed;
    }

    /** set gain
     * @param gain default 0.7f. range -.1 - 1
     */

    void SetGain(const float gain) { gain_ = std::clamp(gain, -0.999f, 0.999f); }

  private:
    float last_sample_ = 0.0f;
    float gain_ = 0.7f;
  };
} // namespace sknight::filters
