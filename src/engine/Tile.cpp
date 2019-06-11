
#include "Tile.h"
#include "sdl_utils.h"
#include "LTexture.h"

Tile::Tile(int x, int y, int tileType) {
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

    //Set the collision box
    mBox.w = 80;
    mBox.h = 80;

    //Get the tile type
    mType = tileType;
}

void Tile::render(SDL_Rect &camera, LTexture gTileTexture, SDL_Renderer* renderer, SDL_Rect clip) {
    //If the tile is on screen
    if (checkCollision(camera, mBox)) {
        //Show the tile
        gTileTexture.render(mBox.x - camera.x, mBox.y - camera.y, renderer, &clip);
    }
}

int Tile::getType() {
    return mType;
}

SDL_Rect Tile::getBox() {
    return mBox;
}