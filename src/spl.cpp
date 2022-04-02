// audioprobe.cpp
#include <iostream>
#include "RtAudio.h"
#include "all.hpp"

#define __LINUX_ALSA__
#define __OS_LINUX__
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
int record(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *userData) {
    auto t1 = high_resolution_clock::now();
    int32_t *iBuf = (int32_t*) inputBuffer;
    if(status)
        std::cout << "Stream overflow detected!" << std::endl;
    std::cout << "######" << std::endl;
    std::cout << "Input buffer contents: " << std::endl;
    for(int i = 0; i < 256; i++) {
        std::cout << "Index: " << i << " :" << iBuf[i] << std::endl;
    }
    std::cout << "######" << std::endl;
    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);
    std::cout << ms_int.count() << "ms\n";
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
