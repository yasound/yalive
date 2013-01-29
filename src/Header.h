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
  nuiButton* mpTestLogin;
  nuiButton* mpLogout;
  nuiLabel* mpLabelLogin;
  
  void OnLogin(const nuiEvent& rEvent);
  void OnTestLogin(const nuiEvent& rEvent);
  void OnLogout(const nuiEvent& rEvent);

  void ShowLoginButtons(bool show);

private:
  void OnCurrentUserReceived(uint16 statusCode, Models::Object *pObject);
  void OnRadiosReceived(uint16 statusCode, Models::Collection *pCollection);
  
  nuiEventSink<Header> mSink;
};