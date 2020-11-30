#ifndef CIV_BUTTON_H
#define CIV_BUTTON_H

#include <SDL.h>
#include "Texture.h"
#include "Tile.h"

class Button {
public:
    Button(SDL_Renderer *renderer,
           Texture *texture,
           int x,
           int y,
           SDL_Rect clip);

    void handleEvent(SDL_Event *e, Tile tiles[], SDL_Rect tileClips[]);

    void render();

private:
    SDL_Point position;
    SDL_Renderer *renderer;
    Texture *texture;
    SDL_Rect clip;
};

#endif
