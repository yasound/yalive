/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSS.h"
#include "nuiVBox.h"

#include "iTunesLibrary.h"

/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : mEventSink(this),
    nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent))
{
  //mClearBackground = false;
  SetDebugMode(true);
  EnableAutoRotation(true);

  LoadCSS(_T("rsrc:/css/main.css"));

  nuiWidget* pWidget = NULL;
  
  pWidget = nuiBuilder::Get().CreateWidget(_T("Header"));
  pWidget->SetPosition(nuiTop);
  AddChild(pWidget);
  mpHeader = new Header(pWidget);
  
  std::map<nglString, nglString> dict;
  dict["DeckName"] = "Deck A";
  pWidget = nuiBuilder::Get().CreateWidget(_T("Deck"), dict);
  pWidget->SetPosition(nuiLeft);
  AddChild(pWidget);
  mpDeckA = new Deck(pWidget);

  dict["DeckName"] = "Deck B";
  pWidget = nuiBuilder::Get().CreateWidget(_T("Deck"), dict);
  pWidget->SetPosition(nuiRight);
  AddChild(pWidget);
  mpDeckB = new Deck(pWidget);

  // Load iTunes DB:
#ifdef _COCOA_
#if 0
  nglPath homePath = nglPath(ePathUser);
  nglString iTunePath;
  iTunePath.Format("%s/%s", homePath.GetPathName().GetChars(), "Music/iTunes/iTunes Music Library.xml");
  NGL_OUT("%s", iTunePath.GetChars());
  iTunesLibrary lib;
  bool res = lib.LoadFromXMLFile(iTunePath);

  if (!res)
  {
    NGL_OUT("Unable to load iTunes library from XML file");
  }
#endif
#endif
}

MainWindow::~MainWindow()
{
}


#define FONT_SIZE 35

void MainWindow::OnCreation()
{
  //EnableAutoRotation(false);
  //SetRotation(90);
  //SetState(nglWindow::eMaximize);

}

void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}

bool MainWindow::LoadCSS(const nglPath& rPath)
{
  nglIStream* pF = rPath.OpenRead();
  if (!pF)
  {
    NGL_OUT(_T("Unable to open CSS source file '%ls'\n"), rPath.GetChars());
    return false;
  }

  nuiCSS* pCSS = new nuiCSS();
  bool res = pCSS->Load(*pF, rPath);
  delete pF;

  if (res)
  {
    nuiMainWindow::SetCSS(pCSS);
    return true;
  }

  NGL_OUT(_T("%ls\n"), pCSS->GetErrorString().GetChars());

  delete pCSS;
  return false;
}
