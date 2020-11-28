#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <string>
#include <sstream>

#include "engine/Texture.h"
#include "engine/Timer.h"
#include "engine/Tile.h"
#include "engine/constants.h"

bool init();
bool loadMedia();
void close();

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;
TTF_Font *gFont = nullptr;
SDL_Rect gSpriteClips[128];
Texture gSpriteSheetTexture;
Texture gFPSTextTexture;

bool init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! %s\n", SDL_GetError());
        success = false;
    } else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }

        Uint32 flags = SDL_WINDOW_ALLOW_HIGHDPI;
        gWindow = SDL_CreateWindow("Civ",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH,
                                   SCREEN_HEIGHT, flags);
        if (gWindow == nullptr) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == nullptr) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }

                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia() {
    bool success = true;

    gFont = TTF_OpenFont("assets/fonts/lazy.ttf", 28);
    if (gFont == nullptr) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }

    if (!gSpriteSheetTexture.loadFromFile(
            gRenderer,
            "assets/images/tiles/painted_terrain_tiles_basic_256x384_sheet.png")) {
        printf("Failed to load sprite sheet texture!\n");
        success = false;
    } else {
        // Grass tiles
        gSpriteClips[GRASS1_TILE].x = TILE_WIDTH * 0;
        gSpriteClips[GRASS1_TILE].y = 0;
        gSpriteClips[GRASS1_TILE].w = TILE_WIDTH;
        gSpriteClips[GRASS1_TILE].h = TILE_HEIGHT;
        SDL_Rect r;
        r.x = TILE_WIDTH * 0;
        r.y = 0;
        r.w = TILE_WIDTH;
        r.h = TILE_HEIGHT;
//        tiles[0] = new Tile(gRenderer, gSpriteSheetTexture, 0, 0, &r);

        gSpriteClips[GRASS2_TILE].x = TILE_WIDTH * 1;
        gSpriteClips[GRASS2_TILE].y = 0;
        gSpriteClips[GRASS2_TILE].w = TILE_WIDTH;
        gSpriteClips[GRASS2_TILE].h = TILE_HEIGHT;

        gSpriteClips[GRASS3_TILE].x = TILE_WIDTH * 2;
        gSpriteClips[GRASS3_TILE].y = 0;
        gSpriteClips[GRASS3_TILE].w = TILE_WIDTH;
        gSpriteClips[GRASS3_TILE].h = TILE_HEIGHT;

        gSpriteClips[GRASS4_TILE].x = TILE_WIDTH * 3;
        gSpriteClips[GRASS4_TILE].y = 0;
        gSpriteClips[GRASS4_TILE].w = TILE_WIDTH;
        gSpriteClips[GRASS4_TILE].h = TILE_HEIGHT;

        // Water tiles
        gSpriteClips[WATER1_TILE].x = TILE_WIDTH * 4;
        gSpriteClips[WATER1_TILE].y = 0;
        gSpriteClips[WATER1_TILE].w = TILE_WIDTH;
        gSpriteClips[WATER1_TILE].h = TILE_HEIGHT;

        gSpriteClips[WATER2_TILE].x = TILE_WIDTH * 5;
        gSpriteClips[WATER2_TILE].y = 0;
        gSpriteClips[WATER2_TILE].w = TILE_WIDTH;
        gSpriteClips[WATER2_TILE].h = TILE_HEIGHT;

        gSpriteClips[WATER3_TILE].x = TILE_WIDTH * 6;
        gSpriteClips[WATER3_TILE].y = 0;
        gSpriteClips[WATER3_TILE].w = TILE_WIDTH;
        gSpriteClips[WATER3_TILE].h = TILE_HEIGHT;

        gSpriteClips[WATER4_TILE].x = TILE_WIDTH * 7;
        gSpriteClips[WATER4_TILE].y = 0;
        gSpriteClips[WATER4_TILE].w = TILE_WIDTH;
        gSpriteClips[WATER4_TILE].h = TILE_HEIGHT;

        // Mountain tiles
        gSpriteClips[MOUNTAIN1_TILE].x = TILE_WIDTH * 0;
        gSpriteClips[MOUNTAIN1_TILE].y = TILE_HEIGHT;
        gSpriteClips[MOUNTAIN1_TILE].w = TILE_WIDTH;
        gSpriteClips[MOUNTAIN1_TILE].h = TILE_HEIGHT;

        gSpriteClips[MOUNTAIN2_TILE].x = TILE_WIDTH * 1;
        gSpriteClips[MOUNTAIN2_TILE].y = TILE_HEIGHT;
        gSpriteClips[MOUNTAIN2_TILE].w = TILE_WIDTH;
        gSpriteClips[MOUNTAIN2_TILE].h = TILE_HEIGHT;

        gSpriteClips[MOUNTAIN3_TILE].x = TILE_WIDTH * 2;
        gSpriteClips[MOUNTAIN3_TILE].y = TILE_HEIGHT;
        gSpriteClips[MOUNTAIN3_TILE].w = TILE_WIDTH;
        gSpriteClips[MOUNTAIN3_TILE].h = TILE_HEIGHT;

        gSpriteClips[MOUNTAIN4_TILE].x = TILE_WIDTH * 3;
        gSpriteClips[MOUNTAIN4_TILE].y = TILE_HEIGHT;
        gSpriteClips[MOUNTAIN4_TILE].w = TILE_WIDTH;
        gSpriteClips[MOUNTAIN4_TILE].h = TILE_HEIGHT;

        gSpriteClips[DESERT1_TILE].x = TILE_WIDTH * 4;
        gSpriteClips[DESERT1_TILE].y = TILE_HEIGHT;
        gSpriteClips[DESERT1_TILE].w = TILE_WIDTH;
        gSpriteClips[DESERT1_TILE].h = TILE_HEIGHT;

        gSpriteClips[DESERT2_TILE].x = TILE_WIDTH * 5;
        gSpriteClips[DESERT2_TILE].y = TILE_HEIGHT;
        gSpriteClips[DESERT2_TILE].w = TILE_WIDTH;
        gSpriteClips[DESERT2_TILE].h = TILE_HEIGHT;

        gSpriteClips[DESERT3_TILE].x = TILE_WIDTH * 6;
        gSpriteClips[DESERT3_TILE].y = TILE_HEIGHT;
        gSpriteClips[DESERT3_TILE].w = TILE_WIDTH;
        gSpriteClips[DESERT3_TILE].h = TILE_HEIGHT;

        gSpriteClips[DESERT4_TILE].x = TILE_WIDTH * 7;
        gSpriteClips[DESERT4_TILE].y = TILE_HEIGHT;
        gSpriteClips[DESERT4_TILE].w = TILE_WIDTH;
        gSpriteClips[DESERT4_TILE].h = TILE_HEIGHT;

        gSpriteClips[FOREST1_TILE].x = TILE_WIDTH * 0;
        gSpriteClips[FOREST1_TILE].y = TILE_HEIGHT * 2;
        gSpriteClips[FOREST1_TILE].w = TILE_WIDTH;
        gSpriteClips[FOREST1_TILE].h = TILE_HEIGHT;

        gSpriteClips[FOREST2_TILE].x = TILE_WIDTH * 1;
        gSpriteClips[FOREST2_TILE].y = TILE_HEIGHT * 2;
        gSpriteClips[FOREST2_TILE].w = TILE_WIDTH;
        gSpriteClips[FOREST2_TILE].h = TILE_HEIGHT;

        gSpriteClips[FOREST3_TILE].x = TILE_WIDTH * 2;
        gSpriteClips[FOREST3_TILE].y = TILE_HEIGHT * 2;
        gSpriteClips[FOREST3_TILE].w = TILE_WIDTH;
        gSpriteClips[FOREST3_TILE].h = TILE_HEIGHT;

        gSpriteClips[FOREST4_TILE].x = TILE_WIDTH * 3;
        gSpriteClips[FOREST4_TILE].y = TILE_HEIGHT * 2;
        gSpriteClips[FOREST4_TILE].w = TILE_WIDTH;
        gSpriteClips[FOREST4_TILE].h = TILE_HEIGHT;

        gSpriteClips[DIRT1_TILE].x = TILE_WIDTH * 0;
        gSpriteClips[DIRT1_TILE].y = TILE_HEIGHT * 3;
        gSpriteClips[DIRT1_TILE].w = TILE_WIDTH;
        gSpriteClips[DIRT1_TILE].h = TILE_HEIGHT;

        gSpriteClips[DIRT2_TILE].x = TILE_WIDTH * 1;
        gSpriteClips[DIRT2_TILE].y = TILE_HEIGHT * 3;
        gSpriteClips[DIRT2_TILE].w = TILE_WIDTH;
        gSpriteClips[DIRT2_TILE].h = TILE_HEIGHT;

        gSpriteClips[DIRT3_TILE].x = TILE_WIDTH * 2;
        gSpriteClips[DIRT3_TILE].y = TILE_HEIGHT * 3;
        gSpriteClips[DIRT3_TILE].w = TILE_WIDTH;
        gSpriteClips[DIRT3_TILE].h = TILE_HEIGHT;

        gSpriteClips[DIRT4_TILE].x = TILE_WIDTH * 3;
        gSpriteClips[DIRT4_TILE].y = TILE_HEIGHT * 3;
        gSpriteClips[DIRT4_TILE].w = TILE_WIDTH;
        gSpriteClips[DIRT4_TILE].h = TILE_HEIGHT;

        gSpriteClips[MARSH1_TILE].x = TILE_WIDTH * 4;
        gSpriteClips[MARSH1_TILE].y = TILE_HEIGHT * 2;
        gSpriteClips[MARSH1_TILE].w = TILE_WIDTH;
        gSpriteClips[MARSH1_TILE].h = TILE_HEIGHT;

        gSpriteClips[MARSH2_TILE].x = TILE_WIDTH * 5;
        gSpriteClips[MARSH2_TILE].y = TILE_HEIGHT * 2;
        gSpriteClips[MARSH2_TILE].w = TILE_WIDTH;
        gSpriteClips[MARSH2_TILE].h = TILE_HEIGHT;

        gSpriteClips[MARSH3_TILE].x = TILE_WIDTH * 6;
        gSpriteClips[MARSH3_TILE].y = TILE_HEIGHT * 2;
        gSpriteClips[MARSH3_TILE].w = TILE_WIDTH;
        gSpriteClips[MARSH3_TILE].h = TILE_HEIGHT;

        gSpriteClips[MARSH4_TILE].x = TILE_WIDTH * 7;
        gSpriteClips[MARSH4_TILE].y = TILE_HEIGHT * 2;
        gSpriteClips[MARSH4_TILE].w = TILE_WIDTH;
        gSpriteClips[MARSH4_TILE].h = TILE_HEIGHT;

        gSpriteClips[HILLS1_TILE].x = TILE_WIDTH * 0;
        gSpriteClips[HILLS1_TILE].y = TILE_HEIGHT * 4;
        gSpriteClips[HILLS1_TILE].w = TILE_WIDTH;
        gSpriteClips[HILLS1_TILE].h = TILE_HEIGHT;

        gSpriteClips[HILLS2_TILE].x = TILE_WIDTH * 1;
        gSpriteClips[HILLS2_TILE].y = TILE_HEIGHT * 4;
        gSpriteClips[HILLS2_TILE].w = TILE_WIDTH;
        gSpriteClips[HILLS2_TILE].h = TILE_HEIGHT;

        gSpriteClips[HILLS3_TILE].x = TILE_WIDTH * 2;
        gSpriteClips[HILLS3_TILE].y = TILE_HEIGHT * 4;
        gSpriteClips[HILLS3_TILE].w = TILE_WIDTH;
        gSpriteClips[HILLS3_TILE].h = TILE_HEIGHT;

        gSpriteClips[HILLS4_TILE].x = TILE_WIDTH * 3;
        gSpriteClips[HILLS4_TILE].y = TILE_HEIGHT * 4;
        gSpriteClips[HILLS4_TILE].w = TILE_WIDTH;
        gSpriteClips[HILLS4_TILE].h = TILE_HEIGHT;
    }

    return success;
}

