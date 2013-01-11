//
//  Deck.cpp
//  YaLive
//
//  Created by Sébastien Métrot on 12/7/12.
//
//

#include "Deck.h"
#include "LoginViewController.h"

#include "nui.h"
#include "nglKernel.h"
#include "nglContext.h"
#include "MyRadios.h"
#include "Radio.h"
#include "RequestBuilder.h"


Deck::Deck(nuiWidget* pDeck)
: mSink(this)
{
  mpDeck = pDeck;
  NGL_ASSERT(mpDeck);

  mpSongName = (nuiLabel*)mpDeck->Find("SongName");
  mpVolume = (nuiSlider*)mpDeck->Find("Volume");
  mpPlayPause = (nuiToggleButton*)mpDeck->Find("PlayPause");
  mpChooseSong = (nuiButton*)mpDeck->Find("ChooseSong");

  NGL_ASSERT(mpVolume && mpPlayPause && mpChooseSong);

  mSink.Connect(mpPlayPause->Activated, &Deck::OnPlayPause);
  mSink.Connect(mpPlayPause->Deactivated, &Deck::OnPlayPause);

  mSink.Connect(mpChooseSong->Activated, &Deck::OnChooseSong);
}

Deck::~Deck()
{
  mpDeck->Trash();
}

void Deck::OnPlayPause(const nuiEvent& rEvent)
{
  NGL_OUT("Play/Pause");
}

void Deck::OnChooseSong(const nuiEvent& rEvent)
{
  NGL_OUT("Choose Song\n");

  LoginViewController* login = [[LoginViewController alloc] initWithCB:this];
}

void Deck::OnLoginReceived(const nglString& sessionId)
{
  NGL_OUT("Login received: %s", sessionId.GetChars());

  Models::RequestBuilder::Instance()->SetSessionId(sessionId);

  Models::MyRadios *pRadios = new Models::MyRadios(sessionId);
  pRadios->Fetch(nuiMakeDelegate(this, &Deck::OnRadiosReceived));
}

void Deck::OnRadiosReceived(Models::Collection *pCollection)
{
  std::vector<Models::Object*>::iterator it = pCollection->mObjects.begin();
  std::vector<Models::Object*>::iterator end = pCollection->mObjects.end();
  
  while (it != end)
  {
    Models::Radio* pRadio = (Models::Radio *)*it;
    NGL_OUT("%s\n", pRadio->GetName().GetChars());
    ++it;
  }
  
  
  NGL_OUT("Yes!!!\n");
  delete pCollection;
}
