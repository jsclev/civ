#include "Tile.h"
#include "constants.h"

Tile::Tile() :
        renderer(nullptr),
        tileTexture(nullptr),
        x(0),
        y(0),
        tileClip({0, 0, 0, 0}) {

}

Tile::Tile(SDL_Renderer *renderer,
           Texture *tileTexture,
           int x,
           int y,
           SDL_Rect tileClip) :
        renderer(renderer),
        tileTexture(tileTexture),
        x(x),
        y(y),
        tileClip(tileClip) {

}

Tile::~Tile() = default;

//void Tile::addLayer(TileLayer layer) {
//    layers.push_back(layer);
//}

void Tile::render() {
    tileTexture->render(
            this->renderer,
            this->x * TILE_WIDTH,
            this->y * TILE_SIZE - TILE_SIZE / 2,
            &tileClip);
//    iconTexture->render(
//            this->renderer,
//            4 * ICON_WIDTH,
//            0 * ICON_HEIGHT,
//            &iconClip);
}
