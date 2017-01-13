#include <stdio.h>
#include <SDL_image.h>
#include "screen.h"

GameScreen::GameScreen(unsigned short w, unsigned short h)
{
    scr_surf = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

    scr_surf = SDL_SetVideoMode(w, h, 16,
            SDL_DOUBLEBUF|SDL_HWSURFACE);
    if (scr_surf == NULL) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_FillRect(scr_surf, NULL, 0x0000ff);

    LoadTileImage(TILE_IMAGE_PATH, &tile_surf);

    SDL_WM_SetCaption("Chinese RogueLike", NULL);
    SDL_ShowCursor(0); //hide mouse pointer
    SDL_EnableKeyRepeat(100, 100); //enable key repeat

    screen_tile_cols = w / TILE_WIDTH;
    screen_tile_rows = h / TILE_HEIGHT;
    printf("screen cols: %d, screen rows: %d\n", screen_tile_cols, screen_tile_rows);
    map_offset_x = map_offset_y = 0;
}

GameScreen::~GameScreen()
{
    if(tile_surf) {
        SDL_FreeSurface(tile_surf);
        tile_surf = NULL;
    }
}

void GameScreen::Show(){
    SDL_Flip(scr_surf);
    // SDL_UpdateRect(scr_surf, 0, 0, 0, 0);
}

void GameScreen::LoadTileImage(const char *image_path,
        SDL_Surface **surf)
{
    SDL_Surface *temp = IMG_Load(image_path);
    if (temp == NULL){
        printf("IMG_Load: %s\n", IMG_GetError());
        exit(1);
    }
    *surf = SDL_DisplayFormat(temp);
    if (*surf == NULL) {
        printf("Convert surface format failed\n");
        exit(1);
    }
    SDL_BlitSurface(*surf, NULL, scr_surf, NULL);
    tile_cols = (*surf)->w / TILE_WIDTH;
    tile_rows = (*surf)->h / TILE_HEIGHT;
    printf("tile cols: %d, tile rows: %d\n", tile_cols, tile_rows);
}

void GameScreen::DrawMap(RogueMap & map)
{
    int i, j;
    unsigned short tile_id;
    SDL_Rect src, dest;
    for(j = map_offset_y; j < map_offset_y + screen_tile_rows; j++) {
        for(i = map_offset_x; i < map_offset_x + screen_tile_cols;
            i++) {
            tile_id = map.graphics[j][i];
            src.x = tile_id % tile_cols * TILE_WIDTH;
            src.y = tile_id / tile_cols * TILE_HEIGHT;
            dest.x = (i - map_offset_x) * TILE_WIDTH;
            dest.y = (j - map_offset_y) * TILE_HEIGHT;
            src.w = dest.w = TILE_WIDTH;
            src.h = dest.h = TILE_HEIGHT;
            SDL_BlitSurface(tile_surf, &src, scr_surf, &dest);
            // printf("blit from %d, %d to %d, %d\n",
            //         src.x, src.y, dest.x, dest.y);
        }
    }
}

void GameScreen::ScrollMap(RogueMap & map, unsigned short x, unsigned short y)
{
    unsigned short max_offset_x = map.width - screen_tile_cols;
    unsigned short max_offset_y = map.height - screen_tile_rows;
    unsigned short new_x = map_offset_x + x;
    unsigned short new_y = map_offset_y + y;
    if (new_x >=0 && new_x <= max_offset_x)
        map_offset_x = new_x;
    if (new_y >=0 && new_y <= max_offset_y)
        map_offset_y = new_y;

}
