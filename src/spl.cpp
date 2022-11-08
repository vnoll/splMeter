#include <iostream>
#include <cstdlib>
#include <chrono>
#include <iterator>
#include <algorithm>
#include "RtAudio.h"
#include "all.hpp"
#include "lowpass.h"
#include "biquad.h"
#include "weightingCoefficients.h"
#include <alsa/asoundlib.h>

#define __LINUX_ALSA__
#define __OS_LINUX__
#define ALSA_PCM_NEW_HW_PARAMS_API
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

bool recordData = true;
auto currentTime = std::chrono::system_clock::now();

float rmsValue(int32_t arr[], int n)
{
    float square = 0;
    float mean = 0.0, root = 0.0;

    for (int i = 0; i < n; i++) {
        square += pow(arr[i], 2);
    }
    
    mean = (square / n);
    
    root = sqrt(mean);

    return root;
}

int record(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *userData) {

    if(recordData) {
        int32_t *iBuf = (int32_t*) inputBuffer;
        int32_t aBuf[256];
        memcpy(aBuf, iBuf, 256*sizeof(int32_t));
        int32_t cBuf[256];
        memcpy(cBuf, iBuf, 256*sizeof(int32_t));
        int32_t aFBuf[256];
        int32_t aSBuf[256];
        int32_t cFBuf[256];
        int32_t cSBuf[256];

        Biquad *aWeight1 = new  Biquad(A_WEIGHT_1_A0,  A_WEIGHT_1_A1,  A_WEIGHT_1_A2,  A_WEIGHT_1_B0,  A_WEIGHT_1_B1,  A_WEIGHT_1_B2);
        Biquad *aWeight2 = new  Biquad(A_WEIGHT_2_A0,  A_WEIGHT_2_A1,  A_WEIGHT_2_A2,  A_WEIGHT_2_B0,  A_WEIGHT_2_B1,  A_WEIGHT_2_B2);
        Biquad *aWeight3 = new  Biquad(A_WEIGHT_3_A0,  A_WEIGHT_3_A1,  A_WEIGHT_3_A2,  A_WEIGHT_3_B0,  A_WEIGHT_3_B1,  A_WEIGHT_3_B2);

        Biquad *cWeight1 = new  Biquad(C_WEIGHT_1_A0,  C_WEIGHT_1_A1,  C_WEIGHT_1_A2,  C_WEIGHT_1_B0, C_WEIGHT_1_B1,  C_WEIGHT_1_B2);
        Biquad *cWeight2 = new  Biquad(C_WEIGHT_2_A0,  C_WEIGHT_2_A1,  C_WEIGHT_2_A2,  C_WEIGHT_2_B0, C_WEIGHT_2_B1,  C_WEIGHT_2_B2);

       for(int i = 0; i < 256; i++) {
            aBuf[i] = aWeight3->filter(aWeight2->filter(aWeight1->filter(iBuf[i])));
            cBuf[i] = cWeight2->filter(cWeight1->filter(iBuf[i]));
        }

        aWeight3->resetZ();
        aWeight2->resetZ();
        aWeight1->resetZ();

        cWeight1->resetZ();
        cWeight2->resetZ();

        lowpass *lpSlow = new lowpass(44100, 1 , aBuf, aSBuf, 256);
        lowpass *lpFast = new lowpass(44100, 0.125, aBuf, aFBuf, 256);

        // apply time averaging filter (fast and slow) for A weighted signal
        lpSlow->applyFilter();
        lpFast->applyFilter();

        // set appropriate inputs and outputs for C weighted signal
        lpSlow->setInput(cBuf);
        lpSlow->setOutput(cSBuf);
        lpFast->setInput(cBuf);
        lpFast->setOutput(cFBuf);

        // apply time averaging filter (fast and slow) for C weighted signal
        lpSlow->applyFilter();
        lpFast->applyFilter();
        
        float rmsRaw = rmsValue(iBuf, 256);
        std::cout << "RMS of raw signal [10 factor]: " << 10*log10(rmsRaw) << std::endl;

        float rmsA = rmsValue(aBuf, 256);
        std::cout << "RMS of A signal [10 factor]: " << 10*log10(rmsA) << std::endl;
        float rmsAF = rmsValue(aFBuf, 256);
        std::cout << "RMS of A signal (fast) [10 factor]: " << 10*log10(rmsAF) << std::endl;
        float rmsAS = rmsValue(aSBuf, 256);
        std::cout << "RMS of A signal (slow) [10 factor]: " << 10*log10(rmsAS) << std::endl;

        float rmsC = rmsValue(cBuf, 256);
        std::cout << "RMS of C signal [10 factor]: " << 10*log10(rmsC) << std::endl;
        float rmsCF = rmsValue(cFBuf, 256);
        std::cout << "RMS of C signal (fast) [10 factor]: " << 10*log10(rmsCF) << std::endl;
        float rmsCS = rmsValue(cSBuf, 256);
        std::cout << "RMS of C signal (slow) [10 factor]: " << 10*log10(rmsCS) << std::endl;
        std::cout << "########" << std::endl;


        recordData = false;
        auto formattedTime = std::chrono::system_clock::to_time_t(currentTime); // convert it to time_t type (loses some precision)
        // std::cout << std::ctime(&formattedTime) << std::endl; // print it formatted
        currentTime = std::chrono::system_clock::now();
    }

    double elapsed_time_ms = std::chrono::duration<double, std::milli>(std::chrono::system_clock::now()-currentTime).count();

    if(elapsed_time_ms > 1000) {
        recordData = true;
    }
    return 0;
}

int main()
{ 
    RtAudio::StreamParameters sParam;
    // TODO: un-hardcode the device id
    sParam.deviceId = 2;
    sParam.nChannels = 1;
    sParam.firstChannel = 0;
    unsigned int sampleRate = 48000;
    unsigned int bufferSize = 256;
    RtAudio *audio = 0;
    
    try {
        audio = new RtAudio();
    }
    catch(RtAudioErrorType &error) {
        std::cout << error << std::endl;
        exit(EXIT_FAILURE);
    }
    try {
        audio->openStream(NULL, &sParam, RTAUDIO_SINT32,
                          sampleRate, &bufferSize, &record, NULL);
        audio->startStream();
    }
    catch(RtAudioErrorType &error) {
        std::cout << error << std::endl;
        exit(EXIT_FAILURE);
    }

    auto currentTime = std::chrono::system_clock::now(); // get the time
    auto formattedTime = std::chrono::system_clock::to_time_t(currentTime); // convert it to time_t type (loses some precision)
    std::cout << std::ctime(&formattedTime) << std::endl; // print it formatted

    char input;
    std::cout << "\n Recording ... press <enter> to quit. \n";
    std::cin.get(input);

    try{
        audio->stopStream();
    }
    catch(RtAudioErrorType &e) {
        std::cout << e << std::endl;
        exit(EXIT_FAILURE);
    }

    if(audio->isStreamOpen())  audio->closeStream();

    return 0; 
}
