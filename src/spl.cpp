// audioprobe.cpp
#include <iostream>
#include "RtAudio.h"

#define __LINUX_ALSA__
#define __OS_LINUX__

int record(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *userData) {
    int32_t *iBuf = (int32_t*) inputBuffer;
    if(status)
        std::cout << "Stream overflow detected!" << std::endl;
    std::cout << "######" << std::endl;
    std::cout << "Input buffer contents: " << std::endl;
    for(int i = 0; i < 256; i++) {
        std::cout << "Index: " << i << " :" << iBuf[i] << std::endl;
    }
    std::cout << "######" << std::endl;
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
