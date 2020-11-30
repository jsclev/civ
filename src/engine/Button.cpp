#include "Button.h"
#include "constants.h"
#include "Tile.h"

Button::Button(SDL_Renderer *renderer,
               Texture *texture,
               int x,
               int y,
               SDL_Rect clip) :
        renderer(renderer),
        texture(texture),
        clip(clip) {
    position.x = x;
    position.y = y;
}

void Button::handleEvent(SDL_Event *e, Tile tiles[], SDL_Rect tileClips[]) {
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (x * 2 >= position.x &&
            x * 2 <= position.x + BUTTON_WIDTH &&
            y * 2 >= position.y &&
            y * 2 <= position.y + BUTTON_HEIGHT) {
            for (int row = 0; row < NUM_ROWS; row++) {
                for (int col = 0; col < NUM_COLS; col++) {
                    int index = row * NUM_COLS + col;
                    int randClip = rand() % 32;
                    tiles[index] = Tile(renderer,
                                        texture,
                                        col * TILE_WIDTH,
                                        row * TILE_SIZE - TILE_SIZE / 2,
                                        tileClips[randClip]);
//                    icons[index] = Tile(gRenderer,
//                                    &gSpritesTexture,
//                                    col * TILE_WIDTH,
//                                    row * TILE_SIZE - TILE_SIZE / 2,
//                                    gIconClips[FOOD_ICON]);
                }
            }
        }
    }
}

void Button::render() {
    texture->render(renderer, position.x, position.y, &clip);
}
