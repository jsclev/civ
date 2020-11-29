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
         Texture *tileTexture,
         int x,
         int y,
         SDL_Rect tileClip);

    ~Tile();

//    void addLayer(TileLayer layer);

    void render();

private:
    SDL_Renderer *renderer;
    Texture *tileTexture;
    Texture *iconTexture;
    SDL_Rect tileClip;
    SDL_Rect iconClip;
    int x;
    int y;
    std::vector<TileLayer> layers;
};


#endif
