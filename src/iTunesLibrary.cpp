//
//  iTunesLibrary.cpp
//  YaLive
//
//  Created by Sébastien Métrot on 12/7/12.
//
//

#include "iTunesLibrary.h"

class iTunesXMLParser: public nuiXMLParser
{
public:
  iTunesXMLParser(iTunesLibrary* pLib)
  : mpLib(pLib)
  {

  }

  bool Load(const nglPath& rPath)
  {
    nglIStream* pStream = rPath.OpenRead();

    bool res = Parse(pStream);

    delete pStream;
    return res;
  }

  virtual ~iTunesXMLParser()
  {
    // ???
  }


  // Callbacks from the SAX parser class:
  virtual void StartElement(const nuiXML_Char* name, const nuiXML_Char** atts)
  {
    NGL_OUT("Start Element %s", name);
  }

  virtual void EndElement(const nuiXML_Char* name)
  {
    NGL_OUT("End Element %s", name);
  }

  virtual void Characters(const nuiXML_Char* s, int len)
  {
    NGL_OUT("Chars %s", s);
  }

  virtual void ProcessingInstruction(const nuiXML_Char* target, const nuiXML_Char* data)
  {
    NGL_OUT("Processing instructions %s -> %s", target, data);
  }

  virtual void Comment(const nuiXML_Char* data)
  {
    NGL_OUT("Comment %s", data);
  }

protected:
  iTunesLibrary* mpLib;
};


iTunesLibrary::iTunesLibrary()
{

}

iTunesLibrary::~iTunesLibrary()
{

}

bool iTunesLibrary::LoadFromXMLFile(const nglPath& rPath)
{
  iTunesXMLParser parser(this);

  bool res = parser.Load(rPath);
  return res;
}

