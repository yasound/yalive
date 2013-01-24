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
#include "CurrentUser.h"

class Header
{
public:
  Header(nuiWidget* pDeck);
  virtual ~Header();
  
public:
  void OnLoginReceived(id controller, const char* sessionId);
  void OnLoginCanceledReceived(id controller);

protected:
  nuiWidget* mpHeader;
  nuiButton* mpLogin;
  
  void OnLogin(const nuiEvent& rEvent);

  
private:
  void OnCurrentUserReceived(Models::Object *pCurrentUser);
  void OnRadiosReceived(Models::Collection *pCollection);
  
  nuiEventSink<Header> mSink;
};