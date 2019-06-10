#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

#include "engine/sdl_utils.h"
#include "engine/engine.h"
#include "engine/Tile.h"
#include "engine/LTexture.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//Tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

//The different tile sprites
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

void ToggleFullscreen(SDL_Window *Window) {
    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
    bool IsFullscreen = SDL_GetWindowFlags(Window) & FullscreenFlag;
    SDL_SetWindowFullscreen(Window, IsFullscreen ? 0 : FullscreenFlag);
    SDL_ShowCursor(IsFullscreen);
}



//The dot that will move around on the screen
class Dot {
public:
    //The dimensions of the dot
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;

    //Maximum axis velocity of the dot
    static const int DOT_VEL = 10;

    //Initializes the variables
    Dot();

    //Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event &e);

    //Moves the dot and check collision against tiles
    void move(Tile *tiles[]);

    //Centers the camera over the dot
    void setCamera(SDL_Rect &camera);

    //Shows the dot on the screen
    void render(SDL_Rect &camera);

private:
    //Collision box of the dot
    SDL_Rect mBox;

    //The velocity of the dot
    int mVelX, mVelY;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia(Tile *tiles[]);



//Checks collision box against set of tiles
bool touchesWall(SDL_Rect box, Tile *tiles[]);

//Sets tiles from tile map
bool setTiles(Tile *tiles[]);

//The window we'll be rendering to
SDL_Window *gWindow = NULL;

//The window renderer
SDL_Renderer *gRenderer = NULL;

//Scene textures
LTexture gDotTexture;
LTexture gTileTexture;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];





Dot::Dot() {
    //Initialize the collision box
    mBox.x = 0;
    mBox.y = 0;
    mBox.w = DOT_WIDTH;
    mBox.h = DOT_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void Dot::handleEvent(SDL_Event &e) {
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        //Adjust the velocity
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                mVelY -= DOT_VEL;
                break;
            case SDLK_DOWN:
                mVelY += DOT_VEL;
                break;
            case SDLK_LEFT:
                mVelX -= DOT_VEL;
                break;
            case SDLK_RIGHT:
                mVelX += DOT_VEL;
                break;
        }
    }
        //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        //Adjust the velocity
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                mVelY += DOT_VEL;
                break;
            case SDLK_DOWN:
                mVelY -= DOT_VEL;
                break;
            case SDLK_LEFT:
                mVelX += DOT_VEL;
                break;
            case SDLK_RIGHT:
                mVelX -= DOT_VEL;
                break;
        }
    }
}

void Dot::move(Tile *tiles[]) {
    //Move the dot left or right
    mBox.x += mVelX;

    //If the dot went too far to the left or right or touched a wall
    if ((mBox.x < 0) || (mBox.x + DOT_WIDTH > LEVEL_WIDTH) || touchesWall(mBox, tiles)) {
        //move back
        mBox.x -= mVelX;
    }

    //Move the dot up or down
    mBox.y += mVelY;

    //If the dot went too far up or down or touched a wall
    if ((mBox.y < 0) || (mBox.y + DOT_HEIGHT > LEVEL_HEIGHT) || touchesWall(mBox, tiles)) {
        //move back
        mBox.y -= mVelY;
    }
}

void Dot::setCamera(SDL_Rect &camera) {
    //Center the camera over the dot
    camera.x = (mBox.x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
    camera.y = (mBox.y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

    //Keep the camera in bounds
    if (camera.x < 0) {
        camera.x = 0;
    }
    if (camera.y < 0) {
        camera.y = 0;
    }
    if (camera.x > LEVEL_WIDTH - camera.w) {
        camera.x = LEVEL_WIDTH - camera.w;
    }
    if (camera.y > LEVEL_HEIGHT - camera.h) {
        camera.y = LEVEL_HEIGHT - camera.h;
    }
}

void Dot::render(SDL_Rect &camera) {
    //Show the dot
    gDotTexture.render(mBox.x - camera.x, mBox.y - camera.y, gRenderer);
}

bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    static int display_in_use = 0; /* Only using first display */

    int i, display_mode_count;
    SDL_DisplayMode mode;
    Uint32 f;

    SDL_Log("SDL_GetNumVideoDisplays(): %i", SDL_GetNumVideoDisplays());

    display_mode_count = SDL_GetNumDisplayModes(display_in_use);
    if (display_mode_count < 1) {
        SDL_Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
        return 1;
    }
    SDL_Log("SDL_GetNumDisplayModes: %i", display_mode_count);

    for (i = 0; i < display_mode_count; ++i) {
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0) {
            SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
            return 1;
        }
        f = mode.format;

        SDL_Log("Mode %i\tbpp %i\t%s\t%i x %i", i,
                SDL_BITSPERPIXEL(f), SDL_GetPixelFormatName(f), mode.w, mode.h);
    }

    return success;
}

bool loadMedia(Tile *tiles[]) {
    //Loading success flag
    bool success = true;

    //Load dot texture
    if (!gDotTexture.loadFromFile("/Users/jcleveland/projects/civ/assets/images/dot.bmp", gRenderer)) {
        printf("Failed to load dot texture!\n");
        success = false;
    }

    //Load tile texture
    if (!gTileTexture.loadFromFile("/Users/jcleveland/projects/civ/assets/images/tiles.png", gRenderer)) {
        printf("Failed to load tile set texture!\n");
        success = false;
    }

    //Load tile map
    if (!setTiles(tiles)) {
        printf("Failed to load tile set!\n");
        success = false;
    }

    return success;
}