void close() {
    gFPSTextTexture.free();

    TTF_CloseFont(gFont);
    gFont = nullptr;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(__unused int argc, __unused char *args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            bool quit = false;
            Tile* tiles = (Tile*)malloc(sizeof(Tile) * NUM_TILES);

            for (int row = 0; row < NUM_ROWS; row++) {
                for (int col = 0; col < NUM_COLS; col++) {
                    int index = row * NUM_COLS + col;
                    tiles[index] = Tile(gRenderer, &gSpriteSheetTexture, col, row, gSpriteClips[FOREST1_TILE]);
                }
            }

            SDL_Event e;
            SDL_Color textColor = {0, 0, 0, 255};

            Timer fpsTimer;
            Timer capTimer;
            std::stringstream timeText;
            int countedFrames = 0;
            fpsTimer.start();

            while (!quit) {
                capTimer.start();

                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                // Calculate and correct fps
                float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
                if (avgFPS > 2000000) {
                    avgFPS = 0;
                }

                timeText.str("");
                timeText << "Average Frames Per Second (With Cap) " << avgFPS;

                if (!gFPSTextTexture.loadFromRenderedText(gRenderer, gFont, timeText.str().c_str(), textColor)) {
                    printf("Unable to render FPS texture!\n");
                }

                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                for (int i = 0; i < NUM_TILES; i++) {
                    tiles[i].render();
                }

//                for (auto &tile : tiles) {
//                    tile.render();
//                }

                gFPSTextTexture.render(gRenderer,
                                       (SCREEN_WIDTH - gFPSTextTexture.getWidth()) / 2,
                                       (SCREEN_HEIGHT - gFPSTextTexture.getHeight()) / 2);
                SDL_RenderPresent(gRenderer);
                ++countedFrames;

                // If frame finished early
                int frameTicks = capTimer.getTicks();
                if (frameTicks < SCREEN_TICK_PER_FRAME) {
                    //Wait remaining time
                    SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
                }
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}