#pragma once

namespace sknight
{
class APF final
{
  public:
    APF() {};
    ~APF() {};

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

    void SetGain(const float gain) { gain_ = gain; }

  private:
    float last_sample_ = 0.0f;
    float gain_        = 0.7f;
};
} // namespace sknight