bool setTiles(Tile *tiles[]) {
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
        for (int i = 0; i < TOTAL_TILES; ++i) {
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
            gTileClips[TILE_RED].x = 0;
            gTileClips[TILE_RED].y = 0;
            gTileClips[TILE_RED].w = TILE_WIDTH;
            gTileClips[TILE_RED].h = TILE_HEIGHT;

            gTileClips[TILE_GREEN].x = 0;
            gTileClips[TILE_GREEN].y = 80;
            gTileClips[TILE_GREEN].w = TILE_WIDTH;
            gTileClips[TILE_GREEN].h = TILE_HEIGHT;

            gTileClips[TILE_BLUE].x = 0;
            gTileClips[TILE_BLUE].y = 160;
            gTileClips[TILE_BLUE].w = TILE_WIDTH;
            gTileClips[TILE_BLUE].h = TILE_HEIGHT;

            gTileClips[TILE_TOPLEFT].x = 80;
            gTileClips[TILE_TOPLEFT].y = 0;
            gTileClips[TILE_TOPLEFT].w = TILE_WIDTH;
            gTileClips[TILE_TOPLEFT].h = TILE_HEIGHT;

            gTileClips[TILE_LEFT].x = 80;
            gTileClips[TILE_LEFT].y = 80;
            gTileClips[TILE_LEFT].w = TILE_WIDTH;
            gTileClips[TILE_LEFT].h = TILE_HEIGHT;

            gTileClips[TILE_BOTTOMLEFT].x = 80;
            gTileClips[TILE_BOTTOMLEFT].y = 160;
            gTileClips[TILE_BOTTOMLEFT].w = TILE_WIDTH;
            gTileClips[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

            gTileClips[TILE_TOP].x = 160;
            gTileClips[TILE_TOP].y = 0;
            gTileClips[TILE_TOP].w = TILE_WIDTH;
            gTileClips[TILE_TOP].h = TILE_HEIGHT;

            gTileClips[TILE_CENTER].x = 160;
            gTileClips[TILE_CENTER].y = 80;
            gTileClips[TILE_CENTER].w = TILE_WIDTH;
            gTileClips[TILE_CENTER].h = TILE_HEIGHT;

            gTileClips[TILE_BOTTOM].x = 160;
            gTileClips[TILE_BOTTOM].y = 160;
            gTileClips[TILE_BOTTOM].w = TILE_WIDTH;
            gTileClips[TILE_BOTTOM].h = TILE_HEIGHT;

            gTileClips[TILE_TOPRIGHT].x = 240;
            gTileClips[TILE_TOPRIGHT].y = 0;
            gTileClips[TILE_TOPRIGHT].w = TILE_WIDTH;
            gTileClips[TILE_TOPRIGHT].h = TILE_HEIGHT;

            gTileClips[TILE_RIGHT].x = 240;
            gTileClips[TILE_RIGHT].y = 80;
            gTileClips[TILE_RIGHT].w = TILE_WIDTH;
            gTileClips[TILE_RIGHT].h = TILE_HEIGHT;

            gTileClips[TILE_BOTTOMRIGHT].x = 240;
            gTileClips[TILE_BOTTOMRIGHT].y = 160;
            gTileClips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
            gTileClips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;
        }
    }

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}

bool touchesWall(SDL_Rect box, Tile *tiles[]) {
    //Go through the tiles
    for (int i = 0; i < TOTAL_TILES; ++i) {
        //If the tile is a wall type tile
        if ((tiles[i]->getType() >= TILE_CENTER) && (tiles[i]->getType() <= TILE_TOPLEFT)) {
            //If the collision box touches the wall tile
            if (checkCollision(box, tiles[i]->getBox())) {
                return true;
            }
        }
    }

    //If no wall tiles were touched
    return false;
}

int main(int argc, char *args[]) {
    //Start up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        //The level tiles
        Tile *tileSet[TOTAL_TILES];

        //Load media
        if (!loadMedia(tileSet)) {
            printf("Failed to load media!\n");
        } else {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //The dot that will be moving around on the screen
            Dot dot;

            //Level camera
            SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

            //While application is running
            while (!quit) {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                            case SDLK_f:
                                ToggleFullscreen(gWindow);
                                break;
                        }
                    }

                    //Handle input for the dot
                    dot.handleEvent(e);
                }

                //Move the dot
                dot.move(tileSet);
                dot.setCamera(camera);

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                //Render level
                for (int i = 0; i < TOTAL_TILES; ++i) {
                    tileSet[i]->render(camera, gTileTexture, gRenderer, gTileClips);
                }

                //Render dot
                dot.render(camera);

                //Update screen
                SDL_RenderPresent(gRenderer);
            }
        }

        //Free resources and close SDL
        close(tileSet, gWindow, gRenderer, gDotTexture, gTileTexture);
    }

    return 0;
}