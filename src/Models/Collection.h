//
//  YaCollection.h
//  YaLive
//
//  Created by Jérôme BLONDON on 11/01/13.
//
//

#pragma once

#include "nui.h"
#include "nuiHTTP.h"
#include "nuiJson.h"
#include "Object.h"

namespace Models {
  class Collection
  {
  public:
    typedef nuiFastDelegate1<Collection*> CollectionResponseDelegate;

    Collection();
    virtual ~Collection();
    
    void Fetch(const CollectionResponseDelegate& rDelegate, int offset=0, int limit=25);
    
  protected:
    virtual Object* ParseObject(nuiJson::Value &object) = 0;
    
  private:
    void HandleResponse(nuiHTTPRequest* pRequest, nuiHTTPResponse* pResponse);
    
    void ParseJson(nuiJson::Value &root);
    void ParseHeader(nuiJson::Value &object);
    void ParseObjects(nuiJson::Value &array);
    
  protected:
    CollectionResponseDelegate mDelegate;
    nglString mApi;
    bool mAuth;
    
    class Header
    {
    public:
      int mTotalCount;
      int mOffset;
      int mLimit;
      nglString mNext;
      nglString mPrevious;
    };
    Header mHeader;
  
  public:
    std::vector<Object*> mObjects;
    
  };
  
}