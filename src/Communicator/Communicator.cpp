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

nuiHTTPRequest* Communicator::BuildGetObjectRequest(const nglString& api, bool auth)
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

nuiHTTPRequest* Communicator::BuildGetObjectsRequest(const nglString& api, bool auth, int offset, int limit)
{
  nuiHTTPRequest *pRequest = new nuiHTTPRequest(this->GetApiUrl(api, offset, limit), _T("GET"));
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

nglString Communicator::GetApiUrl(const nglString& api, int offset, int limit)
{
  nglString url = GetApiUrl(api);
  url.Format("%s?offset=%d&limit=%d", url.GetChars(), offset, limit);
  return url;
}
