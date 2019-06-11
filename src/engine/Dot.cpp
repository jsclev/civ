#include "Dot.h"
#include "Tile.h"
#include "sdl_utils.h"

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

void Dot::move(Tile *tiles[], int levelWidth, int levelHeight, int numTiles) {
    //Move the dot left or right
    mBox.x += mVelX;

    //If the dot went too far to the left or right or touched a wall
    if ((mBox.x < 0) || (mBox.x + DOT_WIDTH > levelWidth) || touchesWall(mBox, tiles, numTiles)) {
        //move back
        mBox.x -= mVelX;
    }

    //Move the dot up or down
    mBox.y += mVelY;

    //If the dot went too far up or down or touched a wall
    if ((mBox.y < 0) || (mBox.y + DOT_HEIGHT > levelHeight) || touchesWall(mBox, tiles, numTiles)) {
        //move back
        mBox.y -= mVelY;
    }
}

void Dot::setCamera(SDL_Rect &camera, int screenWidth, int screenHeight, int levelWidth, int levelHeight) {
    //Center the camera over the dot
    camera.x = (mBox.x + DOT_WIDTH / 2) - screenWidth / 2;
    camera.y = (mBox.y + DOT_HEIGHT / 2) - screenHeight / 2;

    //Keep the camera in bounds
    if (camera.x < 0) {
        camera.x = 0;
    }
    if (camera.y < 0) {
        camera.y = 0;
    }
    if (camera.x > levelWidth - camera.w) {
        camera.x = levelWidth - camera.w;
    }
    if (camera.y > levelHeight - camera.h) {
        camera.y = levelHeight - camera.h;
    }
}

void Dot::render(SDL_Rect &camera, LTexture dotTexture, SDL_Renderer* renderer) {
    dotTexture.render(mBox.x - camera.x, mBox.y - camera.y, renderer);
}
