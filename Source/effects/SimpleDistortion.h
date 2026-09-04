#pragma once

namespace sknight::effects
{
    // SimpleDistortion: simple hard-clip and cubic soft-clip waveshaping distortion.
    //
    //   SetType(type)
    //   SetGain(val)
    class SimpleDistortion final
    {
    public:
        enum class ClippingType
        {
            HardClip,
            SoftClip
        };

        /** process distortion */
        [[nodiscard]] float Process(const float in) const noexcept
        {
            return clipping_type_ == ClippingType::HardClip ? HardClip(in)
                                                            : SoftClip(in);
        }

        /** set clipping type */
        void SetType (const ClippingType type) { clipping_type_ = type; }

        /** set drive gain */
        void SetGain(const float val) { gain_ = val; }

    private:
        float HardClip(float in) const noexcept
        {
            in *= gain_;
            return in > 1.0f ? 1.0f : in < -1.0f ? -1.0f
                                                 : in;
        }
        float SoftClip(float in) const noexcept
        {
            in *= gain_;
            return in >= 1.0f    ? 2.0f / 3.0f
                   : in <= -1.0f ? -2.0f / 3.0f
                                 : in - ((in * in * in) / 3.0f);
        }

        ClippingType clipping_type_ = ClippingType::HardClip;
        float gain_ = 1.0f;
    };
} // namespace sknight::effects
