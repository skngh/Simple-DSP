#pragma once

#include <cmath>

namespace sknight
{

inline constexpr float PI = 3.14159274f;

class OnePole
{
  public:
    enum class FilterType
    {
        LowPass,
        HighPass
    };

    OnePole() {};
    ~OnePole() {};

    void Init(float sample_rate)
    {
        sample_rate_ = sample_rate;
        coeff_       = 0.0f;
        last_sample_ = 0.0f;
    }

    float Process(float in) noexcept
    {
        float lp = (1.0f - coeff_) * in + coeff_ * last_sample_;

        last_sample_ = lp;

        return type_ == FilterType::LowPass ? lp : in - lp;
    }

    void SetCutoff(float frequency)
    { coeff_ = std::exp(-2.0f * PI * frequency / sample_rate_); }

    void SetType(FilterType type) { type_ = type; }


  private:
    float      sample_rate_ = 48000;
    float      coeff_       = 0.0f;
    float      last_sample_ = 0.0f;
    FilterType type_        = FilterType::LowPass;
};
} // namespace sknight