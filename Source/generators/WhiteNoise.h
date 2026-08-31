#pragma once

#include <cstdint>

// algorithm from https://www.musicdsp.org/en/latest/Synthesis/216-fast-whitenoise-generator.html

namespace sknight::generators
{
    class WhiteNoise final
    {
    public:
        void Init() { Reset(); };

        void Reset()
        {
            x1_ = 0x67452301;
            x2_ = 0xefcdab89;
        };

        [[nodiscard]] float Process(const float in)
        {
            x1_ ^= x2_;
            float out = x2_ * kScale - 1.0f;
            x2_ += x1_;
            return out;
        }

    private:
        static constexpr float kScale = 2.0f / 4294967295.0f;
        uint32_t x1_ = 0x67452301u;
        uint32_t x2_ = 0xefcdab89u;
    };
} // namespace sknight::generators