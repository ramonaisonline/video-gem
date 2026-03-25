#ifndef LFO_H
#define LFO_H

#include <Arduino.h>

class LFO {
private:
    float phase = 0.0f;
    float frequency = 1.0f;
    unsigned long lastUpdate = 0;
    bool started = false;

    float applyOffset(float phaseOffset) {
        return fmod(phase + phaseOffset, TWO_PI) / TWO_PI;  // returns 0 to 1
    }

public:
    void setFrequency(float freq) {
        frequency = freq;
    }

    void update() {
        unsigned long currentTime = micros();
        if (!started) {
            lastUpdate = currentTime;
            started = true;
        }
        float deltaTime = (currentTime - lastUpdate) / 1000000.0f;
        lastUpdate = currentTime;
        phase = fmod(phase + TWO_PI * frequency * deltaTime, TWO_PI);
    }

    float getPhase() {
        return phase;
    }

    // --- Unipolar (0 to 1) waveforms ---

    float sine(float phaseOffset = 0.0f) {
        return (sin(phase + phaseOffset) + 1.0f) * 0.5f;
    }

    float triangle(float phaseOffset = 0.0f) {
        float p = applyOffset(phaseOffset);
        return 1.0f - fabs(2.0f * p - 1.0f);
    }

    float square(float phaseOffset = 0.0f) {
        float p = applyOffset(phaseOffset);
        return (p < 0.5f) ? 1.0f : 0.0f;
    }

    float saw(float phaseOffset = 0.0f) {
        return applyOffset(phaseOffset);
    }

    float ramp(float phaseOffset = 0.0f) {
        return 1.0f - applyOffset(phaseOffset);
    }

    // --- Bipolar (-1 to 1) waveforms ---

    float sineBipolar(float phaseOffset = 0.0f) {
        return sin(phase + phaseOffset);
    }

    float triangleBipolar(float phaseOffset = 0.0f) {
        float p = applyOffset(phaseOffset);
        return 1.0f - fabs(4.0f * p - 2.0f);  // -1 to 1
    }

    float squareBipolar(float phaseOffset = 0.0f) {
        float p = applyOffset(phaseOffset);
        return (p < 0.5f) ? 1.0f : -1.0f;
    }

    float sawBipolar(float phaseOffset = 0.0f) {
        return applyOffset(phaseOffset) * 2.0f - 1.0f;
    }

    float rampBipolar(float phaseOffset = 0.0f) {
        return 1.0f - applyOffset(phaseOffset) * 2.0f;
    }
};

#endif