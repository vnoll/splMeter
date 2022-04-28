// audioprobe.cpp
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <iterator>
#include <algorithm>
#include "RtAudio.h"
#include "all.hpp"
#include "lowpass.h"

#define __LINUX_ALSA__
#define __OS_LINUX__
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

bool recordData = true;
auto currentTime = std::chrono::system_clock::now();

float rmsValue(int arr[], int n)
{
    int square = 0;
    float mean = 0.0, root = 0.0;

    // calculate square
    for (int i = 0; i < n; i++) {
        square += pow(arr[i], 2);
    }

    // calculate mean.
    mean = (square / (float)(n));

    // calculate root.
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

        // TODO: this has to be wrapped into some more elegant data structure

        float rmsRaw = rmsValue(iBuf, 256);
        std::cout << "RMS of raw signal [10 factor]: " << 10*log10(rmsRaw) << std::endl;

        for(int i = 0; i < 256; i++) {
            std::cout << "iBuf: " << iBuf[i] << std::endl;
            aBuf[i] = kfr::aweighting(*iBuf);
            cBuf[i] = kfr::cweighting(*iBuf);
            std::cout << "aBuf: " << aBuf[i] << std::endl;
            std::cout << "cBuf: " << cBuf[i] << std::endl;
        }
        // TODO: add time averaging
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
        
        recordData = false;
        auto formattedTime = std::chrono::system_clock::to_time_t(currentTime); // convert it to time_t type (loses some precision)
        // std::cout << std::ctime(&formattedTime) << std::endl; // print it formatted
        currentTime = std::chrono::system_clock::now();
    }

    double elapsed_time_ms = std::chrono::duration<double, std::milli>(std::chrono::system_clock::now()-currentTime).count();

    if(elapsed_time_ms > 1000) {
        recordData = true;
    }


    /* if(status)
        std::cout << "Stream overflow detected!" << std::endl;
    std::cout << "######" << std::endl;
    std::cout << "Input buffer contents: " << std::endl;
    for(int i = 0; i < 256; i++) {
        std::cout << "Index: " << i << " :" << iBuf[i] << std::endl;
    }
    std::cout << "######" << std::endl; */
    /* auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    std::cout << ms_int.count() << "ms\n"; */
    return 0;
}

int main()
{
    RtAudio::StreamParameters sParam;
    // TODO: un-hardcode the device id
    sParam.deviceId = 2;
    sParam.nChannels = 1;
    sParam.firstChannel = 1;
    unsigned int sampleRate = 44100;
    unsigned int bufferSize = 256;
    RtAudio *audio = 0;

    // TODO: clean up this mess
    try {
        audio = new RtAudio();
    }
    catch(RtAudioErrorType &error) {
        std::cout << error << std::endl;
        exit(EXIT_FAILURE);
    }

    try {
        audio->openStream(NULL, &sParam, RTAUDIO_SINT32,
                          sampleRate, &bufferSize, &record);
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
    /*
    RtAudio audio;
    // Determine the number of devices available
    unsigned int devices = audio.getDeviceCount();
    // Scan through devices for various capabilities
    RtAudio::DeviceInfo info;
    for ( unsigned int i=0; i<devices; i++ ) {
        info = audio.getDeviceInfo( i );
        if ( info.probed == true ) {
            // Print, for example, the maximum number of output channels for each device
            std::cout << "device = " << i;
            std::cout << ": maximum output channels = " << info.outputChannels << "\n";
        }
    }
    return 0; */
}
