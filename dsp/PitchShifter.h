#pragma once

#include <cmath>

namespace sknight
{
constexpr float PI = 3.1415927f;
template <size_t MAX_SIZE = 16384>
class PitchShifter final
{
  public:
    PitchShifter() {};
    ~PitchShifter() {};

    void Init()
    {
        write_ptr_ = 0;
        phase_     = 0.0f;
        phase_inc_ = 0.0f;

        for(size_t i = 0; i < MAX_SIZE; ++i)
            delay_line_[i] = 0.0f;
    }

    [[nodiscard]] float Process(const float in) noexcept
    {
        delay_line_[write_ptr_++] = in;
        if(write_ptr_ >= static_cast<int>(MAX_SIZE))
            write_ptr_ -= MAX_SIZE;

        phase_ += phase_inc_;
        while(phase_ >= 1.0f)
            phase_ -= 1.0f;
        while(phase_ < 0.0f)
            phase_ += 1.0f;

        float phase_b = phase_ + 0.5f;
        if(phase_b >= 1.0f)
            phase_b -= 1.0f;

        float read_a = write_ptr_ - phase_ * MAX_SIZE;
        float read_b = write_ptr_ - phase_b * MAX_SIZE;
        if(read_a < 0.0f)
            read_a += MAX_SIZE;
        if(read_b < 0.0f)
            read_b += MAX_SIZE;

        float sample_a = ReadInterpolate(read_a);
        float sample_b = ReadInterpolate(read_b);

        float tri_a = phase_ > 0.5f ? (1.0f - phase_) : phase_;
        float tri_b = phase_b > 0.5f ? (1.0f - phase_b) : phase_b;

        float gain_a = sinf(tri_a * sknight::PI);
        float gain_b = sinf(tri_b * sknight::PI);

        float sample = sample_a * gain_a + sample_b * gain_b;

        return sample;
    }

    void SetTransposition(const float transpose)
    {
        pitch_step_ = std::exp2f(transpose / 12.0f);
        phase_inc_  = (1.0f - pitch_step_) / MAX_SIZE;
    }

  private:
    inline float ReadInterpolate(float read_ptr) noexcept
    {
        int   read_idx_a = static_cast<int>(read_ptr);
        float frac       = read_ptr - read_idx_a;
        int   read_idx_b = read_idx_a + 1;

        if(read_idx_b >= static_cast<int>(MAX_SIZE))
            read_idx_b -= MAX_SIZE;

        return delay_line_[read_idx_a] * (1.0f - frac)
               + delay_line_[read_idx_b] * frac;
    }

    float phase_      = 0.0f;
    float phase_inc_  = 0.0f;
    float pitch_step_ = 1.0f;
    float delay_line_[MAX_SIZE];
    int   write_ptr_ = 0;
};
} // namespace sknight