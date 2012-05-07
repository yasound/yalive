//
//  Broadcaster.cpp
//  YasoundLiveBroadcast
//
//  Created by matthieu campion on 5/7/12.
//  Copyright (c) 2012 MXP4. All rights reserved.
//

#include "Broadcaster.h"
#include <iostream>
#include <AudioToolbox/AudioServices.h>
#include <stdio.h>

void OnLameError(const char *format, va_list ap);
void OnLameDebug(const char *format, va_list ap);
void OnLameMsg(const char *format, va_list ap);


class AudioDeviceState
{
public:
    AudioDeviceState()
    {
        mSampleRate = 0;
        mBufferSize = 0;
        mInputLatency = 0;
        mOutputLatency = 0;
    }
    
    bool Update(AudioDeviceID ID)
    {
        if (ID == 0)
            return false;
        
        Float64 sr;
        UInt32 size = sizeof(Float64);
        if (!AudioDeviceGetProperty(ID, 0, false, kAudioDevicePropertyNominalSampleRate, &size, &sr))
            mSampleRate = sr;
        
        UInt32 FPB;
        size = sizeof (FPB);
        if (!AudioDeviceGetProperty(ID, 0, false, kAudioDevicePropertyBufferFrameSize, &size, &FPB))
            mBufferSize = FPB;
        
        mInputLatency = 0;
        mOutputLatency = 0;
        UInt32 Latency;
        size = sizeof (UInt32);
        if (!AudioDeviceGetProperty(ID, 0, true, kAudioDevicePropertyLatency, &size, &Latency))
            mInputLatency = Latency;
        
        size = sizeof(UInt32);
        if (!AudioDeviceGetProperty(ID, 0, false, kAudioDevicePropertyLatency, &size, &Latency))
            mOutputLatency = Latency;
        
        return true;
    }
    
    double mSampleRate;
    int mBufferSize;
    int mInputLatency;
    int mOutputLatency;
};



Broadcaster::~Broadcaster()
{
    Clean();
}



Broadcaster::Broadcaster()
{
    mDeviceID = 0;
    mLameFlags = NULL;
    mCurl = NULL;
    bool res = Init();
    printf("Init res = %d", res);
}

bool Broadcaster::Init()
{        
    bool res = InitLame();
    if (!res)
        return false;
    res = InitCurl();
    if (!res)
        return false;
    res = InitAudioDevice();
    return res;
}

bool Broadcaster::InitLame()
{
    int inSampleRate = 44100;
    int outSamplerate = 24000;
    int BitRate = 64000;
    mLameFlags = lame_init();
    lame_set_in_samplerate(mLameFlags, inSampleRate);
    lame_set_num_channels(mLameFlags, 2);
    lame_set_scale(mLameFlags, 1.0f);
    lame_set_out_samplerate(mLameFlags, outSamplerate);
    lame_set_bWriteVbrTag(mLameFlags, 0);
    lame_set_quality(mLameFlags, 0);
    //lame_set_mode(mLameFlags, 0);
    lame_set_errorf(mLameFlags, OnLameError);
    lame_set_debugf(mLameFlags, OnLameDebug);
    lame_set_msgf(mLameFlags, OnLameMsg);
    lame_set_brate(mLameFlags, BitRate);
    //lame_set_asm_optimizations(mLameFlags, 1);
    lame_init_params(mLameFlags);
    lame_print_config(mLameFlags);
    
    mpLameOutBuffer = NULL;
    mLameOutBufferSize = 0;
    
    return true;
}

bool Broadcaster::InitCurl()
{
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != 0)
        return false;
    
    mCurl = curl_easy_init();
    if (!mCurl)
        return false;
    
    return true;
}

bool Broadcaster::InitAudioDevice()
{
    OSStatus error = 0;
    
    bool isInput = true;
    mDeviceID = 0;
    UInt32 size;
    AudioHardwareGetPropertyInfo(kAudioHardwarePropertyDefaultInputDevice, &size, NULL);
    AudioHardwareGetProperty(kAudioHardwarePropertyDefaultInputDevice, &size, &mDeviceID);
    
    // set sample rate only if it needs to change (changing causes disruption in audio of other applications)
    Float64 samplerate = 44100;
    Float64 currentSr;
    size = sizeof (samplerate);
    error = AudioDeviceGetProperty(mDeviceID, 0, isInput, kAudioDevicePropertyNominalSampleRate, &size, &currentSr);
    if (memcmp(&currentSr, &samplerate, size))
    {
        error = AudioDeviceSetProperty (mDeviceID, 0, 0, isInput, kAudioDevicePropertyNominalSampleRate, size, &samplerate);
    }
    
    // change buffer size
    UInt32 bufferSize = 1024;
    size = sizeof (bufferSize);
    
    error = AudioDeviceSetProperty (mDeviceID, 0, 0, isInput,  kAudioDevicePropertyBufferFrameSize, size, &bufferSize);
    
    // wait for the changes to happen (on some devices)
    AudioDeviceState state;
    int i = 30;
    while (--i >= 0)
    {
        state.Update(mDeviceID);
        
        if (samplerate == state.mSampleRate && bufferSize == state.mBufferSize)
            break;
        
        //        nglThread::MsSleep(100);
    }
    
    mSampleRate = state.mSampleRate;
    mBufferSize = state.mBufferSize;
    
    if (i < 0)
        return false;
    
    error = AudioDeviceAddIOProc(mDeviceID, IOProc, this);
    if (!error)
        error = AudioDeviceStart(mDeviceID, IOProc);
    return !error;
}


