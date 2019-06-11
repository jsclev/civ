#ifndef CIV_LTEXTURE_H
#define CIV_LTEXTURE_H

#include <string>
#include <SDL.h>

class LTexture {
public:
    LTexture();
    ~LTexture();
    bool loadFromFile(std::string path, SDL_Renderer *gRenderer);

#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
    //Creates image from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
#endif

    //Deallocates texture
    void free();

    //Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //Set blending
    void setBlendMode(SDL_BlendMode blending);

    //Set alpha modulation
    void setAlpha(Uint8 alpha);

    //Renders texture at given point
    void render(int x,
            int y,
            SDL_Renderer* renderer,
            SDL_Rect *clip = NULL,
            double angle = 0.0,
            SDL_Point *center = NULL,
            SDL_RendererFlip flip = SDL_FLIP_NONE,
            SDL_Window *gWindow = NULL);

    int getWidth();
    int getHeight();

private:
    //The actual hardware texture
    SDL_Texture *mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};

#endif //CIV_LTEXTURE_H
