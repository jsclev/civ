#include "Tile.h"
#include "constants.h"

Tile::Tile() :
        renderer(nullptr),
        spriteSheetTexture(nullptr),
        x(0),
        y(0),
        clip({0, 0, 0, 0}) {

}

Tile::Tile(SDL_Renderer *renderer,
           Texture *spriteSheetTexture,
           int x,
           int y,
           SDL_Rect clip) :
           renderer(renderer),
           spriteSheetTexture(spriteSheetTexture),
           x(x),
           y(y),
           clip(clip) {

}

Tile::~Tile() {

}

void Tile::render() {
    spriteSheetTexture->render(
            this->renderer,
            this->x * TILE_WIDTH,
            this->y * TILE_SIZE - TILE_SIZE / 2,
            &clip);
}
