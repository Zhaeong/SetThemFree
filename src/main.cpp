#include "main.h"


//Declarations for sdl window displaying
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

const int FPS = 60;
//How many miliseconds per frame
const int FrameDelay = 1000 / FPS;

string GameState = "MENU";
//string GameState = "CHILDHOOD";

//Initial Loading
SDL_Rect srcRect;
SDL_Rect dstRect;

//Texture declarations
Texture Title;
Texture HeartGreen;
Texture HeartRed;
Texture GiveGuidance;
Texture Guidance;

SDL_Color screenColor;

SDL_Point center;

vect2 direction;

Triangle triangleArray[8];

int radius;
int rotation = 0;

Uint32 gameStartTime = 0;
Uint32 stateBeginTime;
Uint32 nextStateTime;
//Mood vars
int greenTime;
int redTime;
string Mood = "GREEN";
string nextMood;
Uint32 moodTransitionTime;

//Childhood vars
string guidanceState = "MINE";
int guidanceSpeed;

void gameloop() 
{
    Uint32 frameStart;
    Uint32 frameTime;
    frameStart = SDL_GetTicks();  

    //The color at which the screen will be if alpha = 0 on all textures
    SDL_SetRenderDrawColor(renderer, screenColor.r, screenColor.g, screenColor.b, screenColor.a);

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
                        if(GameState == "MENU")
                        {
                            GameState = "FADETITLE";

                        }
                        else if(GameState == "TODDLER")
                        {
                            if(Mood == "STAY")
                            {
                                //next mood is opposite of current mood
                                if(nextMood == "RED")
                                {
                                    if(radius < 200)
                                    {
                                        radius += 1;
                                        UpdateTriangleArrayRadius(triangleArray, 1);
                                    }
                                }
                                else if(nextMood == "GREEN")
                                {
                                    if(radius > 28)
                                    {
                                        radius -= 1;
                                        UpdateTriangleArrayRadius(triangleArray, -1);

                                    }
                                }
                            }
                            cout <<"curRad: " << radius << "\n";
                        }


                    }

                    if(GameState == "CHILDHOOD")
                    {
                        cout << "mouse x: " << mousePoint.x << " mouse y: " << mousePoint.y << "\n";
                        cout << "tex x: " << GiveGuidance.mX << " y: " << GiveGuidance.mY << " w: " << GiveGuidance.mW << " h: " << GiveGuidance.mH << "\n";
                        if(TextureMouseCollisionSingle(GiveGuidance, mousePoint.x, mousePoint.y))
                        {
                            cout << "Gave Guidance\n";
                            if(guidanceState == "MINE")
                            {
                                guidanceState = "GIVING";
                            }
                        }
                    }
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
            gameStartTime = SDL_GetTicks();
            GameState = "TODDLER";
            //Random seed whenever game starts
            srand(time(NULL));

            //Add 1 because transition time can't be 0
            greenTime =  rand() % 8 + 1;
            redTime = rand() % 8 + 1;

            //Set Childhood state to 30s
            stateBeginTime = SDL_GetTicks() - gameStartTime;
            nextStateTime = gameStartTime + (5 * 1000);

            //Set guidanceSpeed to be faster when the polygon is smaller
            guidanceSpeed = (200 - radius) / 10 + 1;
            
            cout << "GreenTime: " << greenTime << " RedTime: " << redTime << "\n"; 
            cout << "Guidance speed: " << guidanceSpeed << "\n";
        }
    }
    else if(GameState == "TODDLER")
    {
        if(frameStart > nextStateTime)
        {
            GameState = "CHILDHOOD";
            screenColor.r = 0;
            screenColor.g = 162; 
            screenColor.b = 232;
            Guidance.mAlpha = 255;
            GiveGuidance.mAlpha = 255;
            stateBeginTime = SDL_GetTicks() - gameStartTime;
            cout << "State Childhood, gameStartTime: " << gameStartTime << " nextStateTime: " << nextStateTime << "\n";
        }

    }
    else if(GameState == "CHILDHOOD")
    {
        rotation = 1;
        if(guidanceState == "GIVING")
        {
            Guidance.mY -= guidanceSpeed;
        }

        //Pass in guildance midpoint
        if(CheckGuidancePolygonCollision(triangleArray, Guidance.mX ,Guidance.mY, Guidance.mW))
        {
            Guidance.mY = GiveGuidance.mY;
            guidanceState = "MINE";
        }
    }

    //Mood changing calculations
    if(GameState == "TODDLER" || GameState == "CHILDHOOD")
    {
        if(Mood == "GREEN")
        {
            //Set when to transition mood
            moodTransitionTime = frameStart + (greenTime * 1000);
            nextMood = "RED";
            Mood = "STAY";
        }
        else if(Mood == "RED")
        {
            moodTransitionTime = frameStart + (redTime* 1000);
            nextMood = "GREEN";
            Mood = "STAY";
        }
        else if(Mood == "STAY")
        {
            //After it passes mood transition time, tranlate mood
            if(frameStart > moodTransitionTime)
            {
                Mood = "TRANSITION";
                //cout << "InTransition\n";
            }
        }
        else if(Mood == "TRANSITION")
        {
            if(nextMood == "RED")
            {
                if(HeartRed.mAlpha < 255)
                {
                    HeartRed.mAlpha++;
                    HeartGreen.mAlpha--;
                }
                else
                {
                    Mood = "RED";
                    //cout << "Change to: " << Mood << "\n";
                }
            }
            if(nextMood == "GREEN")
            {
                if(HeartGreen.mAlpha < 255)
                {
                    HeartGreen.mAlpha++;
                    HeartRed.mAlpha--;
                }
                else
                {
                    Mood = "GREEN";
                    //cout << "Change to: " << Mood << "\n";

                }
            }
        }
    }

    //Set heart in middle of polygon
    HeartGreen.mX = center.x - (HeartGreen.mW/2);
    HeartGreen.mY = center.y - (HeartGreen.mH/2);

    HeartRed.mX = center.x - (HeartRed.mW/2);
    HeartRed.mY = center.y - (HeartRed.mH/2);

    RenderTexture(renderer, Title);


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //Render circle
    RenderTriangleArray(renderer, triangleArray, center);

    //Render Heart 
    RenderTexture(renderer, HeartGreen);
    RenderTexture(renderer, HeartRed);
    RenderTexture(renderer, Guidance);
    RenderTexture(renderer, GiveGuidance);

    //Rototate the startval
    if(rotation > 0)
    {
        RotateTriangleArray(triangleArray, rotation);
    }



    //float sideLength = (testTri.radius * 2) / ( (sqrt(2) + 2) / sqrt(2));

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

    screenColor.r = 191;
    screenColor.g = 232; 
    screenColor.b = 242;
    screenColor.a = 255;

    // 
    //Texture Inits
    //
    SDL_Texture *titleTex = GetSDLTexture(renderer, window, "./res/png/title.png");
    RemoveTextureWhiteSpace(titleTex);
    Title = InitTexture(titleTex, 20, 20); 
    Title.mX = GAMEWIDTH/2 - (Title.mW/2);
    Title.mY = 50;

    SDL_Texture *heartTexGreen = GetSDLTexture(renderer, window, "./res/png/heartGreen.png");
    RemoveTextureWhiteSpace(heartTexGreen);
    HeartGreen = InitTexture(heartTexGreen, 20, 20);  

    SDL_Texture *heartTexRED = GetSDLTexture(renderer, window, "./res/png/heartRed.png");
    RemoveTextureWhiteSpace(heartTexRED);
    HeartRed = InitTexture(heartTexRED, 20, 20); 
    HeartRed.mAlpha = 0;

    SDL_Texture *giveGuidanceTex = GetSDLTexture(renderer, window, "./res/png/giveGuidance.png");
    RemoveTextureWhiteSpace(giveGuidanceTex);
    GiveGuidance = InitTexture(giveGuidanceTex, 20, 20); 
    GiveGuidance.mAlpha = 0;
    GiveGuidance.mX = GAMEWIDTH/2 - (GiveGuidance.mW/2);
    GiveGuidance.mY = GAMEHEIGHT - GiveGuidance.mH;

    SDL_Texture *guidanceTex = GetSDLTexture(renderer, window, "./res/png/guidance.png");
    RemoveTextureWhiteSpace(guidanceTex);
    Guidance = InitTexture(guidanceTex, 20, 20); 
    Guidance.mAlpha = 0;
    Guidance.mX = GAMEWIDTH/2 - (Guidance.mW/2);
    //Same as previous
    Guidance.mY = GAMEHEIGHT - GiveGuidance.mH;

    center.x = 250;
    center.y = 250;

    direction.x = 0;
    direction.y = 1;
    radius = 50;

    InitTriangleArray(triangleArray, radius);

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

