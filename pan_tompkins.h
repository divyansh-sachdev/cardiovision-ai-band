#ifndef PAN_TOMPKINS_H
#define PAN_TOMPKINS_H

#include <Arduino.h>

class QRSDetector {
private:
    float threshold;
    uint32_t lastPeakTime;
    float peakBPM;
public:
    QRSDetector() : threshold(500.0f), lastPeakTime(0), peakBPM(0.0f) {}

    bool processSample(int rawValue, uint32_t currentTimeMs, float &outBPM) {
        // High-pass derivative filter approximation
        static int prev = 0;
        int diff = rawValue - prev;
        prev = rawValue;
        
        int squared = diff * diff;
        
        if (squared > threshold && (currentTimeMs - lastPeakTime) > 300) {
            uint32_t rrInterval = currentTimeMs - lastPeakTime;
            lastPeakTime = currentTimeMs;
            if (rrInterval > 0) {
                peakBPM = 60000.0f / (float)rrInterval;
                outBPM = peakBPM;
                return true; // QRS peak detected
            }
        }
        return false;
    }
};

#endif
