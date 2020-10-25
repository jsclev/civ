/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <string>
#include <sstream>

//Screen dimension constants
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

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


//Texture wrapper class
class LTexture
{
public:
    //Initializes variables
    LTexture();

    //Deallocates memory
    ~LTexture();

    //Loads image at specified path
    bool loadFromFile( std::string path );

#if defined(SDL_TTF_MAJOR_VERSION)
    //Creates image from font string
    bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
#endif

    //Deallocates texture
    void free();

    //Set color modulation
    void setColor( Uint8 red, Uint8 green, Uint8 blue );

    //Set blending
    void setBlendMode( SDL_BlendMode blending );

    //Set alpha modulation
    void setAlpha( Uint8 alpha );

    //Renders texture at given point
    void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

    //Gets image dimensions
    int getWidth();
    int getHeight();

private:
    //The actual hardware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};

//The application time based timer
class LTimer
{
public:
    //Initializes variables
    LTimer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    Uint32 getTicks();

    //Checks the status of the timer
    bool isStarted();
    bool isPaused();

private:
    //The clock time when the timer started
    Uint32 mStartTicks;

    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    //The timer status
    bool mPaused;
    bool mStarted;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font* gFont = NULL;

SDL_Rect gSpriteClips[128];
LTexture gSpriteSheetTexture;

//Scene textures
LTexture gFPSTextTexture;

LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::loadFromFile( std::string path )
{
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface != NULL )
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    else
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }


