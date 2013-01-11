//
//  Communicator.cpp
//  YaLive
//
//  Created by Jérôme BLONDON on 11/01/13.
//
//
#include "RequestBuilder.h"

/*
 * Static constructor
 */
Models::RequestBuilder* Models::RequestBuilder::mpInstance = 0;

Models::RequestBuilder* Models::RequestBuilder::Instance()
{
  if (mpInstance == 0)
  {
    mpInstance = new Models::RequestBuilder();
  }
  return mpInstance;
}


/*
 * Constructor / destructor
 */

Models::RequestBuilder::RequestBuilder()
:mBaseUrl("http://localhost:8000")
{
}

Models::RequestBuilder::~RequestBuilder()
{
}

/*
 * Public methods
 */

void Models::RequestBuilder::SetSessionId(const nglString &sessionId)
{
  mSessionId = sessionId;
}

nuiHTTPRequest* Models::RequestBuilder::BuildGetObjectRequest(const nglString& api, bool auth)
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

nuiHTTPRequest* Models::RequestBuilder::BuildGetObjectsRequest(const nglString& api, bool auth, int offset, int limit)
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

nglString Models::RequestBuilder::GetApiUrl(const nglString& api)
{
  nglString url;
  url.Format("%s%s", this->mBaseUrl.GetChars(), api.GetChars());
  return url;
}

nglString Models::RequestBuilder::GetApiUrl(const nglString& api, int offset, int limit)
{
  nglString url = GetApiUrl(api);
  url.Format("%s?offset=%d&limit=%d", url.GetChars(), offset, limit);
  return url;
}
