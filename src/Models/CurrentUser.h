//
//  CurrentUser.h
//  YaLive
//
//  Created by Jérôme BLONDON on 24/01/13.
//
//

#pragma once

#include "nui.h"
#include "nuiHTTP.h"
#include "nuiJson.h"
#include "Object.h"

namespace Models {
  class CurrentUser: public Object
  {
  public:
    static CurrentUser* Instance();
    
  protected:
    CurrentUser();
    virtual ~CurrentUser();
    
  public:
    // Getter/setters
    const nglString &GetUsername()
    {
      return mUsername;
    }
    
    void SetUsername(const nglString& username)
    {
      mUsername = username;
    }
    
    const nglString &GetSessionId();
    void SetSessionId(const nglString& sessionId);
    
    Object* ParseObject(nuiJson::Value &data);
    bool IsLogged();
  private:
    
  private:
    nglString mUsername;
    nglString mSessionId;

    static CurrentUser* mpInstance;
  };
}