void Broadcaster::Clean()
{
    CleanAudioDevice();
    CleanLame();
    CleanCurl();
}

void Broadcaster::CleanLame()
{
    
}

void Broadcaster::CleanCurl()
{
    curl_easy_cleanup(mCurl);
    mCurl = NULL;
    curl_global_cleanup();
}

void Broadcaster::CleanAudioDevice()
{
    OSStatus error = AudioDeviceStop(mDeviceID, IOProc);
    if (!error)
        error = AudioDeviceRemoveIOProc (mDeviceID, IOProc);
}

OSStatus Broadcaster::IOProc(AudioDeviceID inDevice, const AudioTimeStamp* inNow, const AudioBufferList* inInputData, const AudioTimeStamp* inInputTime,
                                          AudioBufferList* outOutputData, const AudioTimeStamp* inOutputTime, void* inClientData)
{
    // Temporary thread priority boost...
    int policy;
    struct sched_param sched_original;
    struct sched_param sched;
    pthread_getschedparam(pthread_self(), &policy, &sched);
    pthread_getschedparam(pthread_self(), &policy, &sched_original);
    sched.sched_priority = sched.sched_priority + 50 > 120 ? 120 : sched.sched_priority +50;
    pthread_setschedparam(pthread_self(), policy, &sched);
    
    Broadcaster* pThis = (Broadcaster*)inClientData;
    assert(pThis);
    OSStatus status = pThis->Process(inDevice, inNow, inInputData, inInputTime, outOutputData, inOutputTime);
    
    // Temporary thread priority boost end...
    pthread_setschedparam(pthread_self(), policy, &sched_original);
    
    return status;
}

OSStatus Broadcaster::Process(AudioDeviceID inDevice, const AudioTimeStamp* inNow, const AudioBufferList* inInputData, const AudioTimeStamp* inInputTime,
                                           AudioBufferList* outOutputData, const AudioTimeStamp* inOutputTime)
{
    assert(inInputData->mNumberBuffers == 1);
    AudioBuffer buf = inInputData->mBuffers[0];
    mInputBuffers.resize(buf.mNumberChannels);
    int sampleFrames = buf.mDataByteSize / buf.mNumberChannels / sizeof(float);
    int channels = buf.mNumberChannels;
    for (int i = 0; i < channels; i++)
        mInputBuffers[i].resize(sampleFrames);
    
    float* pData = (float*)buf.mData;
    for (int s = 0; s < sampleFrames; s++) 
    {
        for (int c = 0; c < channels; c++)
            mInputBuffers[c][s] = pData[s * channels + c];
    }
    
    Broadcast(mInputBuffers, sampleFrames);
    
    return 0;
}



bool Broadcaster::Broadcast(std::vector<std::vector<float> >& input, int bufferSize)
{
    printf("input %d - %d - %d\n", input.size(), input[0].size(), bufferSize);
    
    unsigned long channels = input.size();
    
    for (int i = 0; i < bufferSize; i++)
    {
        for (int c = 0; c < channels; c++)
        {
            input[c][i] *= 32768;
        }
    }
    
    if (!mpLameOutBuffer)
    {
        mLameOutBufferSize = bufferSize;
        mpLameOutBuffer = new unsigned char[mLameOutBufferSize];
    }
    
    
    float* leftbuffer = &input[0][0];
    float* rightbuffer = &input[1][0];
    int res = lame_encode_buffer_float(mLameFlags, leftbuffer, rightbuffer, bufferSize, mpLameOutBuffer, mLameOutBufferSize);
    
    if (res > 0)
    {    
        FILE* pFile = fopen("/Users/mat/Desktop/converted.mp3", "a");
        if (pFile)
        {
            fwrite(mpLameOutBuffer, sizeof(unsigned char), mLameOutBufferSize, pFile);
        }
    }
    return true;
}









void OnLameError(const char *format, va_list ap)
{
    vprintf(format, ap); 
}

void OnLameDebug(const char *format, va_list ap)
{
    vprintf(format, ap); 
}

void OnLameMsg(const char *format, va_list ap)
{
    vprintf(format, ap); 
}