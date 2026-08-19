#pragma once

namespace sknight
{
class SimpleDistortion final
{
  public:
    enum class ClippingType
    {
        HardClip,
        SoftClip
    };

    SimpleDistortion() {};
    ~SimpleDistortion() {};

    void Init()
    {
        clipping_type_ = ClippingType::HardClip;
        gain_          = 1.0f;
    }

    [[nodiscard]] float Process(float in) noexcept
    {
        return clipping_type_ == ClippingType::HardClip ? HardClip(in)
                                                        : SoftClip(in);
    }

    void SetType(ClippingType type) { clipping_type_ = type; }

    void SetGain(float val) { gain_ = val; }

  private:
    float HardClip(float in) noexcept
    {
        in *= gain_;
        return in > 1.0f ? 1.0f : in < -1.0f ? -1.0f : in;
    }
    float SoftClip(float in) noexcept
    {
        in *= gain_;
        return in >= 1.0f    ? 2.0f / 3.0f
               : in <= -1.0f ? -2.0f / 3.0f
                             : in - ((in * in * in) / 3.0f);
    }

    ClippingType clipping_type_ = ClippingType::HardClip;
    float        gain_          = 1.0f;
};
} // namespace sknight