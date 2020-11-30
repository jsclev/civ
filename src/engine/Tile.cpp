#include "Tile.h"
#include "constants.h"

Tile::Tile() :
        renderer(nullptr),
        texture(nullptr),
        x(0),
        y(0),
        clip({0, 0, 0, 0}) {

}

Tile::Tile(SDL_Renderer *renderer,
           Texture *texture,
           int x,
           int y,
           SDL_Rect clip) :
        renderer(renderer),
        texture(texture),
        x(x),
        y(y),
        clip(clip) {

}

Tile::~Tile() = default;

void Tile::render() {
    texture->render(renderer, x, y, &clip);
}
