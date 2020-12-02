#ifndef CIV_TILELAYER_H
#define CIV_TILELAYER_H

#include <SDL_render.h>
#include "Texture.h"

class TileLayer {
public:
    TileLayer(SDL_Renderer *renderer,
              Texture *texture,
              int x,
              int y,
              SDL_Rect clip,
              int zIndex);

    ~TileLayer();

    float getFood();

    float getProduction();

    float getGold();

    float getScience();

    int getZIndex();

    void setFood(float f);

    void setProduction(float p);

    void setGold(float g);

    void setScience(float s);

    void setZIndex(int z);

    void render();

private:
    SDL_Renderer *renderer;
    Texture *texture;
    SDL_Rect clip;
    int x;
    int y;
    int zIndex;
    float food;
    float production;
    float gold;
    float science;
};

#endif
