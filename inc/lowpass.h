//
// Created by Jakub on 23-Apr-22.
//

#ifndef SPLMETER_LOWPASS_H
#define SPLMETER_LOWPASS_H
#include <stdint.h>

class lowpass {
    private:
        int32_t samplingFrequency;
        int32_t timeConstant;

        int32_t *inputBuffer;
        int32_t *outputBuffer;
        int32_t sampleNum;

        int32_t alpha;
        int32_t deltaT;
        int32_t cutoffFrequency;
    public:
        lowpass(int samplingFrequency, double timeConstant, int32_t* inputBuffer, int32_t* outputBuffer, int32_t sampleNum) {
            this->samplingFrequency = samplingFrequency;
            this->timeConstant = timeConstant;
            this->inputBuffer = inputBuffer;
            this->outputBuffer = outputBuffer;
            this->sampleNum = sampleNum;
    }
    void calcCutoffFrequency();
    void calcDeltaT();
    void calcAlpha();

    void setInput(int32_t *input);
    void setOutput(int32_t *output);

    void applyFilter();
};


#endif //SPLMETER_LOWPASS_H
