#include "main.h"


//Declarations for sdl window displaying
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

const int FPS = 60;
//How many miliseconds per frame
const int FrameDelay = 1000 / FPS;

string GameState = "START";

//Initial Loading
SDL_Rect srcRect;
SDL_Rect dstRect;

Texture Title;
Texture Heart;
int screenR, screenG, screenB;

SDL_Point center;

vect2 direction;
int radius;

SDL_Point polygonArray[8];

int rotation = 0;

Uint32 gameTime = 0;
Uint32 startTime = 0;

//Toddler vars
SDL_Color reject = {255, 0, 0, 150};
SDL_Color listen = {0, 255, 0, 150};
SDL_Color curColor;


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
                cout << "MOUSE_DOWN \n";
                break;
            case SDL_MOUSEBUTTONUP:
                {
                    cout <<  "MOUSE_UP\n";
                    //rotation+=1;
                    SDL_GetMouseState(&xMouse, &yMouse);

                    SDL_Point mousePoint = {xMouse, yMouse};
                    if(CheckPointInCircle(center, radius, mousePoint))
                    {
                        cout << "In polygon\n";
                        if(GameState == "START")
                        {
                            GameState = "FADETITLE";
                            curColor = listen;

                        }

                    }
                    Heart.mW += 1;
                    Heart.mH += 1;
                    cout << "GameState: " << GameState << "\n";
                    break;
                }
            case SDL_QUIT:
                exit(0);
                break;
        }
    }

    if(GameState == "FADETITLE")
    {
        if(Title.mAlpha > 0)
        {
            Title.mAlpha -= 5;  
        }
        else
        {
            GameState = "TODDLER";

        }
    }
    else if(GameState == "TODDLER")
    {
        SetTextureColor(Heart.mTexture, curColor.r, curColor.g, curColor.b, curColor.a);
        curColor.r += 1;
        curColor.g -= 1;
    }

    //Calculate poly points
    GetPolygonPoints(polygonArray, center, radius, direction);

    //Set heart in middle of polygon
    Heart.mX = center.x - (Heart.mW/2);
    Heart.mY = center.y - (Heart.mH/2);

    RenderTexture(renderer, Title);


    //Render circle
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    RenderPolygon(renderer, polygonArray);

    //Render Heart 
    RenderTexture(renderer, Heart);

    //Rototate the startval
    direction = RotateVector(direction, rotation); 


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

    screenR = 191;
    screenG = 232; 
    screenB = 242;
    SDL_Texture *titleTex = GetSDLTexture(renderer, window, "./res/png/title.png");
    RemoveTextureWhiteSpace(titleTex);
    Title = InitTexture(titleTex, 20, 20); 
    Title.mX = 500/2 - (Title.mW/2);
    Title.mY = 50;
    SDL_Texture *heartTex = GetSDLTexture(renderer, window, "./res/png/heart.png");
    RemoveTextureWhiteSpace(heartTex);
    Heart = InitTexture(heartTex, 20, 20);  

    center.x = 250;
    center.y = 250;

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

