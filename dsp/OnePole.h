#pragma once

#include <cmath>

namespace sknight::dsp
{

    inline constexpr float PI = 3.14159274f;

    // OnePole: one-pole low/high-pass filter.
    //
    //   SetCutoff(frequency)
    //   SetType(type)
    class OnePole final
    {
    public:
        enum class FilterType
        {
            LowPass,
            HighPass
        };

        OnePole() {}
        ~OnePole() {}

        /** initialize onepole */
        void Init(float sample_rate)
        {
            sample_rate_ = sample_rate;
            coeff_ = 0.0f;
            Reset();
        }

        /** reset onepole */
        void Reset() { last_sample_ = 0.0f; }

        /** process onepole */
        [[nodiscard]] float Process(const float in) noexcept
        {
            float lp = (1.0f - coeff_) * in + coeff_ * last_sample_;

            last_sample_ = lp;

            return type_ == FilterType::LowPass ? lp : in - lp;
        }

        /**
         * set cutoff
         * @param frequency freq in hz
         */
        void SetCutoff(const float frequency)
        {
            coeff_ = std::exp(-2.0f * PI * frequency / sample_rate_);
        }

        /** set filter type */
        void SetType(FilterType type) { type_ = type; }

    private:
        float sample_rate_ = 48000.0f;
        float coeff_ = 0.0f;
        float last_sample_ = 0.0f;
        FilterType type_ = FilterType::LowPass;
    };
} // namespace sknight::dsp
