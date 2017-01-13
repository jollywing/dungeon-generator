#include <stdlib.h>
#include <stdio.h>
#include "mg.h"

RogueMap * MapGenerator::CreateMap(unsigned short style_id)
{
    MapStyle *style = GetMapStyle(style_id);
    if(!style) return NULL;
    RogueMap *map = new RogueMap(style);
    return map;
}

void MapGenerator::PrintTextMap(RogueMap *map)
{
    int i, j;
    for(i = 0; i < map->height; i++) {
        for(j = 0; j < map->width; j++) {
            printf("%c", map->tiles[i][j]);
        }
        printf("\n");
    }
}

void MapGenerator::DestroyMap(RogueMap * map)
{
    if(map) {
        delete map;
        map = NULL;
    }
}
