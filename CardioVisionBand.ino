/**
 * CardioVision AI Band — Wearable ECG QRS & Arrhythmia Detector
 * Author: Divyansh Sachdev
 */

#include <Arduino.h>
#include "config.example.h"
#include "pan_tompkins.h"

QRSDetector detector;

void setup() {
    Serial.begin(115200);
    pinMode(LO_PLUS_PIN, INPUT);
    pinMode(LO_MINUS_PIN, INPUT);
    
    Serial.println("==================================================");
    Serial.println("  CardioVision AI Band — Wearable ECG Monitor");
    Serial.println("==================================================");
    Serial.println("[INFO] Initializing AD8232 Analog Front-End...");
    Serial.println("[STATUS] Sampling at 250Hz. Ready.");
}

void loop() {
    if (digitalRead(LO_PLUS_PIN) == HIGH || digitalRead(LO_MINUS_PIN) == HIGH) {
        Serial.println("[WARNING] Electrodes disconnected (Leads Off)!");
        delay(500);
        return;
    }

    int ecgRaw = analogRead(ECG_ANALOG_PIN);
    float currentBPM = 0.0f;
    uint32_t now = millis();

    if (detector.processSample(ecgRaw, now, currentBPM)) {
        Serial.printf("[BEAT] QRS Detected! Current BPM: %.1f\n", currentBPM);
        if (currentBPM > TACHYCARDIA_BPM) {
            Serial.println("[ALERT] Tachycardia rhythm detected!");
        } else if (currentBPM < BRADYCARDIA_BPM) {
            Serial.println("[ALERT] Bradycardia rhythm detected!");
        }
    }

    delay(4); // ~250 Hz sampling rate
}
