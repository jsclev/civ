#include "TileLayer.h"

TileLayer::TileLayer(int zIndex,
                     float food,
                     float production,
                     float gold,
                     float science) :
        zIndex(zIndex),
        food(food),
        production(production),
        gold(gold),
        science(science) {

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
