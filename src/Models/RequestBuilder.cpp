//
//  Communicator.cpp
//  YaLive
//
//  Created by Jérôme BLONDON on 11/01/13.
//
//
#include "Settings.h"
#include "RequestBuilder.h"
#include "CurrentUser.h"

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
:mBaseUrl(YASOUND_SERVER)
{
}

Models::RequestBuilder::~RequestBuilder()
{
}

/*
 * Public methods
 */

nuiHTTPRequest* Models::RequestBuilder::BuildGetObjectRequest(const nglString& api, bool auth)
{
  nuiHTTPRequest *pRequest = new nuiHTTPRequest(this->GetApiUrl(api), _T("GET"));
  if (auth)
  {
    nglString cookies;
    cookies.Format("sessionid=%s", CurrentUser::Instance()->GetSessionId().GetChars());
    pRequest->AddHeader(_T("Cookie"), cookies);
  }
  return pRequest;
}

nuiHTTPRequest* Models::RequestBuilder::BuildGetObjectsRequest(const nglString& api, bool auth, int offset, int limit, const nglString& method)
{
  nuiHTTPRequest *pRequest = new nuiHTTPRequest(this->GetApiUrl(api, offset, limit), method);
  if (auth)
  {
    nglString cookies;
    cookies.Format("sessionid=%s", CurrentUser::Instance()->GetSessionId().GetChars());
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
