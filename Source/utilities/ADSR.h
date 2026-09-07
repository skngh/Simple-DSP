#pragma once

#include <cmath>
#include <algorithm>

namespace sknight::utilities
{
    // simple adsr enveloper. release is automatically triggered after decay time.
    // att value goes 0-1. Then decay time goes 1-${sustain}. Then release goes ${sustain} to 0
    //
    // TriggerEnvelope()
    // SetParams(float, float, float, float)
    // SetAttack(float)
    // SetDecay(float)
    // SetSustain(float)
    // SetRelease(float)

    class ADSR final
    {
    public:
        /** initiailize adsr */
        void Init(const float sample_rate)
        {
            sample_rate_ = sample_rate;
            Reset();
        }

        /** reset adsr */
        void Reset()
        {
            env_val_ = 0.0f;
            decay_counter_ = 0.0f;
            stage_ = Stage::Idle;
            has_triggered_ = false;
        }

        /** process adsr. sends 0 when not triggered. (can check with IsGoing() */
        [[nodiscard]] float Process() noexcept
        {
            if (!has_triggered_)
                return 0.0f;

            switch (stage_)
            {
            case Stage::Attack:
                Lerp(env_val_, 0.0f, 1.0f, attack_);
                if (env_val_ >= 1.0f)
                {
                    env_val_ = 1.0f;
                    stage_ = Stage::Decay;
                }
                break;
            case Stage::Decay:
                Lerp(env_val_, 1.0f, sustain_, decay_);
                Lerp(decay_counter_, 0.0f, 1.0f, decay_);
                if (env_val_ <= sustain_ && decay_counter_ >= 1.0f)
                {
                    env_val_ = sustain_;
                    stage_ = loop_until_trigger_ ? Stage::Decay : Stage::Release;
                }
                break;
            case Stage::Release:
                Lerp(env_val_, sustain_, 0.0f, release_);
                if (env_val_ <= 0.0f)
                {
                    env_val_ = 0.0f;
                    has_triggered_ = false;
                    stage_ = Stage::Idle;
                }
                break;
            case Stage::Idle:
                env_val_ = 0.0f;
                break;
            }

            return env_val_;
        }

        /** triggers adsr */
        void TriggerEnvelope(bool loop_until_trigger = false)
        {
            Reset();
            loop_until_trigger_ = loop_until_trigger;
            stage_ = Stage::Attack;
            has_triggered_ = true;
        }

        /** set all parameters at once
         * @param att attack time in seconds
         * @param dec decay time in seconds
         * @param sust sustain amount in linear scale
         * @param rel release time in seconds
         */
        void SetParams(const float att, const float dec, const float sust, const float rel)
        {
            attack_ = GetCoeff(att);
            decay_ = GetCoeff(dec);
            sustain_ = sust;
            release_ = GetCoeff(rel);
        }

        void TriggerRelease()
        {
            stage_ = Stage::Release;
        }

        /** set attack time
         * @param att in seconds
         */
        void SetAttack(const float att) { attack_ = GetCoeff(att); }
        /** set decay time
         * @param dec in seconds
         */
        void SetDecay(const float dec) { decay_ = GetCoeff(dec); }
        /** set sustain amount
         * @param sust in linear scale. clamped at 1
         */
        void SetSustain(const float sust) { sustain_ = (std::min)(sust, 1.0f); }
        /** set release time
         * @param rel in seconds
         */
        void SetRelease(const float rel) { release_ = GetCoeff(rel); }

        /** returns whether adsr is running or not */
        [[nodiscard]] bool IsGoing() const noexcept { return has_triggered_; }

    private:
        static void Lerp(float &val, const float start, const float target, const float coeff)
        {
            val += coeff * (target - start);
        }

        [[nodiscard]] float GetCoeff(const float seconds) const
        {
            return 1.0f - std::exp(-1 / (seconds * sample_rate_));
        }
        enum class Stage
        {
            Attack,
            Decay,
            Release,
            Idle
        };

        float sample_rate_ = 48000.0f;

        // coefficients
        float attack_ = 0.0f;
        float decay_ = 0.0f;
        float sustain_ = 0.0f; // lin
        float release_ = 0.0f;

        bool loop_until_trigger_ = false;

        float env_val_ = 0.0f;
        float decay_counter_ = 0.0f; // since decay should happen even is sustain is 1

        Stage stage_ = Stage::Idle;
        bool has_triggered_ = false;
    };
} // namespace sknight::utilities