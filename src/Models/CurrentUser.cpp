//
//  CurrentUser.cpp
//  YaLive
//
//  Created by Jérôme BLONDON on 24/01/13.
//
//

#include "CurrentUser.h"
#include "RequestBuilder.h"


/*
 * Static constructor
 */
Models::CurrentUser* Models::CurrentUser::mpInstance = 0;

Models::CurrentUser* Models::CurrentUser::Instance()
{
  if (mpInstance == 0)
  {
    mpInstance = new Models::CurrentUser();
  }
  return mpInstance;
}

/*
 * Constructor / destructor
 */

Models::CurrentUser::CurrentUser()
:mSessionId("")
{
  mAuth = true;
  mApi = "/api/v1/my_profile/";
}

Models::CurrentUser::~CurrentUser()
{
}

Models::Object* Models::CurrentUser::ParseObject(nuiJson::Value &data)
{
  for (uint32 i = 0; i < data.getMemberNames().size(); i++)
  {
    std::string name = data.getMemberNames()[i];
    nuiJson::Value& rVal = data[name];
    
    if (name.compare("username") == 0)
    {
      SetUsername(rVal.asString());
    }
  }
  return this;
}

void Models::CurrentUser::Logout()
{
  nglString api = "/api/v1/logout/";
  nuiHTTPRequest *pRequest = RequestBuilder::Instance()->BuildGetObjectsRequest(api, true, 0, 0, _T("POST"));
  pRequest->SendRequest(nuiMakeDelegate(this, &Models::CurrentUser::HandleLogoutResponse));
}

void Models::CurrentUser::HandleLogoutResponse(nuiHTTPRequest* pRequest, nuiHTTPResponse* pResponse)
{
  SetSessionId("");
}




