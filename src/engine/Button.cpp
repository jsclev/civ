#include <iostream>
using namespace std;

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

void Button::handleEvent(SDL_Event *e, std::vector<Tile> *tiles, SDL_Rect tileClips[]) {
    if (e->type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (x * 2 >= position.x &&
            x * 2 <= position.x + BUTTON_WIDTH &&
            y * 2 >= position.y &&
            y * 2 <= position.y + BUTTON_HEIGHT) {
            tiles->clear();
            for (int row = 0; row < NUM_ROWS; row++) {
                for (int col = 0; col < NUM_COLS; col++) {
                    int index = row * NUM_COLS + col;
                    int randClip = rand() % 32;
                    Tile tile = Tile(renderer,
                                     texture,
                                     col * TILE_WIDTH,
                                     row * TILE_SIZE - TILE_SIZE / 2,
                                     tileClips[randClip]);
                    tile.addLayer(TileLayer(1, 10.0, 5.0, 2.0, 1.0));

                    tiles->emplace_back(tile);
//                    icons[index] = Tile(gRenderer,
//                                    &gSpritesTexture,
//                                    col * TILE_WIDTH,
//                                    row * TILE_SIZE - TILE_SIZE / 2,
//                                    gIconClips[FOOD_ICON]);
                }
            }

            for (auto t : *tiles) {
                cout << t.getFood() << endl;
            }
        }
    }
}

void Button::render() {
    texture->render(renderer, position.x, position.y, &clip);
}
