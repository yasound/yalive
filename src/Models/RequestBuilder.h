//
//  Communicator.h
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
  class RequestBuilder
  {
  public:
    static RequestBuilder* Instance();
  
  protected:
    RequestBuilder();
    virtual ~RequestBuilder();
    
  public:
    nuiHTTPRequest* BuildGetObjectRequest(const nglString& api, bool auth=true);
    nuiHTTPRequest* BuildGetObjectsRequest(const nglString& api, bool auth=true, int offset=0, int limit=25, const nglString &method=_T("GET"));
    nglString GetApiUrl(const nglString& api);
    nglString GetApiUrl(const nglString& api, int offset, int limit);
    
    
  private:
    static RequestBuilder* mpInstance;
    nglString mBaseUrl;
    
  };  
}
