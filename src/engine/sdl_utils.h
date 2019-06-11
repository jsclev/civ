#ifndef CIV_SDL_UTILS_H
#define CIV_SDL_UTILS_H

#include <SDL.h>
#include "Tile.h"

bool init(SDL_Renderer *&renderer, SDL_Window *&window, int screenWidth, int screenHeight);
bool checkCollision(SDL_Rect a, SDL_Rect b);
bool touchesWall(SDL_Rect box, Tile *tiles[], int numTiles);
void toggleFullscreen(SDL_Window *window);
SDL_Texture *loadTexture(const std::string path, SDL_Renderer *renderer);

#endif
