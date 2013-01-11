//
//  Radio.h
//  YaLive
//
//  Created by Jérôme BLONDON on 11/01/13.
//
//

#pragma once

#include "nui.h"

#include "Object.h"

namespace Models {
  class Radio: public Object
  {
  public:
    Radio() {};
    virtual ~Radio() {};
    
    const nglString &GetName()
    {
      return mName;
    }
    
    void SetName(const nglString& name)
    {
      mName = name;
    }
    
  private:
    nglString mName;
  };
}