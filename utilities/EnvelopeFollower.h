#pragma once

namespace sknight::utilities
{
class EnvelopeFollower final
{
  public:
    EnvelopeFollower() {};
    ~EnvelopeFollower() {};

    void Init(float sample_rate)
    {
        sample_rate_ = sample_rate;
        att          = 0.0f;
        last_sample_ = -1.0f;
        rel          = 0.0f;
    }

    [[nodiscard]] float Process(const float in) noexcept {}

    void SetAttackMs(const float val) {}

    void SetReleaseMs(const float val) {}

  private:
    float sample_rate_ = 48000.0f;
    float last_sample_ = -1.0f;
    float att          = 0.0f;
    float rel          = 0.0f;
};
} // namespace sknight::utilities