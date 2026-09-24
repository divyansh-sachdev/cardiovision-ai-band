<div align="center">

# CardioVision AI Band

**Wearable ECG band with real-time on-device QRS detection and arrhythmia flagging**

![Domain](https://img.shields.io/badge/Domain-Biomedical_Signal_Processing-00F3FF?style=for-the-badge) ![Platform](https://img.shields.io/badge/Platform-ESP32_Edge-9D00FF?style=for-the-badge) ![Sampling](https://img.shields.io/badge/Sampling-250_Hz-0066FF?style=for-the-badge)

![ESP32](https://img.shields.io/badge/ESP32-0D1117?style=flat-square&logo=espressif&logoColor=white) ![Arduino](https://img.shields.io/badge/Arduino-0D1117?style=flat-square&logo=arduino&logoColor=white) ![C++](https://img.shields.io/badge/C++-0D1117?style=flat-square&logo=cplusplus&logoColor=white) ![AD8232](https://img.shields.io/badge/AD8232-0D1117?style=flat-square) ![Real--Time_DSP](https://img.shields.io/badge/Real--Time_DSP-0D1117?style=flat-square)

</div>

---

## Overview

A wrist-worn single-lead ECG monitor that runs its entire detection stack on the microcontroller — no
phone, no cloud, no round-trip. The AD8232 analog front end is sampled continuously at 250 Hz, and an
embedded QRS detector extracts R-peaks in real time to derive instantaneous heart rate and RR-interval
variability, flagging tachycardia and bradycardia as they happen.

Everything below runs inside the ESP32's main loop with a fixed memory footprint, which is what makes
it viable as a battery-powered wearable rather than a tethered desktop demo.

## Domain &amp; Techniques

| Layer | Implementation |
| :--- | :--- |
| **Acquisition** | AD8232 instrumentation amplifier sampled at 250 Hz on ADC1, sufficient to resolve the QRS complex (~10&ndash;25 Hz band) |
| **R-Peak Detection** | Derivative filter &rarr; squaring &rarr; adaptive amplitude threshold with a 300 ms refractory window, the core of the Pan&ndash;Tompkins method |
| **Rate Derivation** | RR interval between consecutive R-peaks converted to instantaneous BPM (`60000 / RR_ms`) |
| **Rhythm Flagging** | Threshold classification of derived BPM into bradycardia / normal / tachycardia bands |
| **Signal Integrity** | AD8232 lead-off comparators (LO+/LO&minus;) gate the pipeline so detached electrodes never produce phantom beats |

## Pipeline

```
AD8232 analog front end
        |
        v  250 Hz ADC sampling
  [ derivative filter ]      d/dt  — emphasises the steep QRS slope
        |
        v
  [ squaring ]               nonlinear amplification of the R-peak
        |
        v
  [ adaptive threshold ]     + 300 ms refractory lockout
        |
        v
  R-peak timestamps --> RR interval --> BPM --> rhythm flag
```

## Hardware Wiring

| AD8232 Pin | ESP32 Pin | Function |
| --- | --- | --- |
| OUTPUT | GPIO 36 (VP / ADC1_0) | Filtered analog ECG signal |
| LO+    | GPIO 32 | Lead-off positive detector |
| LO-    | GPIO 33 | Lead-off negative detector |
| 3.3V   | 3V3     | Clean regulated power supply |
| GND    | GND     | Common ground |

## Quickstart

1. Flash `CardioVisionBand.ino` to an ESP32 using the Arduino IDE.
2. Open the Serial Plotter at 115200 baud to visualise real-time QRS spikes.
3. Attach a standard 3-electrode configuration (Right Arm, Left Arm, Right Leg ground).

## Repository Layout

| Path | Purpose |
| :--- | :--- |
| `CardioVisionBand.ino` | Main firmware — sampling loop, rhythm flagging, serial output |
| `pan_tompkins.h` | `QRSDetector` class: derivative, squaring, adaptive threshold, RR &rarr; BPM |
| `config.example.h` | Tunable thresholds — copy to `config.h` before building |

## Project Status

**Implemented:** continuous 250 Hz acquisition, derivative/squaring/threshold R-peak detection,
RR-interval heart rate, lead-off gating.

**Roadmap:** the full Pan&ndash;Tompkins chain adds a 5&ndash;15 Hz bandpass stage and a 150 ms
moving-window integrator ahead of the threshold — both are planned to improve robustness under motion
artifact. HRV metrics (SDNN, RMSSD) and PVC morphology classification follow from there.

> Research prototype for engineering and competition work. Not a certified medical device and not a
> substitute for clinical assessment.

---

<div align="center">
  <sub>
    Part of the <b>AI + Robotics</b> engineering portfolio of
    <a href="https://github.com/divyansh-sachdev">Divyansh Sachdev</a><br>
    90+ national &amp; international competition wins &middot; IIT / NIT / IIIT podiums
  </sub>
</div>
