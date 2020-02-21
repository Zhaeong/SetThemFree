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

//Forward declaration due to obj_texture including this file
//class Texture;

//Foward declaration due to obj_ship including this file
//class Ship;

int StartSDL(SDL_Window **window, SDL_Renderer **renderer);

SDL_Texture* GetSDLTexture(SDL_Renderer *renderer, SDL_Window *window, string textureLocation);


#endif // GAME_H
 
