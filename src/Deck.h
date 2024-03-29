//
//  Deck.h
//  YaLive
//
//  Created by Sébastien Métrot on 12/7/12.
//
//

#pragma once

#include "nui.h"

class Deck
{
public:
  Deck(nuiWidget* pDeck);
  virtual ~Deck();

protected:
  nuiWidget* mpDeck;
  nuiLabel* mpSongName;
  nuiSlider* mpVolume;
  nuiToggleButton* mpPlayPause;
  nuiButton* mpChooseSong;

  void OnPlayPause(const nuiEvent& rEvent);
  void OnChooseSong(const nuiEvent& rEvent);

  nuiEventSink<Deck> mSink;
};