#ifndef CIV_DOT_H
#define CIV_DOT_H

#include <SDL.h>
#include "Tile.h"

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
    void move(Tile *tiles[], int levelWidth, int levelHeight, int numTiles);

    //Centers the camera over the dot
    void setCamera(SDL_Rect &camera, int screenWidth, int screenHeight, int levelWidth, int levelHeight);

    //Shows the dot on the screen
    void render(SDL_Rect &camera, LTexture dotTexture, SDL_Renderer* renderer);
private:
    //Collision box of the dot
    SDL_Rect mBox;

    //The velocity of the dot
    int mVelX, mVelY;
};

#endif
