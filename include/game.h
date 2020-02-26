#ifndef GAME_H
#define GAME_H
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <emscripten.h>
#include <iostream>


#include "game_defs.h"


using namespace std;


int StartSDL(SDL_Window **window, SDL_Renderer **renderer);

SDL_Texture* GetSDLTexture(SDL_Renderer *renderer, SDL_Window *window, string textureLocation);

Texture InitTexture(SDL_Texture *sdlTexture, int x, int y);

void RenderTexture(SDL_Renderer *renderer, Texture tex);

void RemoveTextureWhiteSpace(SDL_Texture *texture);

void SetTextureColor(SDL_Texture *texture, int R, int G, int B, int A);

void GetPolygonPoints(SDL_Point *polygonarray, SDL_Point center, int radius, vect2 direction);

void RenderPolygon(SDL_Renderer *renderer, SDL_Point *polygonArray); 

vect2 RotateVector(vect2 direction, int rotation);

bool CheckPointInCircle(SDL_Point circleCenter, int radius, SDL_Point checkPoint);

bool TextureMouseCollisionSingle(Texture mTexture, int xPos, int yPos);


#endif // GAME_H
 
