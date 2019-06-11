#include "engine.h"

#include <stdio.h>
#include <string>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>

const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;

const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

bool setTiles(Tile *tiles[], SDL_Rect clips[]) {
    //Success flag
    bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map("/Users/jcleveland/projects/civ/assets/images/lazy.map");

    //If the map couldn't be loaded
    if (1 == 2) {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    } else {
        //Initialize the tiles
        for (int i = 0; i < 192; ++i) {
            //Determines what kind of tile will be made
            int tileType = -1;

            //Read tile from map file
            map >> tileType;

            //If the was a problem in reading the map
            if (map.fail()) {
                //Stop loading map
                printf("Error loading map: Unexpected end of file!\n");
                tilesLoaded = false;
                break;
            }

            //If the number is a valid tile number
            if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES)) {
                tiles[i] = new Tile(x, y, tileType);
            }
                //If we don't recognize the tile type
            else {
                //Stop loading map
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }

            //Move to next tile spot
            x += TILE_WIDTH;

            //If we've gone too far
            if (x >= LEVEL_WIDTH) {
                //Move back
                x = 0;

                //Move to the next row
                y += TILE_HEIGHT;
            }
        }

        //Clip the sprite sheet
        if (tilesLoaded) {
            clips[TILE_RED].x = 0;
            clips[TILE_RED].y = 0;
            clips[TILE_RED].w = TILE_WIDTH;
            clips[TILE_RED].h = TILE_HEIGHT;

            clips[TILE_GREEN].x = 0;
            clips[TILE_GREEN].y = 80;
            clips[TILE_GREEN].w = TILE_WIDTH;
            clips[TILE_GREEN].h = TILE_HEIGHT;

            clips[TILE_BLUE].x = 0;
            clips[TILE_BLUE].y = 160;
            clips[TILE_BLUE].w = TILE_WIDTH;
            clips[TILE_BLUE].h = TILE_HEIGHT;

            clips[TILE_TOPLEFT].x = 80;
            clips[TILE_TOPLEFT].y = 0;
            clips[TILE_TOPLEFT].w = TILE_WIDTH;
            clips[TILE_TOPLEFT].h = TILE_HEIGHT;

            clips[TILE_LEFT].x = 80;
            clips[TILE_LEFT].y = 80;
            clips[TILE_LEFT].w = TILE_WIDTH;
            clips[TILE_LEFT].h = TILE_HEIGHT;

            clips[TILE_BOTTOMLEFT].x = 80;
            clips[TILE_BOTTOMLEFT].y = 160;
            clips[TILE_BOTTOMLEFT].w = TILE_WIDTH;
            clips[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

            clips[TILE_TOP].x = 160;
            clips[TILE_TOP].y = 0;
            clips[TILE_TOP].w = TILE_WIDTH;
            clips[TILE_TOP].h = TILE_HEIGHT;

            clips[TILE_CENTER].x = 160;
            clips[TILE_CENTER].y = 80;
            clips[TILE_CENTER].w = TILE_WIDTH;
            clips[TILE_CENTER].h = TILE_HEIGHT;

            clips[TILE_BOTTOM].x = 160;
            clips[TILE_BOTTOM].y = 160;
            clips[TILE_BOTTOM].w = TILE_WIDTH;
            clips[TILE_BOTTOM].h = TILE_HEIGHT;

            clips[TILE_TOPRIGHT].x = 240;
            clips[TILE_TOPRIGHT].y = 0;
            clips[TILE_TOPRIGHT].w = TILE_WIDTH;
            clips[TILE_TOPRIGHT].h = TILE_HEIGHT;

            clips[TILE_RIGHT].x = 240;
            clips[TILE_RIGHT].y = 80;
            clips[TILE_RIGHT].w = TILE_WIDTH;
            clips[TILE_RIGHT].h = TILE_HEIGHT;

            clips[TILE_BOTTOMRIGHT].x = 240;
            clips[TILE_BOTTOMRIGHT].y = 160;
            clips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
            clips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;
        }
    }

    map.close();

    return tilesLoaded;
}

bool loadMedia(SDL_Renderer *renderer, LTexture dotTexture, LTexture tileTexture, Tile *tiles[],
        SDL_Rect clips[]) {
    bool success = true;

    if (!dotTexture.loadFromFile("/Users/jcleveland/projects/civ/assets/images/dot.bmp", renderer)) {
        printf("Failed to load dot texture!\n");
        success = false;
    }

    if (!tileTexture.loadFromFile("/Users/jcleveland/projects/civ/assets/images/tiles.png", renderer)) {
        printf("Failed to load tile set texture!\n");
        success = false;
    }

    //Load tile map
//    if (!setTiles(tiles, clips)) {
//        printf("Failed to load tile set!\n");
//        success = false;
//    }

    return success;
}

void close(
        Tile *tiles[],
        SDL_Window *window,
        SDL_Renderer *renderer,
        LTexture dotTexture,
        LTexture tileTexture) {
    //Deallocate tiles
    for (int i = 0; i < 192; ++i) {
        if (tiles[i] == nullptr) {
            delete tiles[i];
            tiles[i] = nullptr;
        }
    }

    //Free loaded images
    dotTexture.free();
    tileTexture.free();

    //Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = nullptr;
    renderer = nullptr;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}