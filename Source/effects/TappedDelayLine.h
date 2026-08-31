#pragma once

#include "DelayLine.h"
#include <array>

namespace sknight::effects
{
    // TappedDelayLine<MAX_SIZE, NUM_TAPS>: a DelayLine read at multiple tap points with independent gains,
    // summed into one output (used in MoorerReverb).
    //
    //   SetTaps(delay_times, gain_amounts)
    template <int MAX_SIZE, int NUM_TAPS>
    class TappedDelayLine final
    {
    public:
        /** initialize tappeddelayline */
        void Init()
        {
            delay_times_.fill(0.0f);
            gain_amounts_.fill(0.0f);
            Reset();
        }

        /** reset tappeddelayline */
        void Reset() { delay_line_.Reset(); }

        /**
         * set taps
         * @param delay_times in samples
         */
        void SetTaps(const std::array<float, NUM_TAPS> &delay_times,
                     const std::array<float, NUM_TAPS> &gain_amounts)
        {
            delay_times_ = delay_times;
            gain_amounts_ = gain_amounts;
        }

        /** process tappeddelayline */
        [[nodiscard]] float Process(const float in) noexcept
        {
            delay_line_.Write(in);

            float out = 0.0f;
            for (int i = 0; i < NUM_TAPS; ++i)
            {
                out += delay_line_.Read(delay_times_[i]) * gain_amounts_[i];
            }

            return out;
        }

    private:
        DelayLine<MAX_SIZE> delay_line_;
        std::array<float, NUM_TAPS> delay_times_;
        std::array<float, NUM_TAPS> gain_amounts_;
    };
} // namespace sknight::effects
