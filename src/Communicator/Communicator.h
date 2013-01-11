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
  typedef nuiFastDelegate1<nuiJson::Value> CommunicatorResponseDelegate;

  Communicator(const nglString& sessionId);
  virtual ~Communicator();
  
public:
  nuiHTTPRequest* BuildGetRequest(const nglString& api, bool auth=true);
  nglString GetApiUrl(const nglString& api);
  
  
private:
  nglString mSessionId;
  nglString mBaseUrl;
  
};
