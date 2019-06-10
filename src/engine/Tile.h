#ifndef CIV_TILE_H
#define CIV_TILE_H

#include <SDL.h>
#include "LTexture.h"

class Tile {
public:
    //Initializes position and type
    Tile(int x, int y, int tileType);

    //Shows the tile
    void render(SDL_Rect &camera, LTexture gTileTexture, SDL_Renderer *gRenderer, SDL_Rect gTileClips[]);

    //Get the tile type
    int getType();

    //Get the collision box
    SDL_Rect getBox();

private:
    //The attributes of the tile
    SDL_Rect mBox;

    //The tile type
    int mType;
};

#endif //CIV_TILE_H
