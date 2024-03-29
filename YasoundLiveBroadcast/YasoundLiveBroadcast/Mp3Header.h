//
//  Mp3Parser.h
//  MP3Parser
//
//  Created by matthieu campion on 11/22/11.
//  Copyright (c) 2011 MXP4. All rights reserved.
//

#pragma once

#include <string>

enum MpegAudioVersion
{
  eMpegVersion2_5 = 0,
  eMpegVersionReserved = 1,
  eMpegVersion2 = 2,
  eMpegVersion1 = 3,
  eMpegVersionUndefined = 4
};

enum MpegLayer
{
  eMpegLayerReserved = 0,
  eMpegLayer3 = 1,
  eMpegLayer2 = 2,
  eMpegLayer1 = 3,
  eMpegLayerUndefined = 4
};

enum MpegChannelMode
{
  eMpegStereo = 0,
  eMpegJointStereo = 1,
  eMpegDualMono = 2,
  eMpegMono = 3,
  eMpegChannelModeUndefined = 4
};

enum MpegEmphasis
{
  eMpegEmphasisNone = 0,
  eMpegEmphasis50_15ms = 1,
  eMpegEmphasisReserved = 2,
  eMpegEmphasisCCIT_J_17 = 3,
  eMpegEmphasisUndefined = 4
};

typedef unsigned long TimeMs;

typedef uint8_t uint8;
typedef int8_t int8;
typedef uint16_t uint16;
typedef int16_t int16;
typedef uint32_t uint32;
typedef int32_t int32;
typedef uint64_t uint64;
typedef int64_t int64;

class Mp3Header
{
public:
  Mp3Header(uint8* data); // data must point to 4 bytes minimum.
  virtual ~Mp3Header();
  
  static float GetBitrate(unsigned int bitrate_index, MpegAudioVersion version, MpegLayer layer);
  static float GetSampleRate(unsigned int samplerate_index, MpegAudioVersion version);
  
  static float sBitrates[5][16];
  static float sSamplerates[3][3];
  static int sSamplesPerFrame[2][3];
  
  const bool IsValid() const;
  const std::string ToString() const;
  
  const int GetFrameByteLength() const;
  const int GetFrameHeaderByteLength() const;
  const int GetFrameDataByteLength() const;
  
  const TimeMs GetFrameDuration() const;
  
  MpegAudioVersion mVersion;
  MpegLayer mLayer;
  MpegChannelMode mChannelMode;
  MpegEmphasis mEmphasis;
  float mBitrate;
  float mSamplerate;
  bool mUseCRC;
  bool mUsePadding;
  bool mIsCopyrighted;
  bool mIsOriginal;
  
  bool mIsXing;
private:
  void Reset();
  void ParseHeaderData(unsigned char* data);
  const int GetSamplesPerFrame() const;
};

