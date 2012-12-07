//
//  SongDB.h
//  YaLive
//
//  Created by Sébastien Métrot on 12/7/12.
//
//

#pragma once

#include "nui.h"

class Song
{
public:
  Song();
  virtual ~Song();

  nglString mTitle;
  nglString mArtist;
  nglString mAlbum;
  nglString mNotes;
  nglString mGenre;
  nglString mArtwork;
  uint8 mRating;
  float mBPM;
  float mDuration;
};

class SongDB
{
public:
  SongDB();
  virtual ~SongDB();

protected:
  std::vector<Song*> mSongs;
  void AddSong(Song* pSong);

  void Clear();
};

