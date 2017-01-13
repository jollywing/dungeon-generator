#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include "map.h"
#include "general.h"

using namespace std;

RogueMap::RogueMap(MapStyle *ms)
{
    int i, j, k;
    style = *ms;
    this->width = style.width;
    this->height = style.height;
    printf("try to create map: w:%d, h:%d\n", width, height);

    // create tiles
    this->tiles = new char*[this->height];
    if (!this->tiles) {
        printf("No enough memory for creating map.\n");
        exit(1);
    }

    for (i = 0; i < this->height; i++) {
        this->tiles[i] = new char[this->width];
        if (!this->tiles[i]) {
            printf("No enough memory for creating map.\n");
            exit(1);
        }
    }

    // alloc memory for graphics
    graphics = new unsigned short *[height];
    if (!graphics) {
        printf("No enough memory for graphic tiles.\n");
        exit(1);
    }

    for(i =0; i < height; i++) {
        graphics[i] = new unsigned short[width];
        if(!graphics[i]) {
            printf("No enough memory for graphic tiles.\n");
            exit(1);
        }
    }

    // split the whole map into 9 areas
    area_width = this->width / 3;
    area_height = this->height / 3;
    if(area_width < MIN_HOUSE_WIDTH ||
            area_height < MIN_HOUSE_HEIGHT) {
        printf("Given map size is too small\n");
        exit(1);
    }

    for(i = 0; i < 9; i++) {
        areas[i].has_house = false;
        areas[i].connection = 0;
        areas[i].rect.start_x = (i % 3) * area_width;
        areas[i].rect.start_y = (i / 3) * area_height;
        areas[i].rect.end_x = areas[i].rect.start_x + area_width -1;
        areas[i].rect.end_y = areas[i].rect.start_y + area_height -1;
        // printf("Area %d: %d,%d; %d,%d\n", i, areas[i].rect.start_x,
        //         areas[i].rect.start_y, areas[i].rect.end_x,
        //         areas[i].rect.end_y);
    }

    // 初始化
    bool connected = false;
    int loop = 0;
    while(!connected) {
        loop ++;
        ResetCharTiles();
        CreateHouses();
        CreateRoads();
        connected = AllConnected();
    }
    printf("I took %d loops to create a good map.\n", loop);

    MakeGraphicTiles();
}

void RogueMap::ResetCharTiles()
{
    int i, j;
    for (i = 0; i < this->height; i++)
        for (j = 0; j < this->width; j++){
            this->tiles[i][j] = '-';
        }
}

bool RogueMap::AllConnected()
{
    int i;
    for(i = 0; i < 9; i++) {
        if(areas[i].has_house && areas[i].connection < 2)
            return false;
    }
    return true;
}

RogueMap::~RogueMap()
{
    int i;

    if(tiles) {
        for(i = 0; i < this->height; i++)
            if(tiles[i])
                delete [] this->tiles[i];

        delete [] this->tiles;
        this->tiles = NULL;
    }

    if(graphics) {
        for(i = 0; i < this->height; i++)
            if(graphics[i])
                delete [] graphics[i];

        delete [] graphics;
        graphics = NULL;
    }

}

int RogueMap::CreateHouses()
{
    int i;
    int house_count;
    int loop = 0;
    do {
        loop ++;
        house_count = 0;
        for(i = 0; i < 9; i++) {
            areas[i].has_house = false;
            areas[i].connection = 0;

            if (rogue::Random(0, 100) <= 90){
                areas[i].has_house = true;
                house_count ++;
            }
        }
    } while(house_count < 6);

    printf("I take %d loops to create %d houses\n",
            loop, house_count);

    for(i = 0; i < 9; i++) {
        if(areas[i].has_house) {
            areas[i].house = CreateHouse(areas[i].rect);
            MakeHouse(areas[i].house);
        }
    }

    return house_count;
}

