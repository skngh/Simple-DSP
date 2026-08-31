#pragma once

#include <array>
#include <cmath>
#include "LPFComb.h"
#include "DelayingAllPass.h"
#include "TappedDelayLine.h"

namespace sknight::effects
{
    // MoorerReverb: James Moorer's early-reflections + parallel-comb/allpass reverb.
    //   I used this pdf I found online for the values https://freeverb3-vst.sourceforge.io/doc/Moorer_Reverb.PDF
    //
    //   SetDecayTime(time)
    //   SetERGain(gain)
    //   SetCombsGain(gain)
    //   SetDamping(damping)
    class MoorerReverb final
    {
    public:
        /** initialize moorerreverb */
        void Init(float sample_rate)
        {
            sample_rate_ = sample_rate;
            er_.SetTaps(er_delays(), er_gains());

            float er_power = 0.0f;
            for (std::size_t i = 0; i < er_gains().size(); ++i)
            {
                er_power += er_gains()[i] * er_gains()[i];
            }
            er_scale_ = 1.0f / std::sqrt(er_power);

            apf_.SetDelay(334.0f);

            for (std::size_t i = 0; i < combs_.size(); ++i)
            {
                combs_[i].SetDelay(comb_delays()[i]);
            }

            SetDecayTime(kDefaultDecayTime);

            Reset();
        }

        /** reset moorerreverb */
        void Reset()
        {
            er_.Reset();
            apf_.Reset();
            for (auto &comb : combs_)
            {
                comb.Reset();
            }
        }

        /** process moorerreverb */
        [[nodiscard]] float Process(const float in) noexcept
        {
            const float er_out = er_.Process(in) * er_scale_;

            float combs_out = 0.0f;
            for (std::size_t i = 0; i < combs_.size(); ++i)
            {
                combs_out += combs_[i].Process(er_out);
            }
            combs_out *= combs_scale_;

            const float apf_out = apf_.Process(combs_out);

            return (apf_out * combs_gain_) + (er_out * er_gain_);
        }

        /**
         * set decay time (rt60)
         * @param time in seconds
         */
        void SetDecayTime(const float time)
        {
            float combs_power = 0.0f;
            for (std::size_t i = 0; i < combs_.size(); ++i)
            {
                const float val = std::pow(
                    10.0f, (-3.0f * (comb_delays()[i] / sample_rate_)) / time);
                combs_[i].SetFeedback(val);
                combs_power += 1.0f / (1.0f - (val * val));
            }

            combs_scale_ = 1.0f / std::sqrt(combs_power);
        }

        /** set early reflections gain */
        void SetERGain(const float gain) { er_gain_ = gain; }

        /** set combs (tail) gain */
        void SetCombsGain(const float gain) { combs_gain_ = gain; }

        /**
         * set damping
         * @param damping 0-1
         */
        void SetDamping(const float damping)
        {
            for (std::size_t i = 0; i < combs_.size(); ++i)
            {
                combs_[i].SetDamping(damping);
            }
        }

    private:
        static constexpr float kDefaultDecayTime = 2.0f;

        // prime numbers
        static const std::array<float, 6> &comb_delays() noexcept
        {
            static constexpr std::array<float, 6> v = {1913, 2129, 2297, 2503, 2683, 2879};
            return v;
        }

        static const std::array<float, 18> &er_delays() noexcept
        {
            static constexpr std::array<float, 18> v = {207,
                                                        1033,
                                                        1081,
                                                        1288,
                                                        1297,
                                                        1431,
                                                        2200,
                                                        2329,
                                                        2747,
                                                        2819,
                                                        2857,
                                                        2939,
                                                        3395,
                                                        3399,
                                                        3485,
                                                        3557,
                                                        3615,
                                                        3826};
            return v;
        }

        static const std::array<float, 18> &er_gains() noexcept
        {
            static constexpr std::array<float, 18> v = {.841f,
                                                        .504f,
                                                        .490f,
                                                        .379f,
                                                        .380f,
                                                        .346f,
                                                        .289f,
                                                        .272f,
                                                        .192f,
                                                        .193f,
                                                        .217f,
                                                        .181f,
                                                        .180f,
                                                        .181f,
                                                        .176f,
                                                        .142f,
                                                        .167f,
                                                        .134f};
            return v;
        }

        DelayingAllPass<3400> apf_;
        std::array<LPFComb<3000>, 6> combs_ = {};
        TappedDelayLine<4000, 18> er_;

        float er_gain_ = 1.0f;
        float combs_gain_ = 1.0f;
        float er_scale_ = 1.0f;
        float combs_scale_ = 1.0f;
        float sample_rate_ = 48000.0f;
    };
} // namespace sknight::effects
