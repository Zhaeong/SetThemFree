#include "main.h"
//Declarations for sdl window displaying
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

const int FPS = 60;
//How many miliseconds per frame
const int frameDelay = 1000 / FPS;

string GameState = "LOAD";


//Initial Loading

SDL_Rect srcRect;
SDL_Rect dstRect;
SDL_Texture *debrisTex; 


void gameloop() 
{
    Uint32 frameStart;
    float frameTime;
    frameStart = SDL_GetTicks();

    //The color at which the screen will be if alpha = 0 on all textures
    SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);

    SDL_RenderClear(renderer);

    ////////////////////////////////////////////////////////////////////////
    //Main Game Code
    ////////////////////////////////////////////////////////////////////////

    if(GameState == "LOAD")
    {
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.h = 50;
        srcRect.w = 50;

        dstRect.x = 0;
        dstRect.y = 0;
        dstRect.h = 50;
        dstRect.w = 50;
        debrisTex = GetSDLTexture(renderer, window, "./png/debris1.png");

        GameState = "REND";

    }
    else
    {
        dstRect.x += 1;
        SDL_SetTextureBlendMode(debrisTex, SDL_BLENDMODE_BLEND);

        SDL_RenderCopyEx(renderer, debrisTex, &srcRect, &dstRect, 0, NULL,SDL_FLIP_NONE);

    }

    ////////////////////////////////////////////////////////////////////////
    //End of main game code
    ////////////////////////////////////////////////////////////////////////

    frameTime = SDL_GetTicks() - frameStart;

    if (frameDelay > frameTime)
    {
        SDL_Delay(frameDelay - frameTime);
    }
    cout << "lopidy" << frameTime << "\n";
    //emscripten_cancel_main_loop();
}

//Need to use this main signature (int argv, char** args)
//Because SDL alters default program entry point resolving
//SDL calls your entry point expecting a specific signature,
//which is no longer subject to compiler interpretation
int main(int argv, char **args)
{
    //printf("heyeyeey");
    cout << "I maketh\n";
    //Initiate SDL
    StartSDL(&window, &renderer);
    emscripten_set_main_loop(gameloop, 0, 0);
    printf("End!\n");
    return 0;
}

