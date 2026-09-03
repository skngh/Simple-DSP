#pragma once

#include <cmath>

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
            a_ = 0.0f;
            d_ = 0.0f;
            r_ = 0.0f;
            stage_ = Stage::Idle;
            has_triggered_ = false;
        }

        /** process adsr. sends 0 when not triggered. (can check with IsGoing() */
        [[nodiscard]] float Process() noexcept
        {
            if (!has_triggered_) return 0.0f;
            float out = 0.0f;

            switch (stage_)
            {
                case Stage::Attack:
                    CalcEnvelope (a_, 1.0f, attack_);
                    out = a_;
                    if (a_ >= 1.0f) stage_ = Stage::Decay;
                    break;
                case Stage::Decay:
                    CalcEnvelope (d_, sustain_, decay_);
                    out = d_;
                    if (d_ >= sustain_) stage_ = Stage::Release;
                    break;
                case Stage::Release:
                    CalcEnvelope (r_, 0.0f, release_);
                    out = r_;
                    if (r_ <= 0.0f)
                    {
                        has_triggered_ = false;
                        stage_ = Stage::Idle;
                        out = 0.0f;
                    }
                    break;
                case Stage::Idle:
                    out = 0.0f;
                    break;
            }

            return out;
        }

        /** triggers adsr */
        void TriggerEnvelope()
        {
            Reset();

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

        /** set attack time
         * @param att in seconds
         */
        void SetAttack(const float att) { attack_ = GetCoeff(att); }
        /** set decay time
         * @param dec in seconds
         */
        void SetDecay(const float dec) { decay_ = GetCoeff(dec); }
        /** set sustain amount
         * @param sust in linear scale
         */
        void SetSustain(const float sust) { sustain_ = sust; }
        /** set release time
         * @param rel in seconds
         */
        void SetRelease(const float rel) { release_ = GetCoeff(rel); }

        /** returns whether adsr is running or not */
        [[nodiscard]] bool IsGoing() const noexcept { return has_triggered_; }
    private:
        static void CalcEnvelope(float& val, const float target, const float coeff)
        {
            val += coeff * (target - val);
        }

        [[nodiscard]] float GetCoeff(const float seconds) const
        {
            return std::exp(-1 / (seconds * sample_rate_));
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

        // envelope values
        float a_ = 0.0f;
        float d_ = 0.0f;
        float r_ = 0.0f;

        Stage stage_ = Stage::Idle;
        bool has_triggered_ = false;
    };
} // namespace sknight::utilities