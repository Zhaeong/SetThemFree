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

void InitTriangleArray(Triangle *triangleArray, int radius);

void RenderTriangleArray(SDL_Renderer *renderer, Triangle *triangleArray, SDL_Point center);

void UpdateTriangleArrayRadius(Triangle *triangleArray, int value);

void RotateTriangleArray(Triangle *triangleArray, int rotation);

bool CheckPointInCircle(SDL_Point circleCenter, int radius, SDL_Point checkPoint);

bool TextureMouseCollisionSingle(Texture mTexture, int xPos, int yPos);

bool CheckGuidancePolygonCollision(Triangle *triangleArray, int guidanceX, int guidanceY, int guidanceW, int *maxGuidance);

void InitChallengeTexture(SDL_Texture *challengeTex, Texture *textureArray, int numTextures, bool isLeft);

void IncrementChallengeTextures(Texture *textureArray, int numTextures, bool isLeft);

string CheckChallengePolygonCollision(Texture *challengeArray, Triangle *triangleArray);

void RenderTextureArray(SDL_Renderer *renderer, Texture *textureArray, int numTextures);

#endif // GAME_H
 
