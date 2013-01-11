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

class Communicator
{
public:
  Communicator(const nglString& sessionId);
  virtual ~Communicator();

  void GetRadios();
  
private:
  nuiHTTPRequest* BuildGetRequest(const nglString& api, bool auth=true);
  nglString GetApiUrl(const nglString& api);
  
  
private:
  nglString mSessionId;
  nglString mBaseUrl;
  
};