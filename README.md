# DSP Library

By Sam Knight

A small header-only C++ library of audio DSP building blocks, which I'm somewhat regularly updating. Primarily written for the daisy seed but works

Most of the filters and reverb's algorithm I adapted from Will Pirkle's _Designing Audio Effect Plug-ins in C++_.

## Layout

- [`filters/`](/Source/filters/) — filters (`sknight::filters` namespace)
- [`effects/`](/Source/effects/) — dsp building blocks + some full effects (`sknight::effects` namespace)
- [`generators/`](/Source/generators/) — signal generators (`sknight::generators` namespace)
- [`utilities/`](/Source/utilities/) — small helpers (`sknight::utilities` namespace)

## Usage pattern

Every class follows the same lifecycle:

```cpp
sknight::filters::OnePole filter;
filter.Init(sample_rate);     // one time setup
filter.SetCutoff(1000.0f);

// ...in the audio callback...
float out = filter.Process(in);

// if you need to clear internal state (doesn't reset set parameters):
filter.Reset();
```

## Classes

### filters/

- **[`OnePole`](Source/filters/OnePole.h)** — one-pole low/high-pass filter.
- **[`Biquad`](Source/filters/Biquad.h)** — biquad bandpass filter.
- **[`APF`](Source/filters/APF.h)** — single-sample first-order allpass filter (Schroeder allpass).
- **[`DelayingAllPass<MAX_SIZE>`](Source/filters/DelayingAllPass.h)** — Schroeder allpass filter built around a `DelayLine`, for reverb diffusion.
- **[`Comb<MAX_SIZE>`](Source/filters/Comb.h)** — basic feedback comb filter.
- **[`LPFComb<MAX_SIZE>`](Source/filters/LPFComb.h)** — comb filter with a one-pole lowpass in the feedback path, for damped reverb tails.

### effects/

- **[`DelayLine<MAX_SIZE>`](Source/effects/DelayLine.h)** — fixed-size circular delay buffer with linear-interpolated fractional reads.
- **[`TappedDelayLine<MAX_SIZE, NUM_TAPS>`](Source/effects/TappedDelayLine.h)** — a `DelayLine` read at multiple tap points with independent gains, summed into one output (used in Moorers Reverb below).
- **[`MoorerReverb`](Source/effects/MoorerReverb.h)** — James Moorer's early-reflections + parallel-comb/allpass reverb.
- **[`BitCrusher`](Source/effects/BitCrusher.h)** — sample-rate reduction and bit-depth quantization.
- **[`SimpleDistortion`](Source/effects/SimpleDistortion.h)** — simple hard-clip and cubic soft-clip waveshaping distortion.
- **[`PitchShifter<MAX_SIZE>`](Source/effects/PitchShifter.h)** — delay-line based pitch shifter using two crossfaded, triangular-windowed read taps.

### generators/

- **[`WhiteNoise`](Source/generators/WhiteNoise.h)** — fast white noise generator.
- **[`Oscillator`](Source/generators/Oscillator.h)** — sin/triangle/square/sawtooth wave oscillator.

### utilities/

- **[`ADSR.h`](Source/utilities/ADSR.h)** — simple adsr enveloper. release is automatically triggered after decay time.
- **[`Utilities.h`](Source/utilities/Utilities.h)** — misc helpers (pretty empty at the moment).

## Notes

- Let me know if you see any issues!
