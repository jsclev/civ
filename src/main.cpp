#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <iostream>
#include <string>

const int SCREEN_WIDTH = 1536;
const int SCREEN_HEIGHT = 1024;
const int TILE_WIDTH = 256;
const int TILE_HEIGHT = 384;
const int TILE_SIZE = 256;

const int GRASS1_TILE = 0;
const int GRASS2_TILE = 1;
const int GRASS3_TILE = 2;
const int GRASS4_TILE = 3;
const int WATER1_TILE = 4;
const int WATER2_TILE = 5;
const int WATER3_TILE = 6;
const int WATER4_TILE = 7;
const int MOUNTAIN1_TILE = 8;
const int MOUNTAIN2_TILE = 9;
const int MOUNTAIN3_TILE = 10;
const int MOUNTAIN4_TILE = 11;
const int DESERT1_TILE = 12;
const int DESERT2_TILE = 13;
const int DESERT3_TILE = 14;
const int DESERT4_TILE = 15;
const int FOREST1_TILE = 16;
const int FOREST2_TILE = 17;
const int FOREST3_TILE = 18;
const int FOREST4_TILE = 19;
const int MARSH1_TILE = 20;
const int MARSH2_TILE = 21;
const int MARSH3_TILE = 22;
const int MARSH4_TILE = 23;
const int DIRT1_TILE = 24;
const int DIRT2_TILE = 25;
const int DIRT3_TILE = 26;
const int DIRT4_TILE = 27;
const int HILLS1_TILE = 28;
const int HILLS2_TILE = 29;
const int HILLS3_TILE = 30;
const int HILLS4_TILE = 31;


class LTexture {
public:
    LTexture();

    ~LTexture();

    bool loadFromFile(std::string path);

    //Deallocates texture
    void free();

    //Renders texture at given point
    void render(int x, int y, SDL_Rect *clip = nullptr);

    //Gets image dimensions
    int getWidth();

    int getHeight();

private:
    //The actual hardware texture
    SDL_Texture *mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window *gWindow = nullptr;

//The window renderer
SDL_Renderer *gRenderer = nullptr;

//Scene sprites
SDL_Rect gSpriteClips[128];
LTexture gSpriteSheetTexture;


LTexture::LTexture() {
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    //Deallocate
    free();
}

bool LTexture::loadFromFile(std::string path) {
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture *newTexture = nullptr;

    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        //Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        } else {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        SDL_RendererInfo rendererInfo;
        SDL_GetRendererInfo(gRenderer, &rendererInfo);

        std::cout << "Renderer: " << rendererInfo.name << std::endl;

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free() {
    //Free texture if it exists
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render(int x, int y, SDL_Rect *clip) {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    //Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
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
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
            printf("Warning: Linear texture filtering not enabled!");
        }

//        if (!SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl")) {
//            printf("Warning: Linear texture filtering not enabled!");
//        }

        //Create window
        Uint32 flags = SDL_WINDOW_ALLOW_HIGHDPI;
        gWindow = SDL_CreateWindow("Civ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT, flags);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia() {
    //Loading success flag
    bool success = true;

    //Load sprite sheet texture
    if (!gSpriteSheetTexture.loadFromFile(
            "/Users/john/projects/civ/assets/images/tiles/painted_terrain_tiles_basic_256x384_sheet.png")) {
        printf("Failed to load sprite sheet texture!\n");
        success = false;
    } else {
        // Grass tiles
        gSpriteClips[GRASS1_TILE].x = TILE_WIDTH * 0;
        gSpriteClips[GRASS1_TILE].y = 0;
        gSpriteClips[GRASS1_TILE].w = TILE_WIDTH;
        gSpriteClips[GRASS1_TILE].h = TILE_HEIGHT;

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

        gSpriteClips[DESERT1_TILE].x = TILE_SIZE * 4;
        gSpriteClips[DESERT1_TILE].y = TILE_SIZE * 2;
        gSpriteClips[DESERT1_TILE].w = TILE_WIDTH;
        gSpriteClips[DESERT1_TILE].h = TILE_HEIGHT;

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
    }

    return success;
}

void close() {
    gSpriteSheetTexture.free();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        //Load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //While application is running
            while (!quit) {
                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Column
                gSpriteSheetTexture.render(0 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[FOREST1_TILE]);
                gSpriteSheetTexture.render(0 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(0 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS1_TILE]);
                gSpriteSheetTexture.render(0 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS2_TILE]);
                gSpriteSheetTexture.render(0 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST1_TILE]);
                gSpriteSheetTexture.render(0 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST2_TILE]);
                gSpriteSheetTexture.render(0 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST2_TILE]);
                gSpriteSheetTexture.render(0 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST2_TILE]);
                gSpriteSheetTexture.render(0 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST2_TILE]);

                // Column
