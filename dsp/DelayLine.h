#pragma once

#include <cassert>

namespace sknight
{
template <int MAX_SIZE>
class DelayLine final
{
  public:
    DelayLine() {}
    ~DelayLine() {}

    void Init()
    {
        for(int i = 0; i < MAX_SIZE; ++i)
        {
            buffer_[i] = 0.0f;
        }
        write_index_ = 0;
        delay_time_  = 1.0f;
    }

    [[nodiscard]] float Read() const noexcept
    {
        int   delay_time_int = static_cast<int>(delay_time_);
        float frac           = delay_time_ - delay_time_int;
        int   read_index_a   = write_index_ - delay_time_int;
        if(read_index_a < 0)
            read_index_a += MAX_SIZE;
        int read_index_b = read_index_a - 1;
        if(read_index_b < 0)
            read_index_b += MAX_SIZE;

        return buffer_[read_index_a] * (1.0f - frac)
               + buffer_[read_index_b] * frac;
    }

    [[nodiscard]] float Read(const float offset) const noexcept
    {
        CheckDelay(offset);
        int   delay_time_int = static_cast<int>(offset);
        float frac           = offset - delay_time_int;
        int   read_index_a   = write_index_ - delay_time_int;
        if(read_index_a < 0)
            read_index_a += MAX_SIZE;
        int read_index_b = read_index_a - 1;
        if(read_index_b < 0)
            read_index_b += MAX_SIZE;

        return buffer_[read_index_a] * (1.0f - frac)
               + buffer_[read_index_b] * frac;
    }

    void Write(const float in) noexcept
    {
        buffer_[write_index_++] = in;
        if(write_index_ >= MAX_SIZE)
            write_index_ -= MAX_SIZE;
    }

    [[nodiscard]] float Process(const float in) noexcept
    {
        Write(in);
        return Read();
    }

    // in samples
    void SetDelay(const float delay)
    {
        CheckDelay(delay);
        delay_time_ = delay;
    }


  private:
    void CheckDelay(float delay) const noexcept
    { assert(delay >= 0.0f && delay <= MAX_SIZE - 2.0f); }

    int   write_index_ = 0;
    float delay_time_  = 1.0f;
    float buffer_[MAX_SIZE];
};
} // namespace sknight