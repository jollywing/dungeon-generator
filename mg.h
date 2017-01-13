#ifndef __MAP_GENERATOR__
#define __MAP_GENERATOR__

#include "map.h"
#include "graphic.h"

class MapGenerator
{
  public:
    RogueMap * CreateMap(unsigned short style_id);
    void PrintTextMap(RogueMap *);
    void DestroyMap(RogueMap *);
};
#endif
