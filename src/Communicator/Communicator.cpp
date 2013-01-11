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