void RogueMap::MakeHouse(RectArea & r)
{
    for(int j = r.start_y; j <= r.end_y; j++) {
        for(int k = r.start_x; k <= r.end_x; k++) {
            if (j == r.start_y || j == r.end_y ||
                    k == r.start_x || k == r.end_x)
                this->tiles[j][k] = '#';
            else
                this->tiles[j][k] = '.';
        }
    }
}

RectArea RogueMap::CreateHouse(RectArea & area)
{
    unsigned short house_width =
            rogue::Random(MIN_HOUSE_WIDTH, area_width - 1);
    unsigned short house_height =
            rogue::Random(MIN_HOUSE_HEIGHT, area_height - 1);

    RectArea r;
    r.start_x = area.start_x +
            rogue::Random(0, area_width - house_width);
    r.start_y = area.start_y +
            rogue::Random(0, area_height - house_height);
    r.end_x = r.start_x + house_width - 1;
    r.end_y = r.start_y + house_height - 1;
    // printf("new house: %d, %d; %d, %d\n", r.start_x, r.start_y,
    //         r.end_x, r.end_y);
    return r;
}

void RogueMap::CreateRoads()
{
    for(int i = 0; i < 9; i++)
        AddAreaConnection(i);
}

void RogueMap::AddAreaConnection(unsigned short area_id)
{

    // 一半的概率建立横向连接，一半的概率建立纵向连接
    if(rogue::Random(0, 1) == 0) {
        FindHorizonRoad(area_id);
        FindVerticalRoad(area_id);
    }
    else {
        FindVerticalRoad(area_id);
        FindHorizonRoad(area_id);
    }
    FindTurnRoad(area_id);
}

bool RogueMap::FindHorizonRoad(unsigned short area_id)
{
    if(!areas[area_id].has_house)
        return true;

    if(areas[area_id].connection > 1)
        return true;

    int i;
    unsigned short area_to_connect = -1;
    RectArea road_rect;
    bool has_road = false;

    unsigned short t = area_id % 3;
    switch(t) {
        case 0:
            for(i = 1; i < 3; i++) {
                if (areas[area_id + i].has_house) {
                    has_road = GetHorizonRoadRect(
                        areas[area_id].house,
                        areas[area_id + i].house, &road_rect);
                    if(has_road) {
                        area_to_connect = area_id + i;
                        break;
                    }
                }
            }
            break;
        case 1:
            unsigned short ids[2];
            if(rogue::Random(0, 1) == 0) {
                ids[0] = area_id - 1;
                ids[1] = area_id + 1;
            }
            else {
                ids[0] = area_id + 1;
                ids[1] = area_id - 1;
            }
            for(i = 0; i < 2; i++) {
                if (areas[ids[i]].has_house) {
                    if (ids[i] < area_id)
                        has_road = GetHorizonRoadRect(
                            areas[ids[i]].house,
                            areas[area_id].house, &road_rect);
                    else
                        has_road = GetHorizonRoadRect(
                            areas[area_id].house,
                            areas[ids[i]].house, &road_rect);
                    if(has_road) {
                        area_to_connect = ids[i];
                        break;
                    }
                }
            }

            break;
        case 2:
            for(i = 1; i < 3; i++) {
                if(areas[area_id - i].has_house) {
                    has_road = GetHorizonRoadRect(
                        areas[area_id - i].house,
                        areas[area_id].house, &road_rect);
                    if(has_road) {
                        area_to_connect = area_id -i;
                        break;
                    }
                }
            }
            break;
    }
    if(has_road) {
        areas[area_id].connection += 1;
        areas[area_to_connect].connection +=1;
        MakeHorizonRoad(road_rect);
        return true;
    }
    else
        return false;
}

bool RogueMap::GetHorizonRoadRect(RectArea &left, RectArea &right,
        RectArea * result)
{
    unsigned short max_top =
            rogue::Max(left.start_y, right.start_y);
    unsigned short min_bottom =
            rogue::Min(left.end_y, right.end_y);
    unsigned short crossed = min_bottom - max_top;
    if ( crossed < MIN_ROAD_WIDTH)
        return false;
    else {
        unsigned max_road_width =
                rogue::Min(crossed, MAX_ROAD_WIDTH);
        unsigned short road_width =
                rogue::Random(MIN_ROAD_WIDTH, max_road_width);
        result->start_x = left.end_x;
        result->end_x = right.start_x;
        result->start_y =
                rogue::Random(max_top, min_bottom - road_width);
        result->end_y = result->start_y + road_width - 1;
        printf("Find horizon road: %d, %d; %d, %d\n",
                result->start_x, result->start_y,
                result->end_x, result->end_y);
        return true;
    }
}

