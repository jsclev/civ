#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

#include "engine/sdl_utils.h"
#include "engine/engine.h"
#include "engine/Dot.h"
#include "engine/Tile.h"
#include "engine/LTexture.h"

int main(int argc, char *args[]) {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    LTexture dotTexture;
    LTexture tileTexture;
    SDL_Rect tileClips[TOTAL_TILE_SPRITES];

    if (!init(renderer, window, SCREEN_WIDTH, SCREEN_HEIGHT)) {
        printf("Failed to initialize!\n");
    }
    else {
        SDL_RendererInfo info;
        int result = SDL_GetRendererInfo(renderer, &info);
        Tile *tileSet[TOTAL_TILES];

        if (!loadMedia(renderer, dotTexture, tileTexture, tileSet, tileClips)) {
            printf("Failed to load media!\n");
        } else {
            bool quit = false;
            SDL_Event e;
            Dot dot;
            SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_Texture *texture = loadTexture( "/Users/jcleveland/projects/civ/assets/images/tiles.png", renderer );

            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                            case SDLK_f:
                                toggleFullscreen(window);
                                break;
                        }
                    }

//                    dot.handleEvent(e);
                }

                //Move the dot
//                dot.move(tileSet, LEVEL_WIDTH, LEVEL_HEIGHT, TOTAL_TILES);
//                dot.setCamera(camera, SCREEN_WIDTH, SCREEN_HEIGHT, LEVEL_WIDTH, LEVEL_HEIGHT);

                //Clear screen
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
//                SDL_RenderClear(renderer);

//                for (int i = 0; i < TOTAL_TILES; ++i) {
//                    tileSet[i]->render(camera, tileTexture, renderer, tileClips[i]);
//                }

//                dot.render(camera, dotTexture, renderer);

                //Clear screen
                SDL_RenderClear( renderer );

                //Render texture to screen
                SDL_RenderCopy( renderer, texture, NULL, NULL );

                //Update screen
                SDL_RenderPresent(renderer);
            }
        }

        close(tileSet, window, renderer, dotTexture, tileTexture);
    }

    return 0;
}
