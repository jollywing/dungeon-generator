#include <stdlib.h>
#include "graphic.h"

static MapStyle map_styles[MAP_STYLE_COUNT];

void InitMapStyles() {
    // 武当山
    map_styles[0].width = 60;
    map_styles[0].height = 45;
    map_styles[0].blank_tile1 = 20;
    map_styles[0].blank_tile1_dark = 21;
    map_styles[0].blank_tile2 = 0;
    map_styles[0].blank_tile2_dark = 1;
    map_styles[0].floor_tile1 = 2;
    map_styles[0].floor_tile1_dark = 4;
    map_styles[0].floor_tile2 = 3;
    map_styles[0].floor_tile2_dark = 4;
    map_styles[0].wall_tile1 = 8;
    map_styles[0].wall_tile1_dark = 9;
    map_styles[0].wall_tile2 = 7;
    map_styles[0].wall_tile2_dark = 9;

    // 青城山
    map_styles[1].width = 48;
    map_styles[1].height = 42;
    map_styles[1].blank_tile1 = 5;
    map_styles[1].blank_tile1_dark = 6;
    map_styles[1].blank_tile2 = 0;
    map_styles[1].blank_tile2_dark = 1;
    map_styles[1].floor_tile1 = 25;
    map_styles[1].floor_tile1_dark = 27;
    map_styles[1].floor_tile2 = 26;
    map_styles[1].floor_tile2_dark = 27;
    map_styles[1].wall_tile1 = 22;
    map_styles[1].wall_tile1_dark = 24;
    map_styles[1].wall_tile2 = 23;
    map_styles[1].wall_tile2_dark = 24;

    // 大沙漠
    map_styles[2].width = 72;
    map_styles[2].height = 42;
    map_styles[2].blank_tile1 = 28;
    map_styles[2].blank_tile1_dark = 30;
    map_styles[2].blank_tile2 = 12;
    map_styles[2].blank_tile2_dark = 13;
    map_styles[2].floor_tile1 = 28;
    map_styles[2].floor_tile1_dark = 30;
    map_styles[2].floor_tile2 = 29;
    map_styles[2].floor_tile2_dark = 30;
    map_styles[2].wall_tile1 = 12;
    map_styles[2].wall_tile1_dark = 13;
    map_styles[2].wall_tile2 = 10;
    map_styles[2].wall_tile2_dark = 11;

    // 雪山
    map_styles[3].width = 60;
    map_styles[3].height = 45;
    map_styles[3].blank_tile1 = 14;
    map_styles[3].blank_tile1_dark = 15;
    map_styles[3].blank_tile2 = 16;
    map_styles[3].blank_tile2_dark = 17;
    map_styles[3].floor_tile1 = 33;
    map_styles[3].floor_tile1_dark = 35;
    map_styles[3].floor_tile2 = 34;
    map_styles[3].floor_tile2_dark = 35;
    map_styles[3].wall_tile1 = 31;
    map_styles[3].wall_tile1_dark = 36;
    map_styles[3].wall_tile2 = 32;
    map_styles[3].wall_tile2_dark = 36;

    // 海岛
    map_styles[4].width = 60;
    map_styles[4].height = 45;
    map_styles[4].blank_tile1 = 18;
    map_styles[4].blank_tile1_dark = 19;
    map_styles[4].blank_tile2 = 18;
    map_styles[4].blank_tile2_dark = 19;
    map_styles[4].floor_tile1 = 40;
    map_styles[4].floor_tile1_dark = 42;
    map_styles[4].floor_tile2 = 41;
    map_styles[4].floor_tile2_dark = 42;
    map_styles[4].wall_tile1 = 37;
    map_styles[4].wall_tile1_dark = 39;
    map_styles[4].wall_tile2 = 38;
    map_styles[4].wall_tile2_dark = 39;

    // 草原
    map_styles[5].width = 60;
    map_styles[5].height = 45;
    map_styles[5].blank_tile1 = 43;
    map_styles[5].blank_tile1_dark = 44;
    map_styles[5].blank_tile2 = 47;
    map_styles[5].blank_tile2_dark = 46;
    map_styles[5].floor_tile1 = 43;
    map_styles[5].floor_tile1_dark = 44;
    map_styles[5].floor_tile2 = 43;
    map_styles[5].floor_tile2_dark = 44;
    map_styles[5].wall_tile1 = 45;
    map_styles[5].wall_tile1_dark = 46;
    map_styles[5].wall_tile2 = 48;
    map_styles[5].wall_tile2_dark = 49;

    // 森林
    map_styles[6].width = 60;
    map_styles[6].height = 45;
    map_styles[6].blank_tile1 = 45;
    map_styles[6].blank_tile1_dark = 46;
    map_styles[6].blank_tile2 = 43;
    map_styles[6].blank_tile2_dark = 44;
    map_styles[6].floor_tile1 = 43;
    map_styles[6].floor_tile1_dark = 44;
    map_styles[6].floor_tile2 = 43;
    map_styles[6].floor_tile2_dark = 44;
    map_styles[6].wall_tile1 = 47;
    map_styles[6].wall_tile1_dark = 46;
    map_styles[6].wall_tile2 = 48;
    map_styles[6].wall_tile2_dark = 49;

}

MapStyle * GetMapStyle(unsigned short i)
{
    if(i < 0 || i >= MAP_STYLE_COUNT)
        return NULL;
    return &(map_styles[i]);
}
