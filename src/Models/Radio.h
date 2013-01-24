//
//  Radio.h
//  YaLive
//
//  Created by Jérôme BLONDON on 11/01/13.
//
//

#pragma once

#include "nui.h"
#include "nuiJson.h"
#include "Object.h"

namespace Models {
  class Radio: public Object
  {
  public:
    Radio() {};
    virtual ~Radio() {};
    
    // Getter/setters
    const nglString &GetName()
    {
      return mName;
    }
    
    void SetName(const nglString& name)
    {
      mName = name;
    }
    Object* ParseObject(nuiJson::Value &data);
    
  private:
    nglString mName;
  };
}