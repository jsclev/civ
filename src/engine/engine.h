#ifndef CIV_ENGINE_H
#define CIV_ENGINE_H

#include "Tile.h"

void close(Tile *tiles[],SDL_Window *gWindow,
           SDL_Renderer *gRenderer,
           LTexture gDotTexture,
           LTexture gTileTexture);

#endif //CIV_ENGINE_H