void RogueMap::MakeHorizonRoad(RectArea & r)
{
    int i, j;
    for (i = r.start_y; i <= r.end_y; i++) {
        for(j = r.start_x; j <= r.end_x; j++) {
            if(i == r.start_y || i == r.end_y) {
                if (tiles[i][j] != '.')
                    tiles[i][j] = '#';
            }
            else
                tiles[i][j] = '.';
        }
    }
}

bool RogueMap::FindVerticalRoad(unsigned short area_id)
{
    if(!areas[area_id].has_house)
        return true;

    if(areas[area_id].connection > 1)
        return true;


    int i;
    unsigned short area_to_connect = -1;
    RectArea road_rect;
    bool has_road = false;

    unsigned short t = area_id / 3;
    switch(t) {
        case 0:
            for(i = 1; i < 3; i++) {
                if (areas[area_id + i * 3].has_house) {
                    has_road = GetVerticalRoadRect(
                        areas[area_id].house,
                        areas[area_id + i*3].house, &road_rect);
                    if(has_road) {
                        area_to_connect = area_id + i*3;
                        break;
                    }
                }
            }
            break;
        case 1:
            unsigned short ids[2];
            if(rand() % 2 == 0) {
                ids[0] = area_id - 3;
                ids[1] = area_id + 3;
            }
            else {
                ids[0] = area_id + 3;
                ids[1] = area_id - 3;
            }

            for(i = 0; i < 2; i++) {
                if (areas[ids[i]].has_house) {
                    if (ids[i] < area_id)
                        has_road = GetVerticalRoadRect(
                            areas[ids[i]].house,
                            areas[area_id].house, &road_rect);
                    else
                        has_road = GetVerticalRoadRect(
                            areas[area_id].house,
                            areas[ids[i]].house, &road_rect);
                    if(has_road) {
                        area_to_connect = ids[i];
                        break;
                    }
                }
            }

            break;
        case 2:
            for(i = 1; i < 3; i++) {
                if(areas[area_id - i*3].has_house) {
                    has_road = GetVerticalRoadRect(
                        areas[area_id - i*3].house,
                        areas[area_id].house, &road_rect);
                    if(has_road) {
                        area_to_connect = area_id -i*3;
                        break;
                    }
                }
            }
    }
    if(has_road) {
        areas[area_id].connection += 1;
        areas[area_to_connect].connection +=1;
        MakeVerticalRoad(road_rect);
        return true;
    }
    else
        return false;

}

bool RogueMap::GetVerticalRoadRect(RectArea &up, RectArea &down,
        RectArea * result)
{
    unsigned short max_left = rogue::Max(up.start_x, down.start_x);
    unsigned short min_right = rogue::Min(up.end_x, down.end_x);
    unsigned short crossed = min_right - max_left;
    if ( crossed < MIN_ROAD_WIDTH)
        return false;
    else {
        unsigned max_road_width =
                rogue::Min(crossed, MAX_ROAD_WIDTH);
        unsigned short road_width =
                rogue::Random(MIN_ROAD_WIDTH, max_road_width);
        result->start_x =
                rogue::Random(max_left, min_right - road_width);
        result->start_y = up.end_y;
        result->end_x = result->start_x + road_width - 1;
        result->end_y = down.start_y;
        return true;
    }

}

void RogueMap::MakeVerticalRoad(RectArea & r)
{
    int i, j;
    for (i = r.start_y; i <= r.end_y; i++) {
        for(j = r.start_x; j <= r.end_x; j++) {
            if(j == r.start_x || j == r.end_x){
                if(tiles[i][j] != '.')
                    tiles[i][j] = '#';
            }
            else
                tiles[i][j] = '.';
        }
    }
}

