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
#include "LoginViewController.h"


Header::Header(nuiWidget* pHeader)
: mSink(this)
{
  mpHeader = pHeader;
  NGL_ASSERT(mpHeader);
  
  mpLogin = (nuiButton*)mpHeader->Find("Login");
  
  NGL_ASSERT(mpLogin);
  
  mSink.Connect(mpLogin->Activated, &Header::OnLogin);
}

Header::~Header()
{
  mpHeader->Trash();
}

void Header::OnLogin(const nuiEvent& rEvent)
{
  [[LoginViewController alloc] initWithCB:this];
}


void Header::OnLoginReceived(id controller, const char* sessionId)
{
  [controller release];
  Models::RequestBuilder::Instance()->SetSessionId(sessionId);
  
  Models::MyRadios *pRadios = new Models::MyRadios(sessionId);
  pRadios->Fetch(nuiMakeDelegate(this, &Header::OnRadiosReceived));
}

void Header::OnRadiosReceived(Models::Collection *pCollection)
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