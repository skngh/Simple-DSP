#pragma once

#include <cassert>

namespace sknight::effects
{
    // DelayLine<MAX_SIZE>: fixed-size circular delay buffer with linear-interpolated fractional reads.
    //
    //   SetDelay(delay)
    template <int MAX_SIZE>
    class DelayLine final
    {
    public:
        /** initialize delayline */
        void Init()
        {
            delay_time_ = 1.0f;
            Reset();
        }

        /** reset delayline */
        void Reset()
        {
            for (int i = 0; i < MAX_SIZE; ++i)
            {
                buffer_[i] = 0.0f;
            }
            write_index_ = 0;
        }

        /** read at the current delay time */
        [[nodiscard]] float Read() const noexcept { return Read(delay_time_); }

        /**
         * read at an explicit offset
         * @param offset in samples
         */
        [[nodiscard]] float Read(const float offset) const noexcept
        {
            CheckDelay(offset);
            const int delay_time_int = static_cast<int>(offset);
            float frac = offset - delay_time_int;
            int read_index_a = write_index_ - delay_time_int;
            if (read_index_a < 0)
                read_index_a += MAX_SIZE;
            int read_index_b = read_index_a - 1;
            if (read_index_b < 0)
                read_index_b += MAX_SIZE;

            return buffer_[read_index_a] * (1.0f - frac) + buffer_[read_index_b] * frac;
        }

        /** write one sample into the buffer */
        void Write(const float in) noexcept
        {
            buffer_[write_index_++] = in;
            if (write_index_ >= MAX_SIZE)
                write_index_ -= MAX_SIZE;
        }

        /** process delayline */
        [[nodiscard]] float Process(const float in) noexcept
        {
            Write(in);
            return Read();
        }

        /**
         * set delay
         * @param delay in samples
         */
        void SetDelay(const float delay)
        {
            CheckDelay(delay);
            delay_time_ = delay;
        }

    private:
        static void CheckDelay(const float delay) noexcept
        {
            assert(delay >= 0.0f && delay <= MAX_SIZE - 2.0f);
        }

        int write_index_ = 0;
        float delay_time_ = 1.0f;
        float buffer_[MAX_SIZE];
    };
} // namespace sknight::effects
