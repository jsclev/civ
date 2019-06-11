#ifndef CIV_ENGINE_H
#define CIV_ENGINE_H

#include "Tile.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;


const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

void close(Tile *tiles[],SDL_Window *gWindow,
           SDL_Renderer *gRenderer,
           LTexture gDotTexture,
           LTexture gTileTexture);

bool loadMedia(SDL_Renderer *renderer, LTexture dotTexture, LTexture tileTexture, Tile *tiles[],
        SDL_Rect clips[]);
bool setTiles(Tile *tiles[], SDL_Rect clips[]);

#endif
