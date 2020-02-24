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

void RemoveTextureWhiteSpace(SDL_Window *window, SDL_Texture *texture);

void RenderPolygon(SDL_Renderer *renderer, SDL_Point center, SDL_Point start);
#endif // GAME_H
 
