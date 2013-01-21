/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiMainWindow.h"
#include "Deck.h"
#include "Header.h"

class MainWindow : public nuiMainWindow
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();

  void OnCreation();
  void OnClose();

protected:
  bool LoadCSS(const nglPath& rPath);

  Deck* mpDeckA;
  Deck* mpDeckB;
  Header* mpHeader;
  nuiEventSink<MainWindow> mEventSink;
};

