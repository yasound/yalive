//
//  MyRadios.h
//  YaLive
//
//  Created by Jérôme BLONDON on 11/01/13.
//
//

#pragma once

#include "nui.h"
#include "Collection.h"
#include "Object.h"

namespace Models {
  class MyRadios : public Collection
  {
  public:
    MyRadios(const nglString& sessionId);
    virtual ~MyRadios();
  
  protected:
    virtual Object* ParseObject(nuiJson::Value &object);
  };
}