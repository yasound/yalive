//
//  Radio.cpp
//  YaLive
//
//  Created by Jérôme BLONDON on 11/01/13.
//
//

#include "Radio.h"

Models::Object* Models::Radio::ParseObject(nuiJson::Value &data)
{
  for (uint32 i = 0; i < data.getMemberNames().size(); i++)
  {
    std::string name = data.getMemberNames()[i];
    nuiJson::Value& rVal = data[name];
    
    if (name.compare("name") == 0)
    {
      SetName(rVal.asString());
    }
  }
  return this;
}
