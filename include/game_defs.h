#ifndef GAMEDEFS_H
#define GAMEDEFS_H

#include <string>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>


using namespace std;



#define GAMEWIDTH  500 
#define GAMEHEIGHT 500


#define PI 3.14159265


const Uint32 TEXTUREFORMAT = SDL_PIXELFORMAT_RGBA8888;
struct Texture
{
    int mX;
    int mY;
    int mW;
    int mH;
    int mRotation;
    int mAlpha;
    SDL_Texture *mTexture;
    SDL_Point *mCenter;
    SDL_RendererFlip mFlip;
};

struct vect2
{
    float x;
    float y;
};

struct Triangle
{
    vect2 direction;
    int radius;
    int numGuidance;
    SDL_Point startPoint;
    SDL_Point endPoint;
};

/*
static float vect2dot(vect2 v1, vect2 v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}
*/

#endif // GAMEDEFS_H

