#include "main.h"


//Declarations for sdl window displaying
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

const int FPS = 60;
//How many miliseconds per frame
const int FrameDelay = 1000 / FPS;

string GameState = "LOAD";

//Initial Loading
SDL_Rect srcRect;
SDL_Rect dstRect;

Texture Title;

int screenR, screenG, screenB;

SDL_Point center;


SDL_Point startVal;

vect2 direction;
int radius;
void gameloop() 
{
    Uint32 frameStart;
    float frameTime;
    frameStart = SDL_GetTicks();  

    //The color at which the screen will be if alpha = 0 on all textures
    SDL_SetRenderDrawColor(renderer, screenR, screenG, screenB, 255);

    SDL_RenderClear(renderer);

    ////////////////////////////////////////////////////////////////////////
    //Main Game Code
    ////////////////////////////////////////////////////////////////////////
    int xMouse = 0;
    int yMouse = 0;
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        switch (event.type) 
        {
            case SDL_KEYDOWN:
                break;
            case SDL_KEYUP:
                cout << "pressed\n"; 
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&xMouse, &yMouse);
                cout << "MOUSE_DOWN x: " << xMouse << " y:" << yMouse << "\n";
                break;
            case SDL_MOUSEBUTTONUP:
                cout <<  "MOUSE_UP\n";
                break;
            case SDL_QUIT:
                exit(0);
                break;
        }
    }

    RenderTexture(renderer, Title);

    //Render circle

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    startVal.x = center.x + direction.x * radius;
    startVal.y = center.y + direction.y * radius;
    RenderPolygon(renderer, center, startVal); 



    //Rototate the startval
    double rad = 1  * PI / (double)180.0;
    //Normalize the vector
    float vecLength = sqrt(
            (direction.x * direction.x) +
            (direction.y * direction.y)
            );
    float dirX = direction.x / vecLength;
    float dirY = direction.y / vecLength;

    //apply transformation
    float newX = dirX * cos(rad) - dirY * sin(rad);
    float newY = dirX * sin(rad) + dirY * cos(rad);

    //round to 2 decimal points
    direction.x = roundf(newX * 100) / 100;
    direction.y = roundf(newY * 100) / 100; 




    /* 
    //emscripten_cancel_main_loop();
    //Set screen color back to screen defaults
    //SDL_SetRenderDrawColor(renderer, screenR, screenG, screenB, 255);

     */
    ////////////////////////////////////////////////////////////////////////
    //End of main game code
    ////////////////////////////////////////////////////////////////////////

    frameTime = SDL_GetTicks() - frameStart;

    if (FrameDelay > frameTime)
    {
        SDL_Delay(FrameDelay - frameTime);
    }
    //cout << "lopidy" << frameTime << "\n";
    //emscripten_cancel_main_loop();
}

//Need to use this main signature (int argv, char** args)
//Because SDL alters default program entry point resolving
//SDL calls your entry point expecting a specific signature,
//which is no longer subject to compiler interpretation
int main(int argv, char **args)
{
    cout << "Starting Game\n";


    //Initiate SDL
    StartSDL(&window, &renderer);


    screenR = 17;
    screenG = 17; 
    screenB = 17;
    SDL_Texture *titleTex = GetSDLTexture(renderer, window, "./res/png/title.png");
    RemoveTextureWhiteSpace(window, titleTex);
    Title = InitTexture(titleTex, 20, 20); 

    center.x = 250;
    center.y = 250;
    //startVal.x = 300;
    //startVal.y = 300;

    direction.x = 0;
    direction.y = 1;
    radius = 50;
#ifdef EMSCRIPTEN
    emscripten_set_main_loop(gameloop, 0, 0);
#else
    while(1)
    {
        gameloop();
    }
#endif 
    return 0;

}

