//
//  SongDB.cpp
//  YaLive
//
//  Created by Sébastien Métrot on 12/7/12.
//
//

#include "SongDB.h"

Song::Song()
: mRating(0), mBPM(0), mDuration(0)
{
}

Song::~Song()
{
}

SongDB::SongDB()
{
}

SongDB::~SongDB()
{
  Clear();
}


void SongDB::Clear()
{
  std::vector<Song*>::iterator it = mSongs.begin();
  std::vector<Song*>::iterator end = mSongs.end();

  while (it != end)
  {
    Song* pSong = *it;
    delete pSong;
    ++it;
  }
}


void SongDB::AddSong(Song* pSong)
{
  mSongs.push_back(pSong);
}