//                gSpriteSheetTexture.render(1 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[GRASS1_TILE]);
//                gSpriteSheetTexture.render(1 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(1 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(1 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS3_TILE]);
//                gSpriteSheetTexture.render(1 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST3_TILE]);
//                gSpriteSheetTexture.render(1 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);
//                gSpriteSheetTexture.render(1 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);
//                gSpriteSheetTexture.render(1 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);
//                gSpriteSheetTexture.render(1 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);
//
//                // Column
//                gSpriteSheetTexture.render(2 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(2 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(2 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(2 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
//                gSpriteSheetTexture.render(2 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
//                gSpriteSheetTexture.render(2 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
//                gSpriteSheetTexture.render(2 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
//                gSpriteSheetTexture.render(2 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
//                gSpriteSheetTexture.render(2 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
//
//                // Column
//                gSpriteSheetTexture.render(3 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(3 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(3 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(3 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(3 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(3 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(3 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(3 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(3 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
//
//                // Column
//                gSpriteSheetTexture.render(4 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(4 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(4 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
//                gSpriteSheetTexture.render(4 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(4 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(4 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(4 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(4 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(4 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//
//                // Column
//                gSpriteSheetTexture.render(5 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(5 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(5 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(5 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(5 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(5 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(5 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(5 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(5 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//
//                // Column
//                gSpriteSheetTexture.render(6 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[FOREST1_TILE]);
//                gSpriteSheetTexture.render(6 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(6 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(6 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS2_TILE]);
//                gSpriteSheetTexture.render(6 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST1_TILE]);
//                gSpriteSheetTexture.render(6 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST2_TILE]);
//                gSpriteSheetTexture.render(6 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST2_TILE]);
//                gSpriteSheetTexture.render(6 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST2_TILE]);
//                gSpriteSheetTexture.render(6 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST2_TILE]);
//
//                // Column
//                gSpriteSheetTexture.render(7 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[GRASS1_TILE]);
//                gSpriteSheetTexture.render(7 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(7 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(7 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN3_TILE]);
//                gSpriteSheetTexture.render(7 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST3_TILE]);
//                gSpriteSheetTexture.render(7 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);
//                gSpriteSheetTexture.render(7 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);
//                gSpriteSheetTexture.render(7 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);
//                gSpriteSheetTexture.render(7 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);
//
//                // Column
//                gSpriteSheetTexture.render(8 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(8 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(8 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(8 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
//                gSpriteSheetTexture.render(8 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
//                gSpriteSheetTexture.render(8 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
//                gSpriteSheetTexture.render(8 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
//                gSpriteSheetTexture.render(8 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
//                gSpriteSheetTexture.render(8 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
//
//                // Column
//                gSpriteSheetTexture.render(9 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(9 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(9 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(9 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(9 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(9 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(9 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(9 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(9 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
//
//                // Column
//                gSpriteSheetTexture.render(10 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(10 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(10 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
//                gSpriteSheetTexture.render(10 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(10 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(10 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(10 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(10 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(10 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//
//                // Column
//                gSpriteSheetTexture.render(11 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(11 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(11 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS3_TILE]);
//                gSpriteSheetTexture.render(11 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS2_TILE]);
//                gSpriteSheetTexture.render(11 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(11 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(11 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(11 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(11 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//
//                // Column
//                gSpriteSheetTexture.render(12 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(12 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(12 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS3_TILE]);
//                gSpriteSheetTexture.render(12 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS2_TILE]);
//                gSpriteSheetTexture.render(12 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(12 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(12 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(12 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(12 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//
//                // Column
//                gSpriteSheetTexture.render(13 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
//                gSpriteSheetTexture.render(13 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
//                gSpriteSheetTexture.render(13 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS3_TILE]);
//                gSpriteSheetTexture.render(13 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS2_TILE]);
//                gSpriteSheetTexture.render(13 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(13 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(13 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(13 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
//                gSpriteSheetTexture.render(13 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);

                SDL_RenderPresent(gRenderer);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}