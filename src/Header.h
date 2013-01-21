//
//  Header.h
//  YaLive
//
//  Created by Jérôme BLONDON on 21/01/13.
//
//

#pragma once

#include "nui.h"
#include "Collection.h"

class Header
{
public:
  Header(nuiWidget* pDeck);
  virtual ~Header();
  
public:
  void OnLoginReceived(const char* sessionId);

protected:
  nuiWidget* mpHeader;
  nuiButton* mpLogin;
  
  void OnLogin(const nuiEvent& rEvent);

  
private:
  void OnRadiosReceived(Models::Collection *pCollection);
  
  nuiEventSink<Header> mSink;
};