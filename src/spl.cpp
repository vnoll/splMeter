#include <iostream>
#include <SPLConfig.h>
#include <portaudio.h>
#include "paRead.h"
#include <cstddef>

    PaStreamParameters      inputParameters;
    PaStream*               stream;
    PaError                 err = paNoError;
    paTestData              data;
    int                     i;
    int                     totalFrames;
    int                     numSamples;
    int                     numBytes;
    SAMPLE                  max, val;
    double                  average;

void terminatePaProcess() {
    Pa_Terminate();
    if(data.recordedSamples) {
        free(data.recordedSamples);
    }
    if(err != paNoError) {
        std::cout << "An error occured while using the PortAudio stream" << std::endl;
        std::cout << "Error number: " << err << std::endl;
        std::cout << "Error message: " << Pa_GetErrorText(err) << std::endl;
        err = 1;
    }
    abort();
}

int main(int argv, char* argc[]) {

    std::cout << "Version: " << SoundPressureLevel_VERSION_MAJOR << "." << SoundPressureLevel_VERSION_MINOR << "." << SoundPressureLevel_VERSION_PATCH << std::endl;



    data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE;
    std::cout << "Total frames: " << totalFrames << std::endl;
    data.frameIndex = 0;
    numSamples = totalFrames * NUM_CHANNELS;
    std::cout << "Number of samples: " << numSamples << std::endl;
    numBytes = numSamples * sizeof(SAMPLE);
    std::cout << "Number of bytes: " << numBytes << std::endl;
    data.recordedSamples = (SAMPLE *)malloc(numBytes);
    std::cout << "Size of allocated record array: " << sizeof(data.recordedSamples) << std::endl;
    if(data.recordedSamples == NULL) {
        std::cout << "Could not allocate record array." << std::endl;
        terminatePaProcess();
    }
    
    return 1;
}