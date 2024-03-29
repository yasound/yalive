//
//  Mp3Parser.cpp
//  MP3Parser
//
//  Created by matthieu campion on 11/22/11.
//  Copyright (c) 2011 MXP4. All rights reserved.
//

#include "Mp3Header.h"


Mp3Header::Mp3Header(uint8* data)
{
  Reset();
  ParseHeaderData(data);
}



Mp3Header::~Mp3Header()
{
}

void Mp3Header::Reset()
{
  mVersion = eMpegVersionUndefined;
  mLayer = eMpegLayerUndefined;
  mChannelMode = eMpegChannelModeUndefined;
  mEmphasis = eMpegEmphasisUndefined;
  mBitrate = 0;
  mSamplerate = 0;
  mUseCRC = false;
  mUsePadding = false;
  mIsCopyrighted = false;
  mIsOriginal = false;
}

void Mp3Header::ParseHeaderData(unsigned char* data)
{
  Reset();
  unsigned int b0 = data[0];
  unsigned int b1 = data[1];
  unsigned int b2 = data[2];
  unsigned int b3 = data[3];
  unsigned int val = b3 | (b2 << 8) | (b1 << 16) | (b0 << 24);
  
  unsigned int sync = 0xffe00000;
  unsigned int res = (val & sync);
  bool ok = (res == sync);
  if (!ok)
  {
    if (0)
    {
      printf("res != sync (%x != %x)\n", res, sync);
      printf("ParseHeaderData broken\n");
    }
    return;
  }
  unsigned int version = (val >> 19) & 3;
  unsigned int layer = (val >> 17) & 3;
  unsigned int crc = (val >> 16) & 1;
  unsigned int bitrate_index = (val >> 12) & 15;
  unsigned int samplerate_index = (val >> 10) & 3;
  unsigned int padding = (val >> 9) & 1;
  unsigned int channelMode = (val >> 6) & 3;
  unsigned int copyright = (val >> 3) & 1;
  unsigned int original = (val >> 2) & 1;
  unsigned int emphasis = (val >> 0) & 3;
  
  
  mVersion = (MpegAudioVersion)version;
  mLayer = (MpegLayer)layer;
  mChannelMode = (MpegChannelMode)channelMode;
  mEmphasis = (MpegEmphasis)emphasis;
  
  mBitrate = GetBitrate(bitrate_index, mVersion, mLayer);
  mSamplerate = GetSampleRate(samplerate_index, mVersion);
  
  mUseCRC = crc;
  mUsePadding = padding;
  
  mIsCopyrighted = copyright;
  mIsOriginal = original;
}


int Mp3Header::sSamplesPerFrame[2][3] =
{
  {384, 1152, 1152},
  {384, 1152, 576}
};

const int Mp3Header::GetFrameHeaderByteLength() const
{
  int size = 4;
  return size;
}

const int Mp3Header::GetSamplesPerFrame() const
{
  int samplesPerFrame = 0;
  switch (mVersion)
  {
    case eMpegVersion1:
    {
      switch (mLayer)
      {
        case eMpegLayer1:
          samplesPerFrame = sSamplesPerFrame[0][0];
          break;
        case eMpegLayer2:
          samplesPerFrame = sSamplesPerFrame[0][1];
          break;
        case eMpegLayer3:
          samplesPerFrame = sSamplesPerFrame[0][2];
          break;
          
        default:
          break;
      }
    }
      break;
      
    case eMpegVersion2:
    case eMpegVersion2_5:
    {
      switch (mLayer)
      {
        case eMpegLayer1:
          samplesPerFrame = sSamplesPerFrame[1][0];
          break;
        case eMpegLayer2:
          samplesPerFrame = sSamplesPerFrame[1][1];
          break;
        case eMpegLayer3:
          samplesPerFrame = sSamplesPerFrame[1][2];
          break;
          
        default:
          break;
      }
    }
      break;
      
    default:
      break;
  }
  
  return samplesPerFrame;
}

const int Mp3Header::GetFrameDataByteLength() const
{
  int bytes = (GetSamplesPerFrame() / mSamplerate) * (mBitrate * 1000.0 / 8.0);
  if (mUsePadding)
    bytes += 1;
  
  bytes -= GetFrameHeaderByteLength();
  return bytes;
}

const int Mp3Header::GetFrameByteLength() const
{
  int size = GetFrameHeaderByteLength() + GetFrameDataByteLength();
  return size;
}

const TimeMs Mp3Header::GetFrameDuration() const
{
  TimeMs durationMs = (GetSamplesPerFrame() / mSamplerate) * 1000;
  return durationMs;
}

const bool Mp3Header::IsValid() const
{
  bool versionOK = mVersion != eMpegVersionUndefined;
  bool layerOK = mLayer != eMpegLayerUndefined;
  bool channelOK = mChannelMode != eMpegChannelModeUndefined;
  bool emphasisOK = mEmphasis != eMpegEmphasisUndefined;
  bool bitrateOK = mBitrate != 0;
  bool samplerateOK = mSamplerate != 0;
  
  bool valid = (versionOK && layerOK && channelOK && emphasisOK && bitrateOK && samplerateOK);
  
  if (!valid)
  {
  }
  return valid;
}

