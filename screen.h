#ifndef __GAME_SCREEN__
#define __GAME_SCREEN__

#include <SDL.h>
#include <SDL_ttf.h>
#include "map.h"

#define TILE_IMAGE_PATH "./tiles.png"
#define TILE_WIDTH 32
#define TILE_HEIGHT 32

class GameScreen
{
  private:
    SDL_Surface *scr_surf;
    SDL_Surface *tile_surf;
    unsigned short tile_cols;
    unsigned short tile_rows;
    unsigned short screen_tile_cols;
    unsigned short screen_tile_rows;
    unsigned short map_offset_x;
    unsigned short map_offset_y;
  public:
    unsigned short width;
    unsigned short height;
    void DrawMap(RogueMap &);
    void ScrollMap(RogueMap &, unsigned short x, unsigned short y);
    void Show();
    GameScreen(unsigned short w, unsigned short h);
    ~GameScreen();
  private:
    void LoadTileImage(const char* image_path, SDL_Surface **surf);
};
#endif
