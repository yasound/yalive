//
//  Header.cpp
//  YaLive
//
//  Created by Jérôme BLONDON on 21/01/13.
//
//

#include "Header.h"
#include "MyRadios.h"
#include "Radio.h"
#include "RequestBuilder.h"
#include "CurrentUser.h"
#include "LoginViewController.h"


Header::Header(nuiWidget* pHeader)
: mSink(this)
{
  mpHeader = pHeader;
  NGL_ASSERT(mpHeader);
  
  mpLogin = (nuiButton*)mpHeader->Find("HeaderBox")->Find("Login");
  mpTestLogin = (nuiButton*)mpHeader->Find("HeaderBox")->Find("TestLogin");
  mpLogout = (nuiButton*)mpHeader->Find("HeaderBox")->Find("Logout");
  mpLabelLogin = (nuiLabel*)mpHeader->Find("HeaderBox")->Find("LabelLogin");
  
  NGL_ASSERT(mpLogin && mpTestLogin && mpLogout && mpLabelLogin);
  
  mSink.Connect(mpLogin->Activated, &Header::OnLogin);
  mSink.Connect(mpTestLogin->Activated, &Header::OnTestLogin);
  mSink.Connect(mpLogout->Activated, &Header::OnLogout);
  
  mpTestLogin->SetVisible(false);
                          
  ShowLoginButtons(true);
  mpLogin->SetEnabled(false);
  Models::CurrentUser::Instance()->Fetch(nuiMakeDelegate(this, &Header::OnCurrentUserReceived));
}

Header::~Header()
{
  mpHeader->Trash();
}

void Header::OnLogin(const nuiEvent& rEvent)
{
  mpLogin->SetEnabled(false);
  [[LoginViewController alloc] initWithCB:this];
}

void Header::OnTestLogin(const nuiEvent& rEvent)
{
  Models::CurrentUser::Instance()->Fetch(nuiMakeDelegate(this, &Header::OnCurrentUserReceived));
}

void Header::OnLogout(const nuiEvent& rEvent)
{
  Models::CurrentUser::Instance()->Logout();
  mpLabelLogin->SetText(_T("Not logged in"));
  mpLogin->SetVisible(true);
  mpLogin->SetEnabled(true);
  mpLogout->SetVisible(false);
}


void Header::OnLoginReceived(id controller, const char* sessionId)
{
  NGL_OUT("Received sessionid=%s\n", sessionId);
  
  mpLogin->SetEnabled(true);
  
  [controller release];
  Models::CurrentUser::Instance()->SetSessionId(sessionId);
  
  Models::CurrentUser::Instance()->Fetch(nuiMakeDelegate(this, &Header::OnCurrentUserReceived));
                                         
  Models::MyRadios *pRadios = new Models::MyRadios();
  NGL_OUT("Fetching owner radios\n");
  pRadios->Fetch(nuiMakeDelegate(this, &Header::OnRadiosReceived));
}

void Header::OnLoginCanceledReceived(id controller)
{
  ShowLoginButtons(true);
  [controller release];
}

void Header::OnRadiosReceived(uint16 statusCode, Models::Collection *pCollection)
{
  NGL_OUT("Received radios (status=%d)\n", statusCode);
  std::vector<Models::Object*>::iterator it = pCollection->mObjects.begin();
  std::vector<Models::Object*>::iterator end = pCollection->mObjects.end();
  
  while (it != end)
  {
    Models::Radio* pRadio = (Models::Radio *)*it;
    NGL_OUT("%s\n", pRadio->GetName().GetChars());
    ++it;
  }
  
  delete pCollection;
}

void Header::OnCurrentUserReceived(uint16 statusCode, Models::Object *pObject)
{
  if (statusCode == 200)
  {
    Models::CurrentUser *pUser = (Models::CurrentUser*) pObject;
    mpLabelLogin->SetText(pUser->GetUsername());
    ShowLoginButtons(false);
  } else {
    mpLabelLogin->SetText(_T("Not logged in"));
    ShowLoginButtons(true);
  }
}

void Header::ShowLoginButtons(bool show)
{
  if (show)
  {
    mpLogin->SetVisible(true);
    mpLogin->SetEnabled(true);
    mpLogout->SetVisible(false);
  }
  else
  {
    mpLogin->SetVisible(false);
    mpLogout->SetVisible(true);
  }
}

