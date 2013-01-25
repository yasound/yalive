//
//  Object.cpp
//  YaLive
//
//  Created by Jérôme BLONDON on 11/01/13.
//
//

#include "Object.h"
#include "RequestBuilder.h"

Models::Object::Object()
{
  mAuth = false;
}

void Models::Object::Fetch(const ObjectResponseDelegate& rDelegate)
{
  mDelegate = rDelegate;
  nuiHTTPRequest *pRequest = RequestBuilder::Instance()->BuildGetObjectsRequest(mApi, mAuth);
  pRequest->SendRequest(nuiMakeDelegate(this, &Models::Object::HandleResponse));
}


void Models::Object::HandleResponse(nuiHTTPRequest* pRequest, nuiHTTPResponse* pResponse)
{
  if (pResponse)
  {
    nuiJson::Value root;
    nuiJson::Reader reader;
    bool parsed = reader.parse(pResponse->GetBodyStr().GetStdString(), root);
    if (parsed)
    {
      ParseObject(root);
    }
    mDelegate(pResponse->GetStatusCode(), this);
  }
}

