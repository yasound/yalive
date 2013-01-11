//
//  Communicator.cpp
//  YaLive
//
//  Created by Jérôme BLONDON on 11/01/13.
//
//
#include "Communicator.h"

Communicator::Communicator(const nglString &sessionId)
:mBaseUrl("http://localhost:8000")
{
  this->mSessionId = sessionId;
}

Communicator::~Communicator()
{
}

void Communicator::GetRadios(const CommunicatorResponseDelegate& rDelegate)
{
  nuiHTTPRequest *pRequest = this->BuildGetRequest("/api/v1/my_radios/");
  
  Communicator_Handler *pHandler = new Communicator_Handler(rDelegate, pRequest);
  pHandler->SendRequest();
}


nuiHTTPRequest* Communicator::BuildGetRequest(const nglString& api, bool auth)
{
  nuiHTTPRequest *pRequest = new nuiHTTPRequest(this->GetApiUrl(api), _T("GET"));
  if (auth)
  {
    nglString cookies;
    cookies.Format("sessionid=%s", this->mSessionId.GetChars());
    pRequest->AddHeader(_T("Cookie"), cookies);
  }
  return pRequest;
}

nglString Communicator::GetApiUrl(const nglString& api)
{
  nglString url;
  url.Format("%s%s", this->mBaseUrl.GetChars(), api.GetChars());
  return url;
}

//-------------

Communicator_Handler::Communicator_Handler(const Communicator::CommunicatorResponseDelegate &rDelegate, nuiHTTPRequest *pRequest)
:mDelegate(rDelegate), mpRequest(pRequest)
{
  
}

Communicator_Handler::~Communicator_Handler()
{
}

void Communicator_Handler::SendRequest()
{
  mpThread = mpRequest->SendRequest(nuiMakeDelegate(this, &Communicator_Handler::HandleResponse));
}

void Communicator_Handler::HandleResponse(nuiHTTPRequest* pRequest, nuiHTTPResponse* pResponse)
{
  if (!pResponse)
  {
    NGL_OUT(_T("Request error\n"));
    delete this;
    return;
  }
  nuiJson::Value root;
  nuiJson::Reader reader;
  bool parsed = reader.parse(pResponse->GetBodyStr().GetStdString(), root);
  if (!parsed)
  {
    NGL_OUT(_T("Cannot parse json data"));
    delete this;
    return;
  }
  this->mDelegate(root);
  delete this;
}