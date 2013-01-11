//
//  Deck.h
//  YaLive
//
//  Created by Sébastien Métrot on 12/7/12.
//
//

#pragma once

#include "nui.h"
#include "nuiJson.h"

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

public:
  void OnLoginReceived(const nglString& sessionId);
  void OnRadiosReceived(nuiJson::Value root);
  
  nuiEventSink<Deck> mSink;
};