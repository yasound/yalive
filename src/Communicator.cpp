//
//  Communicator.cpp
//  YaLive
//
//  Created by Jérôme BLONDON on 11/01/13.
//
//
#include "Communicator.h"
#include "nuiJson.h"

Communicator::Communicator(const nglString &sessionId)
:mBaseUrl("http://localhost:8000")
{
  this->mSessionId = sessionId;
}

Communicator::~Communicator()
{
}

void Communicator::GetRadios()
{
  nuiHTTPRequest *pRequest = this->BuildGetRequest("/api/v1/my_radios/");
  NGL_OUT(_T("Launching request!\n"));
  nuiHTTPResponse* pRes = pRequest->SendRequest();
  NGL_OUT(_T("Done!\n"));
  if (!pRes)
  {
    NGL_OUT(_T("Cannot get radios\n"));
    
    delete pRequest;
    return;
  }
  nuiJson::Value root;
  nuiJson::Reader reader;
  bool parsed = reader.parse(pRes->GetBodyStr().GetStdString(), root);
  if (!parsed)
  {
    NGL_OUT(_T("Cannot parse json data"));

    delete pRequest;
    return;
  }
  delete pRequest;  
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
