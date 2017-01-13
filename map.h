#ifndef __ROGUE_MAP__
#define __ROGUE_MAP__

#define MIN_HOUSE_WIDTH  5
#define MIN_HOUSE_HEIGHT 5
#define MIN_ROAD_WIDTH 3
#define MAX_ROAD_WIDTH 5

#include "graphic.h"

struct RectArea
{
    unsigned short start_x;
    unsigned short start_y;
    unsigned short end_x;
    unsigned short end_y;
};

struct Area
{
    RectArea rect;
    bool has_house;
    RectArea house;
    unsigned short connection;
};

class RogueMap
{
  public:
    unsigned short width;
    unsigned short height;
    char ** tiles;
    MapStyle style;
    unsigned short ** graphics;

    RogueMap(MapStyle *);
    ~RogueMap();
    int CreateHouses();
    void CreateRoads();
    void MakeGraphicTiles();
  private:
    Area areas[9];
    unsigned short area_width;
    unsigned short area_height;

    void ResetCharTiles();
    bool AllConnected();

    RectArea CreateHouse(RectArea &);
    void MakeHouse(RectArea &);
    void AddAreaConnection(unsigned short area_id);
    bool FindHorizonRoad(unsigned short area_id);
    bool GetHorizonRoadRect(RectArea &left, RectArea &right,
            RectArea * result);
    void MakeHorizonRoad(RectArea &r);
    bool FindVerticalRoad(unsigned short area_id);
    bool GetVerticalRoadRect(RectArea &up, RectArea &down,
            RectArea * result);
    void MakeVerticalRoad(RectArea & r);

    void FindTurnRoad(unsigned short area_id);

};
#endif
