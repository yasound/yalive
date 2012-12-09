//
//  iTunesLibrary.cpp
//  YaLive
//
//  Created by Sébastien Métrot on 12/7/12.
//
//

#include "iTunesLibrary.h"
#include "nuiStopWatch.h"

class iTunesXMLParser
{
public:
  iTunesXMLParser(iTunesLibrary* pLib)
  : mpLib(pLib)
  {

  }

  bool Load(const nglPath& rPath)
  {
    nglIStream* pStream = rPath.OpenRead();


    bool res;

    nuiXML xml;
    {
      nuiStopWatch sw("Load iTunes XML");
      res = xml.Load(*pStream);

      sw.AddIntermediate("XML");

      for (int32 i = 0; i < xml.GetChildrenCount(); i++)
      {
        nuiXMLNode* pNode = xml.GetChild(i);
        if (pNode->GetName() == "dict")
        {
          for (int j = 0; j < pNode->GetChildrenCount(); j++)
          {
            nuiXMLNode* pChild = pNode->GetChild(j);

            if (pChild->GetName() == "key")
            {
              nuiXMLNode* pName = pChild->GetChild(0);
              nglString name = pName->GetValue();

              if (name == "Tracks")
              {
                // look for the tracks dict:
                j++;
                nuiXMLNode* pTracks = pNode->GetChild(j);

                for (int k = 0; k < pTracks->GetChildrenCount() / 2; k++)
                {
                  nuiXMLNode* pKey = pTracks->GetChild(k * 2);
                  nuiXMLNode* pTrack = pTracks->GetChild(k * 2 + 1);

                  NGL_OUT("key: %s ->\n", name.GetChars());
                }
              }


            }


          }
        }
      }
    }

    delete pStream;
    return res;
  }

  virtual ~iTunesXMLParser()
  {
    // ???
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

