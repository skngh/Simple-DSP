# DSP Library

By Sam Knight

A small header-only C++ library of audio DSP building blocks, which I'm somewhat regularly updating. Primarily written for the daisy seed but works

Most of the filters and reverb's algorithm I adapted from Will Pirkle's _Designing Audio Effect Plug-ins in C++_.

## Layout

- [`dsp/`](dsp) — audio processing classes (`sknight::dsp` namespace)
- [`utilities/`](utilities) — small helpers (`sknight::utilities` namespace)

## Usage pattern

Every class follows the same lifecycle:

```cpp
sknight::dsp::OnePole filter;
filter.Init(sample_rate);     // one time setup
filter.SetCutoff(1000.0f);

// ...in the audio callback...
float out = filter.Process(in);

// if you need to clear internal state (doesn't reset set parameters):
filter.Reset();
```

## Classes

### dsp/

- **[`DelayLine<MAX_SIZE>`](dsp/DelayLine.h)** — fixed-size circular delay buffer with linear-interpolated fractional reads.
- **[`TappedDelayLine<MAX_SIZE, NUM_TAPS>`](dsp/TappedDelayLine.h)** — a `DelayLine` read at multiple tap points with independent gains, summed into one output (used in Moorers Reverb below).
- **[`APF`](dsp/APF.h)** — single-sample first-order allpass filter (Schroeder allpass).
- **[`DelayingAllPass<MAX_SIZE>`](dsp/DelayingAllPass.h)** — Schroeder allpass filter built around a `DelayLine`, for reverb diffusion.
- **[`Comb<MAX_SIZE>`](dsp/Comb.h)** — basic feedback comb filter.
- **[`LPFComb<MAX_SIZE>`](dsp/LPFComb.h)** — comb filter with a one-pole lowpass in the feedback path, for damped reverb tails.
- **[`MoorerReverb`](dsp/MoorerReverb.h)** — James Moorer's early-reflections + parallel-comb/allpass reverb.
- **[`OnePole`](dsp/OnePole.h)** — one-pole low/high-pass filter.
- **[`BitCrusher`](dsp/BitCrusher.h)** — sample-rate reduction and bit-depth quantization.
- **[`SimpleDistortion`](dsp/SimpleDistortion.h)** — simple hard-clip and cubic soft-clip waveshaping distortion.
- **[`PitchShifter<MAX_SIZE>`](dsp/PitchShifter.h)** — delay-line based pitch shifter using two crossfaded, triangular-windowed read taps.

### utilities/

- **[`Utilities.h`](utilities/Utilities.h)** — misc helpers (pretty empty at the moment)

## Notes

- Let me know if you see any issues!
