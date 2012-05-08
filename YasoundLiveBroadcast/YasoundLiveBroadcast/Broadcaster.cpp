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
#include "Mp3Header.h"

#ifdef WIN32
#include <WinSock2.h>
#include <Ws2tcpip.h>
#undef GetAddrInfo
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/ioctl.h>
#endif

#define PRODUCTION 0

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
  mSocket = -1;
  bool res = Init();
  printf("Init res = %d", res);
}

bool Broadcaster::Init()
{        
  bool res = InitLame();
  if (!res)
    return false;
  res = InitNetwork();
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


#ifdef WIN32
#define W(X) WSA##X
#else
#define W(X) X
#endif

void DumpError(int err)
{
  if (!err)
    return;
  
  const char* error = NULL;
  
  switch (err)
  {
    case EACCES: error = "The destination address is a broadcast address and the socket option SO_BROADCAST is not set."; break;
    case W(EADDRINUSE): error = "The address is already in use."; break;
    case W(EADDRNOTAVAIL): error = "The specified address is not available on this machine."; break;
    case W(EAFNOSUPPORT): error = "Addresses in the specified address family cannot be used with this socket."; break;
    case W(EALREADY): error = "The socket is non-blocking and a previous connection attempt has not yet been completed."; break;
    case W(EBADF): error = "socket is not a valid descriptor."; break;
    case W(ECONNREFUSED): error = "The attempt to connect was ignored (because the target is not listening for connections) or explicitly rejected."; break;
    case W(EFAULT): error = "The address parameter specifies an area outside the process address space."; break;
    case W(EHOSTUNREACH): error = "The target host cannot be reached (e.g., down, disconnected)."; break;
    case W(EINPROGRESS): error = "The socket is non-blocking and the connection cannot be completed immediately.  It is possible to select(2) for completion by selecting the socket for writing."; break;
    case W(EINTR): error = "Its execution was interrupted by a signal."; break;
    case W(EINVAL): error = "An invalid argument was detected (e.g., address_len is not valid for the address family, the specified address family is invalid)."; break;
    case W(EISCONN): error = "The socket is already connected."; break;
    case W(ENETDOWN): error = "The local network interface is not functioning."; break;
    case W(ENETUNREACH): error = "The network isn't reachable from this host."; break;
    case W(ENOBUFS): error = "The system call was unable to allocate a needed memory buffer."; break;
    case W(ENOTSOCK): error = "socket is not a file descriptor for a socket."; break;
    case W(EOPNOTSUPP): error = "Because socket is listening, no connection is allowed."; break;
    case W(EPROTOTYPE): error = "address has a different type than the socket that is bound to the specified peer address."; break;
    case W(ETIMEDOUT): error = " Connection establishment timed out without establishing a connection."; break;
    case W(ECONNRESET): error = "Remote host reset the connection request."; break;
    default: error = "Unknown error "; break;
  }
  
  printf("Socket Error: %s\n", error);
}




bool Broadcaster::InitNetwork()
{
  mSocket = socket(AF_INET, SOCK_STREAM, 0);

  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_CANONNAME;     /* AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST */
  hints.ai_family = PF_INET;    /* PF_xxx */
  hints.ai_socktype = 0;  /* SOCK_xxx */
  hints.ai_protocol = 0;  /* 0 or IPPROTO_xxx for IPv4 and IPv6 */
  
  hints.ai_socktype = SOCK_STREAM;  /* SOCK_xxx */
  hints.ai_protocol = IPPROTO_TCP;  /* 0 or IPPROTO_xxx for IPv4 and IPv6 */
  
  struct addrinfo* infos = NULL;

#if PRODUCTION
  const char* hostname = "yas-web-01.ig-1.net";
#else
  const char* hostname = "dev.yasound.com";
#endif
  
  int res = getaddrinfo(hostname, "8000", &hints, &infos);
  
  if (res != 0)
  {
    const char* err = NULL;
    switch (res)
    {
      case EAI_AGAIN:
        err = "temporary failure in name resolution EAI_BADFLAGS invalid value for ai_flags EAI_BADHINTS invalid value for hints";
        break;
      case EAI_FAIL:
        err = "non-recoverable failure in name resolution";
        break;
      case EAI_FAMILY:
        err = "ai_family not supported";
        break;
      case EAI_MEMORY:
        err = "memory allocation failure";
        break;
      case EAI_NONAME:
        err = "hostname or servname not provided, or not known EAI_OVERFLOW argument buffer overflow EAI_PROTOCOL resolved protocol is unknown";
        break;
      case EAI_SERVICE:
        err = "servname not supported for ai_socktype EAI_SOCKTYPE ai_socktype not supported";
        break;
#ifndef WIN32
      case EAI_SYSTEM:
        err = "system error returned in errno";
        break;
#endif
    }
    
    printf("Network resolve error: %s\n", err);
  }
  
  res = connect(mSocket, infos->ai_addr, infos->ai_addrlen);
  freeaddrinfo(infos);

  if (res != 0)
  {
    DumpError(errno);
    return false;
  }
  
#if PRODUCTION
  const char* radioid = "ce68c12adce8450fab688ac222740cdf";
#else
  const char* radioid = "ce68c12adce8450fab688ac222740cdf";
#endif
  
  std::string httpheader = "POST ";
  httpheader.append(radioid);
  httpheader.append(" HTTP/1.1\r\n\r\n");

  res = send(mSocket, httpheader.c_str(), httpheader.size(), 0);
  if (res != httpheader.size())
  {
    DumpError(errno);
    return false;
  }
  
  
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
  CleanNetwork();
}

void Broadcaster::CleanLame()
{
  
}

void Broadcaster::CleanNetwork()
{
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
  //printf("input %d - %d - %d\n", input.size(), input[0].size(), bufferSize);
  
  unsigned long channels = input.size();
  
  for (int i = 0; i < bufferSize; i++)
  {
    for (int c = 0; c < channels; c++)
    {
      if (input[c][i] >= 0)
        input[c][i] *= 32767;
      else
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
  
  FILE* pFile = fopen("/Users/meeloo/Desktop/live.mp3", "a");
  int size = res;
  int offset = 0;
  while (size > 0)
  {
    Mp3Header hdr(mpLameOutBuffer + offset);
    assert(hdr.IsValid());
    int s = hdr.GetFrameByteLength();
    res -= s;

    if (hdr.GetFrameDuration() > 0)
    {
      int done = send(mSocket, mpLameOutBuffer + offset, s, 0);
      if (done != s)
      {
        DumpError(errno);
        return false;
      }
      
      if (pFile)
        fwrite(mpLameOutBuffer + offset, sizeof(unsigned char), s, pFile);
    } 
    else
    {
      printf("Skip empty frame?\n");
    }

    offset += s;
    size -= s;
  }

  if (pFile)
    fclose(pFile);
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


