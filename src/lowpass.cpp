//
// Created by Jakub on 23-Apr-22.
//

#include "../inc/lowpass.h"

#define M_PI           3.14159265358979323846

void lowpass::calcCutoffFrequency() {

    // fc = 1 / 2pi*RC
    this->cutoffFrequency = 1 / (2*M_PI*this->timeConstant);
}

void lowpass::calcDeltaT() {

    // deltaT = 1 / fs
    this->deltaT = 1 / (this->samplingFrequency);
}

void lowpass::calcAlpha() {
    calcCutoffFrequency();
    calcDeltaT();

    // alpha = 2*pi*deltaT*fc / (2*pi*deltaT*fc + 1)
    this->alpha = (2*M_PI*this->deltaT*this->cutoffFrequency) / (2*M_PI*this->deltaT*this->cutoffFrequency+1);
}

void lowpass::applyFilter() {
    int16_t counter = 1;

    this->calcAlpha();
    // y[n] = y[n-1] + a(x[n] - y[n-1])
    // y[k] = 0 for all negative k
    this->outputBuffer[0] = 0 + this->alpha*(this->inputBuffer[0] - 0);
    for(counter; counter < this->sampleNum; counter++) {
        this->outputBuffer[counter] = this->outputBuffer[counter-1] + this->alpha*(this->inputBuffer[counter] - this->outputBuffer[counter-1]);
    }
}

void lowpass::setInput(int32_t *input) {
    this->inputBuffer = input;
}

void lowpass::setOutput(int32_t *output) {
    this->outputBuffer = output;
}