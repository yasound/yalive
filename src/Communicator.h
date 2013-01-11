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

  void GetRadios(const CommunicatorResponseDelegate& rDelegate);
  
private:
  nuiHTTPRequest* BuildGetRequest(const nglString& api, bool auth=true);
  nglString GetApiUrl(const nglString& api);
  
  
private:
  nglString mSessionId;
  nglString mBaseUrl;
  
};

class Communicator_Handler
{
public:
  Communicator_Handler(const Communicator::CommunicatorResponseDelegate &rDelegate, nuiHTTPRequest *pRequest);
  virtual ~Communicator_Handler();
  
  void SendRequest();
  
private:
  void HandleResponse(nuiHTTPRequest* pRequest, nuiHTTPResponse* pResponse);

private:
  Communicator::CommunicatorResponseDelegate mDelegate;
  nuiHTTPRequest *mpRequest;
  nuiHTTPRequest_Thread* mpThread;
};