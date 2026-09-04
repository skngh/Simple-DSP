#pragma once

#include <cmath>
#include "../utilities/Utilities.h"

namespace sknight::generators
{
    // sin/triangle/square/sawtooth wave oscillator
    //
    // SetType(generator::WaveType type)
    // SetFreq(frequency)
    class Oscillator final
    {
    public:
        enum class WaveType
        {
            SIN,
            SAW,
            SQUARE,
            TRI
        };

        void Init(const float sample_rate)
        {
            sample_rate_ = sample_rate;
            sr_rec_ = 1.0f / sample_rate_;
            Reset();
        }

        void Reset()
        {
            phase_ = 0.0f;
            CalcPhaseInc();
        }

        [[nodiscard]] float Process() noexcept
        {
            float out = 0.0f;
            const float gb = 2.0f * phase_ - 1.0f;

            switch (wave_type_)
            {
            case WaveType::SIN:
                out = std::sinf(phase_ * 2.0f * utilities::kPi);
                break;
            case WaveType::SAW:
                out = gb;
                break;
            case WaveType::SQUARE:
                out = phase_ > 0.5f ? 1.0f : -1.0f;
                break;
            case WaveType::TRI:
                out = 2.0f * (fabsf(gb) - 0.5f);
                break;
            }

            phase_ += phase_inc_;
            if (phase_ > 1.0f)
                phase_ -= 1.0f;

            return out;
        }

        /** set waveform type
         * @param type SIN SAW SQUARE TRI
         */
        void SetType(const WaveType type)
        {
            wave_type_ = type;
        }

        /** set frequency
         * @param freq freq in hz
         */
        void SetFreq(const float freq)
        {
            freq_ = freq;
            CalcPhaseInc();
        }

    private:
        void CalcPhaseInc()
        {
            phase_inc_ = freq_ * sr_rec_;
        }
        float sample_rate_ = 48000.0f;
        float sr_rec_ = 0.1f;
        WaveType wave_type_ = WaveType::SIN;

        float freq_ = 440.0f;

        float phase_ = 0.0f;
        float phase_inc_ = 0.0f;
    };
} // namespace sknight::generators