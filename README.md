# DSP Library

By Sam Knight

A small header-only C++ library of audio DSP building blocks, which I'm somewhat regularly updating. Primarily written for the daisy seed but works

Most of the filters and reverb's algorithm I adapted from Will Pirkle's _Designing Audio Effect Plug-ins in C++_.

## Layout

- `dsp/` — audio processing classes (`sknight::dsp` namespace)
- `utilities/` — small helpers (`sknight::utilities` namespace)

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

- **`DelayLine<MAX_SIZE>`** — fixed-size circular delay buffer with linear-interpolated fractional reads.
- **`TappedDelayLine<MAX_SIZE, NUM_TAPS>`** — a `DelayLine` read at multiple tap points with independent gains, summed into one output (used in Moorers Reverb below).
- **`APF`** — single-sample first-order allpass filter (Schroeder allpass).
- **`DelayingAllPass<MAX_SIZE>`** — Schroeder allpass filter built around a `DelayLine`, for reverb diffusion.
- **`Comb<MAX_SIZE>`** — basic feedback comb filter.
- **`LPFComb<MAX_SIZE>`** — comb filter with a one-pole lowpass in the feedback path, for damped reverb tails.
- **`MoorerReverb`** — James Moorer's early-reflections + parallel-comb/allpass reverb.
- **`OnePole`** — one-pole low/high-pass filter.
- **`BitCrusher`** — sample-rate reduction and bit-depth quantization.
- **`SimpleDistortion`** — simple hard-clip and cubic soft-clip waveshaping distortion.
- **`PitchShifter<MAX_SIZE>`** — delay-line based pitch shifter using two crossfaded, triangular-windowed read taps.

### utilities/

- **`Utilities.h`** — misc helpers (pretty empty at the moment)

## Notes

- Let me know if you see any issues!
