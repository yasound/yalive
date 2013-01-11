//
//  MyRadios.cpp
//  YaLive
//
//  Created by Jérôme BLONDON on 11/01/13.
//
//

#include "MyRadios.h"
#include "Radio.h"

Models::MyRadios::MyRadios(const nglString& sessionId)
:Models::Collection(sessionId)
{
  mAuth = true;
  mApi = "/api/v1/my_radios/";
}

Models::MyRadios::~MyRadios()
{
  
}

Models::Object* Models::MyRadios::ParseObject(nuiJson::Value &object)
{
  Radio *pRadio = new Radio();
  for (uint32 i = 0; i < object.getMemberNames().size(); i++)
  {
    std::string name = object.getMemberNames()[i];
    nuiJson::Value& rVal = object[name];
    
    if (name.compare("name") == 0)
    {
      pRadio->SetName(rVal.asString());
    }
  }
  return pRadio;
}