    //Return success
    return mTexture != NULL;
}
#endif

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture rgb
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
    //Set blending function
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
    //Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
    //Timer is running and paused
    return mPaused && mStarted;
}

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        Uint32 flags = SDL_WINDOW_ALLOW_HIGHDPI;
        gWindow = SDL_CreateWindow("Civ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT, flags);
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }

                //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Open the font
    gFont = TTF_OpenFont( "assets/fonts/lazy.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    //Load sprite sheet texture
    if (!gSpriteSheetTexture.loadFromFile(
            "assets/images/tiles/painted_terrain_tiles_basic_256x384_sheet.png")) {
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

void close()
{
    //Free loaded images
    gFPSTextTexture.free();

    //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //Set text color as black
            SDL_Color textColor = { 0, 0, 0, 255 };

            //The frames per second timer
            LTimer fpsTimer;

            //The frames per second cap timer
            LTimer capTimer;

            //In memory text stream
            std::stringstream timeText;

            //Start counting frames per second
            int countedFrames = 0;
            fpsTimer.start();

            //While application is running
            while( !quit )
            {
                //Start cap timer
                capTimer.start();

                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                }

                //Calculate and correct fps
                float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
                if( avgFPS > 2000000 )
                {
                    avgFPS = 0;
                }

                //Set text to be rendered
                timeText.str( "" );
                timeText << "Average Frames Per Second (With Cap) " << avgFPS;

                //Render text
                if( !gFPSTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
                {
                    printf( "Unable to render FPS texture!\n" );
                }

                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

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
                gSpriteSheetTexture.render(1 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[GRASS1_TILE]);
                gSpriteSheetTexture.render(1 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(1 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(1 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS3_TILE]);
                gSpriteSheetTexture.render(1 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST3_TILE]);
                gSpriteSheetTexture.render(1 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);
                gSpriteSheetTexture.render(1 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);
                gSpriteSheetTexture.render(1 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);
                gSpriteSheetTexture.render(1 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);

                // Column
                gSpriteSheetTexture.render(2 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(2 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(2 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(2 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
                gSpriteSheetTexture.render(2 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
                gSpriteSheetTexture.render(2 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
                gSpriteSheetTexture.render(2 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
                gSpriteSheetTexture.render(2 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
                gSpriteSheetTexture.render(2 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);

                // Column
                gSpriteSheetTexture.render(3 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(3 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(3 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(3 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(3 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(3 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(3 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(3 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(3 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);

                // Column
                gSpriteSheetTexture.render(4 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(4 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(4 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
                gSpriteSheetTexture.render(4 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(4 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(4 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(4 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(4 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(4 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);

                // Column
                gSpriteSheetTexture.render(5 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(5 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(5 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(5 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(5 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(5 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(5 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(5 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(5 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);

                // Column
                gSpriteSheetTexture.render(6 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[FOREST1_TILE]);
                gSpriteSheetTexture.render(6 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(6 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(6 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS2_TILE]);
                gSpriteSheetTexture.render(6 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST1_TILE]);
                gSpriteSheetTexture.render(6 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST2_TILE]);
                gSpriteSheetTexture.render(6 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST2_TILE]);
                gSpriteSheetTexture.render(6 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST2_TILE]);
                gSpriteSheetTexture.render(6 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST2_TILE]);

                // Column
                gSpriteSheetTexture.render(7 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[GRASS1_TILE]);
                gSpriteSheetTexture.render(7 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(7 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(7 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN3_TILE]);
                gSpriteSheetTexture.render(7 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST3_TILE]);
                gSpriteSheetTexture.render(7 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);
                gSpriteSheetTexture.render(7 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);
                gSpriteSheetTexture.render(7 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);
                gSpriteSheetTexture.render(7 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[FOREST4_TILE]);

                // Column
                gSpriteSheetTexture.render(8 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(8 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(8 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(8 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
                gSpriteSheetTexture.render(8 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
                gSpriteSheetTexture.render(8 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
                gSpriteSheetTexture.render(8 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
                gSpriteSheetTexture.render(8 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
                gSpriteSheetTexture.render(8 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);

                // Column
                gSpriteSheetTexture.render(9 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(9 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(9 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(9 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(9 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(9 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(9 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(9 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(9 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN1_TILE]);

                // Column
                gSpriteSheetTexture.render(10 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(10 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(10 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[MOUNTAIN2_TILE]);
                gSpriteSheetTexture.render(10 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(10 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(10 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(10 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(10 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(10 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);

                // Column
                gSpriteSheetTexture.render(11 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(11 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(11 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS3_TILE]);
                gSpriteSheetTexture.render(11 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS2_TILE]);
                gSpriteSheetTexture.render(11 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(11 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(11 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(11 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(11 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);

                // Column
                gSpriteSheetTexture.render(12 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(12 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(12 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS3_TILE]);
                gSpriteSheetTexture.render(12 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS2_TILE]);
                gSpriteSheetTexture.render(12 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(12 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(12 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(12 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(12 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);

                // Column
                gSpriteSheetTexture.render(13 * TILE_WIDTH, -(TILE_SIZE / 2), &gSpriteClips[WATER1_TILE]);
                gSpriteSheetTexture.render(13 * TILE_WIDTH, TILE_SIZE - 128, &gSpriteClips[MOUNTAIN1_TILE]);
                gSpriteSheetTexture.render(13 * TILE_WIDTH, 2 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS3_TILE]);
                gSpriteSheetTexture.render(13 * TILE_WIDTH, 3 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS2_TILE]);
                gSpriteSheetTexture.render(13 * TILE_WIDTH, 4 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(13 * TILE_WIDTH, 5 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(13 * TILE_WIDTH, 6 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(13 * TILE_WIDTH, 7 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);
                gSpriteSheetTexture.render(13 * TILE_WIDTH, 8 * TILE_SIZE - TILE_SIZE / 2, &gSpriteClips[GRASS4_TILE]);

                gFPSTextTexture.render( ( SCREEN_WIDTH - gFPSTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gFPSTextTexture.getHeight() ) / 2 );

                SDL_RenderPresent(gRenderer);
                ++countedFrames;

                //If frame finished early
                int frameTicks = capTimer.getTicks();
                if( frameTicks < SCREEN_TICK_PER_FRAME )
                {
                    //Wait remaining time
                    SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
                }
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}