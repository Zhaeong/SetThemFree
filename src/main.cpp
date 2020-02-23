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

float xDeca = 300;
float yDeca = 300;

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
    //Midpoint
    int centerX = 250;
    int centerY = 250;

    int radius = 150;



    //Convert degrees to radians
    double rad = 36 * PI / (double)180.0;

    float xVal = xDeca;
    float yVal = yDeca;
    float xNew;
    float yNew;

    double cosVal = cos(rad);
    double sinVal = sin(rad);
    //Clockwise rotation about center points
    for(int i = 0; i < 10; i++)
    { 
        //Origin point
        xNew = (cosVal * (xVal - centerX)) - (sinVal * (yVal - centerY)) + centerX;
        yNew = (sinVal * (xVal - centerX)) + (cosVal * (yVal - centerY)) + centerY;

        SDL_RenderDrawPoint(renderer, centerX, centerY);

        SDL_RenderDrawLine(renderer, xVal, yVal, xNew, yNew);
        //cout << "ori x: " << xVal << " y: " << yVal << "\n";
        //cout << "new x: " << xNew << " y: " << yNew << "\n";

        xVal = xNew;
        yVal = yNew;
    }

    //Translate xDeca by one degree 
/*
    rad = 45 * PI / (double)180.0;
    cosVal = cos(rad);
    sinVal = sin(rad);

    xDeca = (cosVal * (xDeca - centerX)) - (sinVal * (xDeca - centerY)) + centerX;
    yDeca = (sinVal * (xDeca - centerX)) + (cosVal * (yDeca - centerY)) + centerY;
*/
    yDeca += 1;
    //emscripten_cancel_main_loop();
    //Set screen color back to screen defaults
    //SDL_SetRenderDrawColor(renderer, screenR, screenG, screenB, 255);
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

    xDeca = 250;
    yDeca = 300;

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