void RogueMap::FindTurnRoad(unsigned short area_id) {
    if(!areas[area_id].has_house)
        return;
    if(areas[area_id].connection > 1)
        return;

    // select a house to connect
    int i;
    vector<unsigned short> v;
    for(i = 0; i < 9; i++) {
        if (i % 3 == area_id % 3 || i / 3 == area_id / 3 ||
            !areas[i].has_house)
            continue;
        v.push_back(i);
    }
    random_shuffle(v.begin(), v.end());

    if(v.size() == 0)
        return;
    unsigned short area_to_connect = v.at(0);

    // calculate hroad_width
    unsigned short hroad_width;
    unsigned short given_house_height =
            areas[area_id].house.end_y -
            areas[area_id].house.start_y + 1;
    unsigned short max_hroad_width =
            rogue::Max(given_house_height, MAX_ROAD_WIDTH);
    hroad_width = rogue::Random(MIN_ROAD_WIDTH, max_hroad_width);

    // calculate vertical road width
    unsigned short vroad_width;
    unsigned short object_house_width =
            areas[area_to_connect].house.end_x -
            areas[area_to_connect].house.start_x + 1;
    unsigned short max_vroad_width =
            rogue::Min(object_house_width, MAX_ROAD_WIDTH);
    vroad_width = rogue::Random(MIN_ROAD_WIDTH, max_vroad_width);


    RectArea hroad_rect, vroad_rect;

    hroad_rect.start_y =
            rogue::Random(areas[area_id].house.start_y,
                    areas[area_id].house.end_y - hroad_width);
    hroad_rect.end_y = hroad_rect.start_y + hroad_width - 1;

    // make horizon road, 目标在右边
    if (areas[area_id].rect.end_x <
            areas[area_to_connect].rect.end_x) {

        vroad_rect.start_x = areas[area_to_connect].house.start_x;
        vroad_rect.end_x = vroad_rect.start_x + vroad_width - 1;

        hroad_rect.start_x = areas[area_id].house.end_x;
        hroad_rect.end_x = vroad_rect.end_x - 1;
    }
    else {
        vroad_rect.start_x = areas[area_to_connect].house.end_x -
                vroad_width + 1;
        vroad_rect.end_x = areas[area_to_connect].house.end_x;
        hroad_rect.start_x = vroad_rect.start_x + 1;
        hroad_rect.end_x = areas[area_id].house.start_x;
    }

    MakeHorizonRoad(hroad_rect);

    // make vertical road
    // if 目标在下边
    if (areas[area_id].rect.end_y <
            areas[area_to_connect].rect.end_y) {
        vroad_rect.start_y = hroad_rect.start_y + 1;
        vroad_rect.end_y = areas[area_to_connect].house.start_y;
    }
    else {                      // 目标在上面
        vroad_rect.start_y = areas[area_to_connect].house.end_y;
        vroad_rect.end_y = hroad_rect.end_y - 1;
    }
    MakeVerticalRoad(vroad_rect);

    areas[area_id].connection += 1;
    areas[area_to_connect].connection += 1;
}

void RogueMap::MakeGraphicTiles()
{
    int i, j;
    unsigned short tile_id;

    for(j = 0; j < height; j++){
        for(i = 0; i < width; i++) {
            switch(tiles[j][i]) {
                case '-':
                    if(rand() % 100 < 90)
                        tile_id = style.blank_tile1;
                    else
                        tile_id = style.blank_tile2;
                    break;
                case '#':
                    if(rand() % 100 < 70)
                        tile_id = style.wall_tile1;
                    else
                        tile_id = style.wall_tile2;
                    break;
                case '.':
                    if(rand() % 100 < 90)
                        tile_id = style.floor_tile1;
                    else
                        tile_id = style.floor_tile2;
                    break;
                default:
                    tile_id = style.blank_tile1;

            }
            graphics[j][i] = tile_id;
        }
    }
}
