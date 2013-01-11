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

class Communicator
{
public:
  Communicator(const nglString& sessionId);
  virtual ~Communicator();
  
public:
  nuiHTTPRequest* BuildGetObjectRequest(const nglString& api, bool auth=true);
  nuiHTTPRequest* BuildGetObjectsRequest(const nglString& api, bool auth=true, int offset=0, int limit=25);
  nglString GetApiUrl(const nglString& api);
  nglString GetApiUrl(const nglString& api, int offset, int limit);
  
  
private:
  nglString mSessionId;
  nglString mBaseUrl;
  
};
