#ifndef CIV_TILE_H
#define CIV_TILE_H

#include <SDL.h>
#include <vector>

#include "Texture.h"
#include "TileLayer.h"

class Tile {
public:
    Tile();

    Tile(SDL_Renderer *renderer,
         Texture *texture,
         int x,
         int y,
         SDL_Rect clip);

    ~Tile();

    void render();

private:
    SDL_Renderer *renderer;
    Texture *texture;
    SDL_Rect clip;
    int x;
    int y;
};


#endif
