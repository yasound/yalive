//
//  Broadcaster.h
//  YasoundLiveBroadcast
//
//  Created by matthieu campion on 5/7/12.
//  Copyright (c) 2012 MXP4. All rights reserved.
//

#pragma once

#import <CoreAudio/CoreAudio.h>
#include <vector>
#include <lame/lame.h>
#include <curl/curl.h>

class Broadcaster 
{    
public:
    Broadcaster();
    virtual ~Broadcaster();
    
private:
    bool Init();
    bool InitLame();
    bool InitCurl();
    bool InitAudioDevice();
    
    void Clean();
    void CleanLame();
    void CleanCurl();
    void CleanAudioDevice();
    
    static OSStatus IOProc(AudioDeviceID inDevice, const AudioTimeStamp* inNow, const AudioBufferList* inInputData, const AudioTimeStamp* inInputTime,
                           AudioBufferList* outOutputData, const AudioTimeStamp* inOutputTime, void* inClientData);
    OSStatus Process(AudioDeviceID inDevice, const AudioTimeStamp* inNow, const AudioBufferList* inInputData, const AudioTimeStamp* inInputTime,
                     AudioBufferList* outOutputData, const AudioTimeStamp* inOutputTime);
    
    bool Broadcast(std::vector<std::vector<float> >& input, int bufferSize);
    
    AudioDeviceID mDeviceID;
    double mSampleRate;
    int mBufferSize;
    std::vector< std::vector<float> > mInputBuffers;
    
    lame_global_flags* mLameFlags;
    unsigned char* mpLameOutBuffer;
    int mLameOutBufferSize;
    
    CURL* mCurl;
};