const std::string Mp3Header::ToString() const
{
  // version
  std::string versionStr = "version: '";
  switch (mVersion)
  {
    case eMpegVersion1:
      versionStr += "v1";
      break;
    case eMpegVersion2:
      versionStr += "v2";
      break;
    case eMpegVersion2_5:
      versionStr += "v2.5";
      break;
    case eMpegVersionReserved:
      versionStr += "reserved";
      break;
    case eMpegVersionUndefined:
      versionStr += "undefined";
      break;
      
    default:
      break;
  }
  versionStr += "'  ";
  
  // layer
  std::string layerStr = "layer: '";
  switch (mLayer)
  {
    case eMpegLayer1:
      layerStr += "1";
      break;
    case eMpegLayer2:
      layerStr += "2";
      break;
    case eMpegLayer3:
      layerStr += "3";
      break;
    case eMpegLayerReserved:
      layerStr += "reserved";
      break;
    case eMpegLayerUndefined:
      layerStr += "undefined";
      break;
      
    default:
      break;
  }
  layerStr += "'  ";
  
  // channel mode
  std::string channelStr = "channels: '";
  switch (mChannelMode)
  {
    case eMpegStereo:
      channelStr += "stereo";
      break;
    case eMpegJointStereo:
      channelStr += "joint stereo";
      break;
    case eMpegMono:
      channelStr += "mono";
      break;
    case eMpegDualMono:
      channelStr += "dual mono";
      break;
    case eMpegChannelModeUndefined:
      channelStr += "undefined";
      break;
      
    default:
      break;
  }
  channelStr += "'  ";
  
  // emphasis
  std::string emphasisStr = "emphasis: '";
  switch (mEmphasis)
  {
    case eMpegEmphasisNone:
      emphasisStr += "none";
      break;
    case eMpegEmphasis50_15ms:
      emphasisStr += "50/15 ms";
      break;
    case eMpegEmphasisCCIT_J_17:
      emphasisStr += "CCIT J.17";
      break;
    case eMpegEmphasisReserved:
      emphasisStr += "reserved";
      break;
    case eMpegEmphasisUndefined:
      emphasisStr += "undefined";
      break;
      
    default:
      break;
  }
  emphasisStr += "'  ";
  
  char temp[1024];
  
  // bit rate
  sprintf(temp, "bitrate: '%.1f kbps'  ", mBitrate);
  std::string bitrateStr(temp);
  
  // sample rate
  sprintf(temp, "samplerate: '%.1f Hz'  ", mSamplerate);
  std::string samplerateStr(temp);
  
  // crc
  sprintf(temp, "crc: '%s'  ", mUseCRC ? "yes" : "no");
  std::string crcStr(temp);
  
  // padding
  sprintf(temp, "padding: '%s'  ", mUsePadding ? "yes" : "no");
  std::string paddingStr(temp);
  
  // copyright
  sprintf(temp, "copyright: '%s'  ", mIsCopyrighted ? "yes" : "no");
  std::string copyrightStr(temp);
  
  // original
  sprintf(temp, "original: '%s'  ", mIsOriginal ? "yes" : "no");
  std::string originalStr(temp);
  
  std::string s;
  s += versionStr;
  s += layerStr;
  s += bitrateStr;
  s += samplerateStr;
  s += channelStr;
  s += crcStr;
  s += paddingStr;
  s += copyrightStr;
  s += originalStr;
  s += emphasisStr;
  return s;
}



float Mp3Header::sBitrates[5][16] =
{
  {0, 32, 64, 96, 128, 160,192, 224, 256, 288, 320, 352, 384, 416, 448, -1},
  {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, -1},
  {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, -1},
  {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, -1},
  {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1}
};


float Mp3Header::GetBitrate(unsigned int bitrate_index, MpegAudioVersion version, MpegLayer layer)
{
  if (bitrate_index == 0)
    return 0;
  if (bitrate_index == 15)
    return 0;
  
  float bitrate = 0;
  switch (version)
  {
    case eMpegVersion1:
    {
      switch (layer)
      {
        case eMpegLayer1:
          bitrate = sBitrates[0][bitrate_index];
          break;
          
        case eMpegLayer2:
          bitrate = sBitrates[1][bitrate_index];
          break;
          
        case eMpegLayer3:
          bitrate = sBitrates[2][bitrate_index];
          break;
          
        default:
          break;
      }
    }
      break;
      
    case eMpegVersion2:
    case eMpegVersion2_5:
    {
      switch (layer)
      {
        case eMpegLayer1:
          bitrate = sBitrates[3][bitrate_index];
          break;
          
        case eMpegLayer2:
        case eMpegLayer3:
          bitrate = sBitrates[4][bitrate_index];
          break;
          
        default:
          break;
      }
    }
      break;
      
    default:
      break;
  }
  
  return bitrate;
}

float Mp3Header::sSamplerates[3][3] =
{
  {44100, 48000, 32000},
  {22050, 24000, 16000},
  {11025, 12000, 8000}
};

float Mp3Header::GetSampleRate(unsigned int samplerate_index, MpegAudioVersion version)
{
  if (samplerate_index == 3)
    return 0;
  
  float samplerate = 0;
  switch (version)
  {
    case eMpegVersion1:
      samplerate = sSamplerates[0][samplerate_index];
      break;
      
    case eMpegVersion2:
      samplerate = sSamplerates[1][samplerate_index];
      break;
      
    case eMpegVersion2_5:
      samplerate = sSamplerates[2][samplerate_index];
      break;
      
    default:
      break;
  }
  
  return samplerate;
}
