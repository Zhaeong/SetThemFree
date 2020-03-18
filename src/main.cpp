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
SDL_Texture *fontTexture;

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
int gameTime;

//Mood vars
int greenTime;
int redTime;
string Mood = "GREEN";
string nextMood;
Uint32 moodTransitionTime;


int childTime = 28;
int teenTime = 21;
int adultTime = 25;


//int childTime = 2;
//int teenTime = 2;
//int adultTime = 2;


//Childhood vars
string guidanceState = "MINE";
int guidanceSpeed;

//Teen vars
bool moveLeft = true;
int moveLeftBound, moveRightBound;

//Adults vars
SDL_Point topPoint, botPoint;
string movement = "TOP";

Texture LeftChallenge[10];
Texture RightChallenge[10];
Texture MidChallenge[10];
int numMidChallenge;
SDL_Texture *challengeTex;

SDL_Point startCol = {-1, -1};
SDL_Point endCol = {-1, -1};

int colLineAlpha = 255;
Uint32 score;

//Music

SDL_AudioDeviceID audioDevice;

AudioClip ToddlerMus;
AudioClip ChildMus;
AudioClip TeenMus;
AudioClip AdultMus;


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
                            gameStartTime = SDL_GetTicks();
                            frameStart = SDL_GetTicks();
                            GameState = "FADETITLE";
                            PlayAudio(audioDevice, ToddlerMus);
                        }
                        else if(GameState == "TODDLER")
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
                        cout << "radius: " << radius << "\n";

                    }

                    if(GameState == "CHILDHOOD" || GameState == "TEEN")
                    {
                        if(TextureMouseCollisionSingle(GiveGuidance, mousePoint.x, mousePoint.y))
                        {
                            if(guidanceState == "MINE")
                            {
                                guidanceState = "GIVING";
                            }
                        }
                    }
                    break;
                }
            case SDL_QUIT:
                exit(0);
                break;
        }
    }
    if(GameState == "MENU")
    {
        gameTime = 0;
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
            //Random seed whenever game starts
            srand(time(NULL));

            //Add 1 because transition time can't be 0
            greenTime =  rand() % 8 + 1;
            redTime = rand() % 8 + 1;

            //Set Childhood state to 30s
            stateBeginTime = SDL_GetTicks() - gameStartTime;
            nextStateTime = gameStartTime + (childTime * 1000);

            //Set guidanceSpeed to be faster when the polygon is smaller
            guidanceSpeed = 4;

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

            stateBeginTime = SDL_GetTicks() - gameStartTime;
            //Transition to Teen
            nextStateTime = nextStateTime + (teenTime * 1000);

            SDL_ClearQueuedAudio(audioDevice);
            PlayAudio(audioDevice, ChildMus);
        }

    }
    else if(GameState == "CHILDHOOD")
    {
        if(GiveGuidance.mAlpha + 5 < 255)
        {
            GiveGuidance.mAlpha += 5;
        }
        else
        {
            GiveGuidance.mAlpha = 255;
            Guidance.mAlpha = 255;
        }
        rotation = 1;
        if(guidanceState == "GIVING")
        {
            Guidance.mY -= guidanceSpeed;
        }
        int maxGuidance = 0;

        bool increment = true;

        if(nextMood == "GREEN")
        {
            increment = false;
        }
        //Pass in guildance midpoint
        if(CheckGuidancePolygonCollision(triangleArray, Guidance.mX ,Guidance.mY, Guidance.mW, &maxGuidance, increment))
        {
            Guidance.mY = GiveGuidance.mY;
            guidanceState = "MINE";
        }

        if(frameStart > nextStateTime)
        {
            GameState = "TEEN";
            screenColor.r = 0;
            screenColor.g = 122; 
            screenColor.b = 174;
            moveLeftBound = 10;
            moveRightBound = GAMEWIDTH - 10;

            SDL_ClearQueuedAudio(audioDevice);
            PlayAudio(audioDevice, TeenMus);

            //Transition to adulthood
            nextStateTime = nextStateTime + (adultTime * 1000);
        }
    }
    else if(GameState == "TEEN")
    {
        rotation = 2;
        if(guidanceState == "GIVING")
        {
            Guidance.mY -= guidanceSpeed;
            if(Guidance.mY < 0)
            {
                Guidance.mY = GiveGuidance.mY;
                guidanceState = "MINE";
            }
        }
        int maxGuidance = 0;

        bool increment = true;
        if(nextMood == "GREEN")
        {
            increment = false;
        }
        //Pass in guildance midpoint
        if(CheckGuidancePolygonCollision(triangleArray, Guidance.mX ,Guidance.mY, Guidance.mW, &maxGuidance, increment))
        {
            Guidance.mY = GiveGuidance.mY;
            guidanceState = "MINE";
        }

        //Move the polygon from side to side
        if(moveLeft)
        {
            if(center.x - (radius + maxGuidance) > moveLeftBound)
            {
                center.x -= 1;
            }
            else
            {
                moveLeft = false;
            }
        }
        else
        {
            if(center.x + (radius + maxGuidance) < moveRightBound)
            {
                center.x += 1;
            }
            else
            {
                moveLeft = true;
            }

        }

        if(frameStart > nextStateTime)
        {

            GameState = "ADULT";
            screenColor.r = 0;
            screenColor.g = 40; 
            screenColor.b = 81;
            Guidance.mAlpha = 0;
            GiveGuidance.mAlpha = 0;

            topPoint.x = GAMEWIDTH/2;
            topPoint.y = GAMEHEIGHT/2 - 80;

            botPoint.x = GAMEWIDTH/2;
            botPoint.y = GAMEHEIGHT/2 + 80;

            SDL_ClearQueuedAudio(audioDevice);
            PlayAudio(audioDevice, AdultMus);

            InitChallengeTexture(challengeTex, LeftChallenge, 10, true);
            InitChallengeTexture(challengeTex, RightChallenge, 10, false);

            //Add more mid challenges if radius is very big
            int addVal = 0;
            if(radius > 100)
            {
                addVal = 3;
            }
            numMidChallenge = (rand() % 4) + (2 + addVal);

            cout << "numMid: " << numMidChallenge << "\n";
            InitMidChallengeTexture(challengeTex, MidChallenge, numMidChallenge);

        }
    }
    if(GameState == "ADULT")
    {
        //Move towards top
        if(movement == "TOP")
        {
            if(center.x != topPoint.x)
            {
                if(center.x > topPoint.x)
                {
                    center.x -= 1;
                }
                else
                {
                    center.x += 1;
                }
            }

            if(center.y != topPoint.y)
            {
                center.y -= 1;    
            }

            if(center.y == topPoint.y && center.x == topPoint.x)
            {
                movement = "BOT";
            }
        }
        else if(movement == "BOT")
        {
            if(center.y != botPoint.y)
            {
                center.y += 1;    
            }

            if(center.y == botPoint.y)
            {
                movement = "FLY";
            }

        }
        else if(movement == "FLY")
        {
            CollisionMarker col;
            if(moveLeft)
            {
                col = CheckChallengePolygonCollision(LeftChallenge, 10, triangleArray); 
                if(col.colState == 1)
                {
                    moveLeft = false;
                    startCol = col.startPoint;
                    endCol = col.endPoint;
                    colLineAlpha = 255;

                }
                else if(col.colState == 0)
                {
                    center.x -= 1;
                }
                else if(col.colState == 2)
                {
                    movement = "END";
                }
            }
            else
            { 
                col = CheckChallengePolygonCollision(RightChallenge, 10, triangleArray); 
                if(col.colState == 1)
                {
                    moveLeft = true;
                    startCol = col.startPoint;
                    endCol = col.endPoint;

                    colLineAlpha = 255;

                }
                else if(col.colState == 0) 
                {
                    center.x += 1;
                }
                else if(col.colState == 2)
                {
                    movement = "END";
                }
            }

            //Also check for mid challenges
            col = CheckChallengePolygonCollision(MidChallenge, numMidChallenge, triangleArray); 
            if(col.colState == 1)
            {
                int xVal= (rand() % (GAMEWIDTH - 70)) + 70;
                int yVal = (rand() % GAMEHEIGHT) -1000;

                MidChallenge[col.index].mX = xVal;
                MidChallenge[col.index].mY = yVal;

                startCol = col.startPoint;
                endCol = col.endPoint;

                colLineAlpha = 255;
            }
            else if(col.colState == 2)
            {
                movement = "END";
            }


        }
        if(movement == "FLY" || movement == "BOT")
        {
            IncrementChallengeTextures(LeftChallenge, 10, true);
            IncrementChallengeTextures(RightChallenge, 10, false);
            IncrementMidChallengeTextures(MidChallenge, numMidChallenge);
        }
        if(movement == "END")
        {
            SDL_ClearQueuedAudio(audioDevice);
            score = SDL_GetTicks() - gameStartTime;
            cout << "Score: " << score << "\n";
            movement = "RESTART";
        }


    }

    //Mood changing calculations
    if(GameState == "TODDLER" || GameState == "CHILDHOOD" || GameState == "TEEN")
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
    if(GameState == "ADULT")
    {
        //Render challenges
        RenderTextureArray(renderer, LeftChallenge, 10); 
        RenderTextureArray(renderer, RightChallenge, 10); 
        RenderTextureArray(renderer, MidChallenge, numMidChallenge);

        //Render col line

        if(colLineAlpha > 0)
        {
            colLineAlpha -= 5;
        }
        else
        {
            colLineAlpha = 0;
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, colLineAlpha);

        SDL_RenderDrawLine(renderer, startCol.x, startCol.y, endCol.x, endCol.y);

    }

    //Set heart in middle of polygon
    HeartGreen.mX = center.x - (HeartGreen.mW/2);
    HeartGreen.mY = center.y - (HeartGreen.mH/2);

    HeartRed.mX = center.x - (HeartRed.mW/2);
    HeartRed.mY = center.y - (HeartRed.mH/2);

    RenderTexture(renderer, Title);

    if(movement != "END" && movement != "RESTART")
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        //Render circle
        RenderTriangleArray(renderer, triangleArray, center);
    }


    //Render Heart 
    RenderTexture(renderer, HeartGreen);
    RenderTexture(renderer, HeartRed);
    RenderTexture(renderer, Guidance);
    RenderTexture(renderer, GiveGuidance);
    if(GameState != "MENU")
    {
        gameTime = (frameStart - gameStartTime) / 1000;
    }
    //cout << "GameTime: " << gameTime << "\n";
    //RenderTexture(renderer, NumberFont);

    RenderFont(renderer, fontTexture, to_string(gameTime), center.x, center.y);
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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    screenColor.r = 191;
    screenColor.g = 232; 
    screenColor.b = 242;
    screenColor.a = 255;

    // 
    //Texture Inits
    //
    fontTexture = GetSDLTexture(renderer, window, "./res/png/numbers.png");
    RemoveTextureWhiteSpace(fontTexture);

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

    challengeTex = GetSDLTexture(renderer, window, "./res/png/challenge.png");
    RemoveTextureWhiteSpace(challengeTex);

    //Music Inits

    ToddlerMus = InitAudio("./res/music/toddler.wav");
    ChildMus = InitAudio("./res/music/child.wav");
    TeenMus = InitAudio("./res/music/teen.wav");
    AdultMus = InitAudio("./res/music/adult.wav");


    audioDevice = SDL_OpenAudioDevice(NULL, 0, &ToddlerMus.wavSpec, NULL, 0);
    if (audioDevice == 0) {
        printf("Failed to open audio: %s", SDL_GetError());
    } 



    SDL_PauseAudioDevice(audioDevice, 0);



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

