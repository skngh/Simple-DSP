#pragma once

#include "Biquad.h"

namespace sknight::filters
{
    struct BiquadParams
    {
        float frequency_ = 1000.0f;
        float q_ = 1.0f;
        float t60_ = 1.0f;
        float gain_ = 1.0f;
    };
    template <int SIZE>
    class ModalBank final
    {
    public:
        void Init(const float sample_rate)
        {
            sample_rate_ = sample_rate;
            for (int i = 0; i < SIZE; ++i)
            {
                biquad_[i].Init(sample_rate);
            }
        }

        void Reset()
        {
            for (int i = 0; i < SIZE; ++i)
            {
                biquad_[i].Reset();
            }
        }

        [[nodiscard]] float Process(const float in) noexcept
        {
            float out = 0.0f;
            for (int i = 0; i < SIZE; ++i)
            {
                out += biquad_[i].Process(in) * gains_[i] * scale_amount_;
            }
            return out;
        }

        void SetParams(const BiquadParams &params, const int index)
        {
            biquad_[index].SetParams(params.frequency_, params.q_);
            gains_[index] = params.gain_;
            SetScaleAmount();
        }

        void SetParamsT60(const BiquadParams &params, const int index)
        {
            biquad_[index].SetParamsT60(params.frequency_, params.t60_);
            gains_[index] = params.gain_;
            SetScaleAmount();
        }

    private:
        void SetScaleAmount()
        {
            // float sum = 0.0f;
            // for (int i = 0; i < SIZE; ++i)
            //     sum += gains_[i];
            // scale_amount_ = 1.0f / sqrt(sum);
            scale_amount_ = 1.0f / sqrt(SIZE);
            // scale_amount_ = 1.0f / SIZE;
        }
        float sample_rate_ = 48000.0f;
        float scale_amount_ = 1.0f;
        float gains_[SIZE] = {1.0f};
        Biquad biquad_[SIZE];
    };
} // namespace sknight::filters