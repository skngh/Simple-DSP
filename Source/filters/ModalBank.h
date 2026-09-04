#pragma once

#include "Biquad.h"

namespace sknight::filters
{
    template<int SIZE>
    class ModalBank final
    {
    public:
        struct BiquadParams
        {
            float frequency_ = 1000.0f;
            float q_ = 1.0f;
            float t60_ = 1.0f;
            float gain_ = 1.0f;
        };

        void Init(const float sample_rate)
        {
            sample_rate_ = sample_rate;
        }

        [[nodiscard]] float Process(const float in) noexcept
        {
            float out = 0.0f;
            for (int i = 0; i < SIZE; ++i)
            {
                out += biquad_[i].Process(in) * biquad_[i].gain_;
            }
            return out;
        }

        void SetParams(const BiquadParams& params, const int index)
        {
            biquad_[index].SetParams(params.frequency_, params.q_);
            biquad_[index].gain_ = params.gain_;
            SetScaleAmount();
        }

        void SetParamsT60(const BiquadParams& params, const int index)
        {
            biquad_[index].SetParams(params.frequency_, params.t60_);
            biquad_[index].gain_ = params.gain_;
            SetScaleAmount();
        }
    private:
        void SetScaleAmount()
        {
            float sum = 0.0f;
            for (int i = 0; i < SIZE; ++i)
                sum += biquad_[i].gain_;
            scale_amount_ = 1.0f / sqrt(sum);
            // scale_amount_ = 1.0f / sqrt(SIZE);
        }
        float sample_rate_ = 48000.0f;
        float scale_amount_ = 1.0f;
        Biquad biquad_[SIZE];
    };
} //namespace sknight::filters