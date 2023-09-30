#include "hall-effect.h"

#include <Arduino.h>

#include "constants.h"

HallEffect* instance;

void HallEffect::setup() {
    instance = this;
    auto callback = []() { instance->interrupt(); };
    attachInterrupt(constants::INTERRUPT_PIN, callback, RISING);
}

void HallEffect::loop() {
    unsigned long start = lastReading;
    unsigned long stop = millis();
    if (stop - start > constants::SAMPLE_PERIOD) {
        float rpm = (count * 60000.0) / (stop - start);
        this->callback(rpm);
        // reset variables
        count = 0;
        lastReading = stop;
    }
}

void HallEffect::interrupt() { count++; }

void HallEffect::onData(void (*callback)(float)) { this->callback = callback; }