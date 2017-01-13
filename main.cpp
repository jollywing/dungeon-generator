#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include "mg.h"
#include "screen.h"
#include "game.h"

int main(int argc, char *argv[])
{
    Game engine;
    engine.Run();

    srand(time(0));

    InitMapStyles();
    MapGenerator mg;
    RogueMap *map = mg.CreateMap(0);
    mg.PrintTextMap(map);

    GameScreen screen(800, 640);
    screen.DrawMap(*map);
    screen.Show();
    SDL_Event event;
    while(SDL_WaitEvent(&event)) {
        if (event.key.type == SDL_KEYUP) {
            if(event.key.keysym.sym == SDLK_ESCAPE)
                break;
            if (event.key.keysym.sym == SDLK_r) {
                delete map;
                map = mg.CreateMap(0);
                screen.DrawMap(*map);
                screen.Show();
            }
        }

        if (event.key.state == SDL_PRESSED) {
            switch(event.key.keysym.sym) {
                case SDLK_LEFT:
                    screen.ScrollMap(*map, -1, 0);
                    screen.DrawMap(*map);
                    screen.Show();
                    break;
                case SDLK_RIGHT:
                    screen.ScrollMap(*map, 1, 0);
                    screen.DrawMap(*map);
                    screen.Show();
                    break;
                case SDLK_UP:
                    screen.ScrollMap(*map, 0, -1);
                    screen.DrawMap(*map);
                    screen.Show();
                    break;
                case SDLK_DOWN:
                    screen.ScrollMap(*map, 0, 1);
                    screen.DrawMap(*map);
                    screen.Show();
            }
        }
    }
    mg.DestroyMap(map);
    return 0;
}
