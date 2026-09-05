#pragma once

#include <algorithm>
#include <cmath>

namespace sknight::effects
{
    // BitCrusher: sample-rate reduction and bit-depth quantization.
    //
    //   SetBitDepth(depth)
    //   SetSamplingRate(target_sr)
    class BitCrusher final
    {
    public:
        /** initialize bitcrusher */
        void Init(const float sample_rate)
        {
            ql_ = 0.0f;
            sample_rate_ = sample_rate;
            Reset();
        }

        /** reset bitcrusher */
        void Reset()
        {
            phase_ = 0.0f;
            last_sample_ = 0.0f;
        }

        /** process bitcrusher, hard clips signal to 1 & -1 */
        [[nodiscard]] float Process(const float in) noexcept
        {
            // downsample (sample & hold)
            float sig = in;
            phase_ += phase_step_;

            if (phase_ >= 1.0f)
            {
                last_sample_ = sig;
                phase_ -= 1.0f;
            }
            sig = last_sample_;

            // bit depth
            float crushed = ql_ * (std::floor(sig / ql_ + 0.5f));
            if (crushed > 1.0f)
                crushed = 1.0f;
            if (crushed < -1.0f)
                crushed = -1.0f;
            return crushed;
        }

        /**
         * set bit depth
         * @param depth 1-24 bits
         */
        void SetBitDepth(int depth)
        {
            depth = std::clamp(depth, 1, 24);
            ql_ = 2.0f / static_cast<float>(std::pow(2.0f, depth) - 1.0f);
        }

        /**
         * set downsampled rate
         * @param target_sr in hz
         */
        void SetSamplingRate(float target_sr)
        {
            target_sr = (std::min)(target_sr, sample_rate_);
            phase_step_ = target_sr / sample_rate_;
        }

    private:
        float ql_ = 0.0f;
        float sample_rate_ = 48000.0f;
        float last_sample_ = 0.0f;
        float phase_step_ = 1.0f;
        float phase_ = 0.0f;
    };
} // namespace sknight::effects
