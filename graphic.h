
#ifndef __GRAPHIC_DEF__
#define __GRAPHIC_DEF__

#define MAP_STYLE_COUNT 12

struct MapStyle
{
    char name[24];
    unsigned short width;
    unsigned short height;
    unsigned short blank_tile1;
    unsigned short blank_tile2;
    unsigned short blank_tile1_dark;
    unsigned short blank_tile2_dark;
    unsigned short floor_tile1;
    unsigned short floor_tile2;
    unsigned short floor_tile1_dark;
    unsigned short floor_tile2_dark;
    unsigned short wall_tile1;
    unsigned short wall_tile2;
    unsigned short wall_tile1_dark;
    unsigned short wall_tile2_dark;
};

void InitMapStyles();
MapStyle * GetMapStyle(unsigned short i);

#endif
