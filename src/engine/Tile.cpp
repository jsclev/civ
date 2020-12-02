#include "Tile.h"

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

void Tile::addLayer(TileLayer layer) {
    layers.emplace_back(layer);
}

float Tile::getFood() {
    float food = 0;

    for (auto layer: layers) {
        food += layer.getFood();
    }

    return food;
}
