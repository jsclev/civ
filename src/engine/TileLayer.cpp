#include "TileLayer.h"

TileLayer::TileLayer(SDL_Renderer *renderer,
                     Texture *texture,
                     int x,
                     int y,
                     SDL_Rect clip,
                     int zIndex) :
        renderer(renderer),
        texture(texture),
        x(x),
        y(y),
        clip(clip),
        zIndex(zIndex),
        food(0.0),
        production(0.0),
        gold(0.0),
        science(0.0 ){

}

TileLayer::~TileLayer() = default;

float TileLayer::getFood() {
    return food;
}

float TileLayer::getProduction() {
    return production;
}

float TileLayer::getGold() {
    return gold;
}

float TileLayer::getScience() {
    return science;
}

int TileLayer::getZIndex() {
    return zIndex;
}

void TileLayer::setFood(float f) {
    this->food = f;
}

void TileLayer::setProduction(float p) {
    this->production = p;
}

void TileLayer::setGold(float g) {
    this->gold = g;
}

void TileLayer::setScience(float s) {
    this->science = s;
}

void TileLayer::setZIndex(int z) {
    this->zIndex = z;
}

void TileLayer::render() {
    texture->render(renderer, x, y, &clip);
}
