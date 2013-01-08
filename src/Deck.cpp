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

//  NSWindow* window = [[NSWindow alloc]
//                      initWithContentRect: NSMakeRect(0, 0, 640, 480)
//                      styleMask: NSTitledWindowMask | NSMiniaturizableWindowMask
//                      backing: NSBackingStoreBuffered
//                      defer: NO];
//  [window setTitle: @"New Window"];
//
  LoginViewController* login = [[LoginViewController alloc] init];
//
//  NSWindow* window = [[NSWindow alloc]
//                      initWithContentRect: NSMakeRect(0, 0, 640, 480)
//                      styleMask: NSTitledWindowMask | NSMiniaturizableWindowMask
//                      backing: NSBackingStoreBuffered
//                      defer: NO];
//  [window setTitle: @"New Window"];
//  [window center];
//  [window makeKeyAndOrderFront:nil];
}
