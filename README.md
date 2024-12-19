# CardioVision AI Band

Wearable ECG band providing real-time cardiac monitoring, Pan-Tompkins QRS wave detection, and early detection of arrhythmias (tachycardia, bradycardia, premature ventricular beats) directly on edge hardware.

## Features

- **250 Hz Continuous ECG Sampling**: Accurately captures high-frequency components of the cardiac cycle.
- **Embedded Pan-Tompkins Filter**: Real-time bandpass filtering, differentiation, squaring, and moving window integration implemented in C++.
- **Lead-Off Failsafe**: Automatic hardware notification when skin contact with the 3 gel electrodes is compromised.
- **Lightweight Metrics**: Computes instantaneous RR intervals, Heart Rate Variability (HRV), and flags abnormal rhythms.

## Hardware Wiring

| AD8232 Pin | ESP32 Pin | Function |
| --- | --- | --- |
| OUTPUT | GPIO 36 (VP / ADC1_0) | Filtered analog ECG signal |
| LO+    | GPIO 32 | Lead-off positive detector |
| LO-    | GPIO 33 | Lead-off negative detector |
| 3.3V   | 3V3     | Clean regulated power supply |
| GND    | GND     | Common ground |

## Quickstart

1. Flash `CardioVisionBand.ino` to an ESP32 using Arduino IDE.
2. Open Serial Plotter at 115200 baud to visualize real-time QRS spikes.
3. Attach standard 3-electrode configuration (Right Arm, Left Arm, Right Leg ground).
