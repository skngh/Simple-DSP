#pragma once

#include <cmath>
#include <algorithm>
#include "../utilities/Utilities.h"

namespace sknight::filters
{
    // Biquad bandpass filter with constant skirt gain. I plan on adding other biquad coeff variations to this eventually
    // SetFreq(freq)
    // SetQ(q)
    // adapted from https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
    class Biquad final
    {
    public:
        /** initiailize biquad */
        void Init(const float sample_rate)
        {
            sample_rate_ = sample_rate;
            max_freq_ = sample_rate_ * 0.45f;
            SetCoeffs();
            Reset();
        }

        /** reset biquad */
        void Reset()
        {
            x1_ = 0.0f;
            x2_ = 0.0f;
            y1_ = 0.0f;
            y2_ = 0.0f;
        }

        /** process biquad */
        [[nodiscard]] float Process(const float in) noexcept
        {
            const float out = b0_ * in + b1_ * x1_ + b2_ * x2_ - a1_ * y1_ - a2_ * y2_;

            x2_ = x1_;
            x1_ = in;
            y2_ = y1_;
            y1_ = out;

            return out;
        }

        /** set q
         * @param q clamped at min 0.01f. range typically 0.1-100
         */
        void SetQ(const float q)
        {
            q_ = q;
            SetCoeffs();
        }

        /** set center freq
         * @param freq in hz
         */
        void SetFreq(const float freq)
        {
            SetFreqInternal(freq);
            SetCoeffs();
        }

        /** set freq and q together. avoid extra coeff calling cost
         * @param freq center freq in hz
         * @param q clamped at min 0.01f. range typically 0.1-100
         */
        void SetParams(const float freq, const float q)
        {
            SetFreqInternal(freq);
            q_ = q;
            SetCoeffs();
        }

        /** set freq and T60 together. avoid extra coeff calling cost
         * @param freq center freq in hz
         * @param time time in seconds till 60dB
         */
        void SetParamsT60(const float freq, const float time)
        {
            SetFreqInternal(freq);
            q_ = time * utilities::kPi * freq_ / 6.91f;
            SetCoeffs();
        }

        /** set decay time using T60 (amount of time until it hits -60dB)
         * @param time time in seconds
         */
        void SetT60(const float time)
        {
            q_ = time * utilities::kPi * freq_ / 6.91f;
            SetCoeffs();
        }

    private:
        void SetCoeffs()
        {
            q_ = (std::max)(q_, 0.01f);

            const float omega = 2.0f * utilities::kPi * (freq_ / sample_rate_);

            const float sin_omega = std::sin(omega);
            const float cos_omega = std::cos(omega);

            const float alpha = sin_omega / (2.0f * q_);

            const float a0 = 1.0f + alpha;
            const float norm = 1.0f / a0;

            b0_ = q_ * alpha * norm;
            b1_ = 0.0f;
            b2_ = q_ * -alpha * norm;
            a1_ = -2.0f * cos_omega * norm;
            a2_ = (1.0f - alpha) * norm;
        }

        void SetFreqInternal(const float freq)
        {
            freq_ = (std::clamp)(freq, 20.0f, max_freq_);
        }

        float q_ = 1.0f;
        float freq_ = 1000.0f;
        float sample_rate_ = 48000.0f;

        float max_freq_ = 20000.0f;

        float x1_ = 0.0f;
        float x2_ = 0.0f;
        float y1_ = 0.0f;
        float y2_ = 0.0f;

        float b0_ = 0.0f;
        float b1_ = 0.0f;
        float b2_ = 0.0f;
        float a1_ = 0.0f;
        float a2_ = 0.0f;
    };
} // namespace sknight::filters