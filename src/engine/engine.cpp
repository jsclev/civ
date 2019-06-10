#include "engine.h"

#include <SDL.h>
#include <SDL_image.h>

void close(
        Tile *tiles[],
        SDL_Window *gWindow,
        SDL_Renderer *gRenderer,
        LTexture gDotTexture,
        LTexture gTileTexture) {
    //Deallocate tiles
    for (int i = 0; i < 192; ++i) {
        if (tiles[i] == NULL) {
            delete tiles[i];
            tiles[i] = NULL;
        }
    }

    //Free loaded images
    gDotTexture.free();
    gTileTexture.free();

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}