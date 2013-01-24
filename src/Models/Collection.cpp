//
//  YaCollection.cpp
//  YaLive
//
//  Created by Jérôme BLONDON on 11/01/13.
//
//

#include "Collection.h"
#include "RequestBuilder.h"

Models::Collection::Collection()
{
  mAuth = false;
}

Models::Collection::~Collection()
{
  std::vector<Object*>::iterator it = mObjects.begin();
  std::vector<Object*>::iterator end = mObjects.end();
  
  while (it != end)
  {
    Object* pObject = *it;
    delete pObject;
    ++it;
  }
  
}

void Models::Collection::Fetch(const CollectionResponseDelegate& rDelegate, int offset, int limit)
{
  mDelegate = rDelegate;
  
  nuiHTTPRequest *pRequest = RequestBuilder::Instance()->BuildGetObjectsRequest(mApi, mAuth, offset, limit);
  pRequest->SendRequest(nuiMakeDelegate(this, &Models::Collection::HandleResponse));
}

void Models::Collection::HandleResponse(nuiHTTPRequest* pRequest, nuiHTTPResponse* pResponse)
{
  if (pResponse)
  {
    nuiJson::Value root;
    nuiJson::Reader reader;
    bool parsed = reader.parse(pResponse->GetBodyStr().GetStdString(), root);
    if (parsed)
    {
      ParseJson(root);
    }
    mDelegate(this);
  }
}

void Models::Collection::ParseHeader(nuiJson::Value& object)
{
  for (uint32 i = 0; i < object.getMemberNames().size(); i++)
  {
    std::string name = object.getMemberNames()[i];
    nuiJson::Value& rVal = object[name];
    
    if (name.compare("total_count") == 0)
    {
      mHeader.mTotalCount = rVal.asInt();
    }
    else if (name.compare("offset") == 0)
    {
      mHeader.mOffset = rVal.asInt();
    }
    else if (name.compare("limit") == 0)
    {
      mHeader.mLimit = rVal.asInt();
    }
    else if (name.compare("previous") == 0)
    {
      mHeader.mPrevious = rVal.asString();
    }
    else if (name.compare("next") == 0)
    {
      mHeader.mNext = rVal.asString();
    }
  }
}

void Models::Collection::ParseObjects(nuiJson::Value& array)
{
  for (uint32 i = 0; i < array.size(); i++)
  {
    nuiJson::Value& rVal = array[i];
    Object *pObject = ParseObject(rVal);
    if (pObject)
    {
      mObjects.push_back(pObject);
    }
  }
}

void Models::Collection::ParseJson(nuiJson::Value& root)
{
  for (uint32 i = 0; i < root.getMemberNames().size(); i++)
  {
    std::string name = root.getMemberNames()[i];
    nuiJson::Value& rVal = root[name];
        
    if (name.compare("meta") == 0)
    {
      ParseHeader(rVal);
    }
    else if (name.compare("objects") == 0)
    {
      ParseObjects(rVal);
    }
  }
}