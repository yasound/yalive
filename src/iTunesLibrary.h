//
//  iTunesLibrary.h
//  YaLive
//
//  Created by Sébastien Métrot on 12/7/12.
//
//

#pragma once

#include "SongDB.h"

class iTunesXMLParser;

class iTunesLibrary : public SongDB
{
public:
  iTunesLibrary();
  virtual ~iTunesLibrary();

  bool LoadFromXMLFile(const nglPath& rPath);

private:
  friend class iTunesXMLParser;

};