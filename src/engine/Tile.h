#ifndef CIV_TILE_H
#define CIV_TILE_H

#include <SDL.h>

#include "Texture.h"

class Tile
{
public:
    Tile();
    Tile(SDL_Renderer *renderer, Texture *spriteSheetTexture, int x, int y, SDL_Rect clip);
    ~Tile();
    void render();
private:
    SDL_Renderer *renderer;
    Texture *spriteSheetTexture;
    SDL_Rect clip;
    int x;
    int y;
};


#endif
