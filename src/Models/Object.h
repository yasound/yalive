//
//  Object.h
//  YaLive
//
//  Created by Jérôme BLONDON on 11/01/13.
//
//

#pragma once

#include "nui.h"
#include "nuiHTTP.h"
#include "nuiJson.h"

namespace Models {
  class Object
  {
  public:
    typedef nuiFastDelegate1<Object*> ObjectResponseDelegate;
    Object();
    virtual ~Object() {};
    
    void Fetch(const ObjectResponseDelegate& rDelegate);
    virtual Object* ParseObject(nuiJson::Value &data) = 0;
    
  private:
    void HandleResponse(nuiHTTPRequest* pRequest, nuiHTTPResponse* pResponse);
    
  protected:
    ObjectResponseDelegate mDelegate;
    nglString mApi;
    bool mAuth;
  };
}