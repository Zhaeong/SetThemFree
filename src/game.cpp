#include "game.h"

int StartSDL(SDL_Window **window, SDL_Renderer **renderer)
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Couldn't initialize SDL: %s",
                SDL_GetError());
    }

    //Init Audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Couldn't initialize SDL: %s",
                SDL_GetError());
    }

    //Uint32 windowType = SDL_WINDOW_FULLSCREEN;
    Uint32 windowType = SDL_WINDOW_RESIZABLE;

    //SDL_WINDOW_FULLSCREEN
    //SDL_WINDOW_RESIZABLE
    if (SDL_CreateWindowAndRenderer(GAMEWIDTH,
                GAMEHEIGHT,
                windowType,
                window,
                renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Couldn't create window and renderer: %s",
                SDL_GetError());
    }

    return 0;
}

SDL_Texture *GetSDLTexture(SDL_Renderer *renderer, SDL_Window *window, string textureLocation)
{
    //Make sure to initialize texture to null or else SDL_DestroyTexture will crash program
    SDL_Texture *texture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(textureLocation.c_str());

    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n",
                textureLocation.c_str(),
                IMG_GetError());
    }
    else
    {
        //Convert surface to display format

        //Note have to use SDL_PIXELFORMAT_ARGB8888 due to window using SDL_PIXELFORMAT_RGB888
        //Which doesn't have alpha channel

        //SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( loadedSurface,
        //                                                          SDL_GetWindowPixelFormat( window ),
        //                                                          0 );

        cout << "Loading Texture: " << textureLocation << "\n";
        SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface,
                TEXTUREFORMAT,
                0);

        if (formattedSurface == NULL)
        {
            printf("Unable to convert loaded surface to display format! SDL Error: %s\n",
                    SDL_GetError());
        }
        else
        {
            //Create blank streamable texture

            //texture = SDL_CreateTexture( renderer,
            //                             SDL_GetWindowPixelFormat( window ),
            //                             SDL_TEXTUREACCESS_STREAMING,
            //                             formattedSurface->w,
            //                             formattedSurface->h );

            texture = SDL_CreateTexture(renderer,
                    TEXTUREFORMAT,
                    SDL_TEXTUREACCESS_STREAMING,
                    formattedSurface->w,
                    formattedSurface->h);

            //texture = SDL_CreateTextureFromSurface( renderer, formattedSurface );
            if (texture == NULL)
            {
                printf("Unable to create blank texture! SDL Error: %s\n",
                        SDL_GetError());
            }
            else
            {
                void *mPixels;
                int mPitch;

                SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

                //Lock texture for manipulation
                if (SDL_LockTexture(texture, NULL, &mPixels, &mPitch) != 0)
                {
                    printf("Unable to lock texture! %s\n", SDL_GetError());
                }

                //Copy loaded/formatted surface pixels
                memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

                //Unlock texture to update
                SDL_UnlockTexture(texture);

                mPixels = NULL;

                //Get image dimensions
                //mWidth = formattedSurface->w;
                //mHeight = formattedSurface->h;
            }

            //Get rid of old formatted surface
            SDL_FreeSurface(formattedSurface);
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return texture;
}

void RemoveTextureWhiteSpace(SDL_Texture *texture)
{
    void *mPixels;
    int mPitch;

    if (texture == NULL)
    {
        printf("Input Texture is null in RemoveTextureWhiteSpace! SDL Error: %s\n",
                SDL_GetError());
    }

    if (SDL_LockTexture(texture, NULL, &mPixels, &mPitch) != 0)
    {
        printf("Unable to lock texture! %s\n", SDL_GetError());
    }
    else
    {
        //Allocate format from window
        //Uint32 format = SDL_GetWindowPixelFormat( window );

        SDL_PixelFormat *mappingFormat = SDL_AllocFormat(TEXTUREFORMAT);

        int texWidth = 0;
        int texHeight = 0;

        Uint32 texFormat;
        SDL_QueryTexture(texture, &texFormat, NULL, &texWidth, &texHeight);

        //Get pixel data
        Uint32 *pixels = (Uint32 *)mPixels;
        int pixelCount = (mPitch / 4) * texHeight;

        //cout << "texformat:" << texFormat << "\n";
        //cout << "texWidth:" << texWidth << "\n";
        //cout << "texHeight:" << texHeight << "\n";
        //cout << "mPitch:" << mPitch << "\n";
        //cout << "pixelCount:" << pixelCount << "\n";

        //Map colors

        Uint32 colorKeyWhite = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0xFF);
        Uint32 colorKeyGray = SDL_MapRGBA(mappingFormat, 195, 195, 195, 0xFF);

        Uint32 transparent = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0);

        //Color key pixels
        for (int i = 0; i < pixelCount; ++i)
        {
            //cout << "col:" << pixels[i] << "\n";
            //cout << "key:" << colorKey << "\n";

            if (pixels[i] == colorKeyWhite || pixels[i] == colorKeyGray)
            {
                pixels[i] = transparent;
            }
        }

        SDL_UnlockTexture(texture);
        //mPixels = NULL;
        //Free format
        SDL_FreeFormat(mappingFormat);
    }
}

Texture InitTexture(SDL_Texture *sdlTexture, int x, int y)
{
    Texture outTex;
    outTex.mX = x;
    outTex.mY = y;
    SDL_QueryTexture(sdlTexture, NULL, NULL, &outTex.mW, &outTex.mH);
    outTex.mRotation = 0;
    outTex.mAlpha = 255;
    outTex.mTexture = sdlTexture;
    outTex.mCenter = NULL;
    outTex.mFlip = SDL_FLIP_NONE;
    return outTex;
}

void RenderTexture(SDL_Renderer *renderer, Texture tex)
{
    //Don't render if the alpha is 0
    if(tex.mAlpha > 0)
    {
        SDL_SetTextureBlendMode(tex.mTexture, SDL_BLENDMODE_BLEND);

        SDL_SetTextureAlphaMod(tex.mTexture, tex.mAlpha);

        SDL_Rect srcRect;
        SDL_Rect dstRect;

        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.h = tex.mH;
        srcRect.w = tex.mW;

        dstRect.x = tex.mX;
        dstRect.y = tex.mY;
        dstRect.h = tex.mH;
        dstRect.w = tex.mW;

        SDL_RenderCopyEx(renderer, tex.mTexture, &srcRect, &dstRect, tex.mRotation, tex.mCenter, tex.mFlip);
    }
}

void SetTextureColor(SDL_Texture *texture, int R, int G, int B, int A)
{
    void *mPixels;
    int mPitch;

    if (texture == NULL)
    {
        printf("Input Texture is null in RemoveTextureWhiteSpace! SDL Error: %s\n",
                SDL_GetError());
    }

    if (SDL_LockTexture(texture, NULL, &mPixels, &mPitch) != 0)
    {
        printf("Unable to lock texture! %s\n", SDL_GetError());
    }
    else
    {
        SDL_PixelFormat *mappingFormat = SDL_AllocFormat(TEXTUREFORMAT);

        int texWidth = 0;
        int texHeight = 0;

        Uint32 texFormat;
        SDL_QueryTexture(texture, &texFormat, NULL, &texWidth, &texHeight);

        //Get pixel data
        Uint32 *pixels = (Uint32 *)mPixels;
        int pixelCount = (mPitch / 4) * texHeight;

        //Map colors

        Uint32 colorKey = SDL_MapRGBA(mappingFormat, R, G, B, A);

        Uint32 transparent = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0);

        //Color key pixels
        for (int i = 0; i < pixelCount; ++i)
        {

            if (pixels[i] != transparent)
            {
                pixels[i] = colorKey;
            }
        }

        SDL_UnlockTexture(texture);

        //Free format
        SDL_FreeFormat(mappingFormat);
    }
}

void InitTriangleArray(Triangle *triangleArray, int radius)
{
    vect2 curDirection = {0 ,1};

    //Convert degrees to radians
    double rad = 45  * PI / (double)180.0;
    double cosVal = cos(rad);
    double sinVal = sin(rad);

    for(int i = 0; i < 8; i++)
    { 
        Triangle polygonTri;
        polygonTri.numGuidance = 0;
        polygonTri.direction.x = curDirection.x;
        polygonTri.direction.y = curDirection.y;
        polygonTri.radius = radius;

        triangleArray[i] = polygonTri;
        //Now apply 45 degree rotation of the direction
        //Normalize the vector
        float vecLength = sqrt(
                (curDirection.x * curDirection.x) +
                (curDirection.y * curDirection.y)
                );
        float dirX = curDirection.x / vecLength;
        float dirY = curDirection.y / vecLength;

        //apply transformation
        float newX = dirX * cosVal - dirY * sinVal;
        float newY = dirX * sinVal + dirY * cosVal;

        //round to 2 decimal points
        curDirection.x = roundf(newX * 100) / 100;
        curDirection.y = roundf(newY * 100) / 100; 

    }

}

void RenderTriangleArray(SDL_Renderer *renderer, Triangle *triangleArray, SDL_Point center)
{
    //Convert degrees to radians
    double rad = 45  * PI / (double)180.0;
    double cosVal = cos(rad);
    double sinVal = sin(rad);
    for(int i = 0; i < 8; i++)
    { 
        Triangle polygonTri = triangleArray[i];

        int radius = polygonTri.radius + polygonTri.numGuidance;
        vect2 secondDirection;
        SDL_Point firstPoint, secondPoint;



        //Now apply 45 degree rotation of the direction
        //Normalize the vector
        float vecLength = sqrt(
                (polygonTri.direction.x * polygonTri.direction.x) +
                (polygonTri.direction.y * polygonTri.direction.y)
                );
        float dirX = polygonTri.direction.x / vecLength;
        float dirY = polygonTri.direction.y / vecLength;

        //apply transformation
        float newX = dirX * cosVal - dirY * sinVal;
        float newY = dirX * sinVal + dirY * cosVal;

        //round to 2 decimal points
        secondDirection.x = roundf(newX * 100) / 100;
        secondDirection.y = roundf(newY * 100) / 100;

        firstPoint.x = center.x + (polygonTri.direction.x * radius);
        firstPoint.y = center.y + (polygonTri.direction.y * radius);

        secondPoint.x = center.x + (secondDirection.x * radius);
        secondPoint.y = center.y + (secondDirection.y * radius);

        SDL_RenderDrawLine(renderer, firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);

        polygonTri.startPoint = firstPoint;
        polygonTri.endPoint = secondPoint;

        triangleArray[i] = polygonTri;

        //Now render all the lines before in multiples of 4
        for(int j = 0; j < polygonTri.numGuidance; j += 4)
        {
            radius -= 4;
            firstPoint.x = center.x + (polygonTri.direction.x * radius);
            firstPoint.y = center.y + (polygonTri.direction.y * radius);

            secondPoint.x = center.x + (secondDirection.x * radius);
            secondPoint.y = center.y + (secondDirection.y * radius);

            SDL_RenderDrawLine(renderer, firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);
        }
    }

}

void UpdateTriangleArrayRadius(Triangle *triangleArray, int value)
{
    for(int i = 0; i < 8; i++)
    { 
        triangleArray[i].radius += value;
    } 
}


void RotateTriangleArray(Triangle *triangleArray, int rotation)
{
    for(int i = 0; i < 8; i++)
    { 
        double rad = rotation * PI / (double)180.0;
        //Normalize the vector
        float vecLength = sqrt(
                (triangleArray[i].direction.x * triangleArray[i].direction.x) +
                (triangleArray[i].direction.y * triangleArray[i].direction.y)
                );
        float dirX = triangleArray[i].direction.x / vecLength;
        float dirY = triangleArray[i].direction.y / vecLength;

        //apply transformation
        float newX = dirX * cos(rad) - dirY * sin(rad);
        float newY = dirX * sin(rad) + dirY * cos(rad);

        //round to 2 decimal points
        triangleArray[i].direction.x = roundf(newX * 1000) / 1000;
        triangleArray[i].direction.y = roundf(newY * 1000) / 1000; 
    } 
}

bool CheckPointInCircle(SDL_Point circleCenter, int radius, SDL_Point checkPoint)
{
    //Compute distance between point and circlecenter, and check if that distance is smaller than radius
    //No need to square root due, just square radius 
    if ((checkPoint.x - circleCenter.x) * (checkPoint.x - circleCenter.x) + 
            (checkPoint.y - circleCenter.y) * (checkPoint.y - circleCenter.y) <= radius * radius) 
    {
        return true; 
    } 
    else
    { 
        return false; 
    }
} 

bool TextureMouseCollisionSingle(Texture mTexture, int xPos, int yPos)
{

    if (xPos >= mTexture.mX 
            && xPos <= (mTexture.mX + mTexture.mW) 
            && yPos >= mTexture.mY
            && yPos <= (mTexture.mY + mTexture.mH) 
       )
    {
        return true;
    }

    return false;
}

bool CheckGuidancePolygonCollision(Triangle *triangleArray, int guidanceX, int guidanceY, int guidanceW, int *maxGuidance, bool increment)
{
    bool isCollided = false;

    int iCollided = 0;
    int nearestY = 0;
    int maxGuide = 0;

    for(int i = 0; i < 8; i++)
    { 
        Triangle polygonTri = triangleArray[i];

        //find leftmost point
        int leftX  = polygonTri.startPoint.x;
        int rightX = polygonTri.endPoint.x;
        if(polygonTri.endPoint.x < leftX)
        {
            leftX = polygonTri.endPoint.x;
            rightX = polygonTri.startPoint.x;
        }

        if(polygonTri.numGuidance > maxGuide)
        {
            maxGuide = polygonTri.numGuidance;
        }

        //check x collision
        if(rightX >= guidanceX && (guidanceX + guidanceW) >= leftX)
        {
            //check y collision
            if(guidanceY < polygonTri.startPoint.y)
            {
                isCollided = true;

                if(polygonTri.endPoint.y > nearestY)
                {
                    nearestY = polygonTri.endPoint.y;
                    iCollided = i;
                }
            }
            if(guidanceY < polygonTri.endPoint.y)
            {
                isCollided = true;

                if(polygonTri.startPoint.y > nearestY)
                {
                    nearestY = polygonTri.startPoint.y;
                    iCollided = i;
                }
            }
        }
    } 

    if(isCollided)
    {
        if(increment)
        {
            if(triangleArray[iCollided].numGuidance + triangleArray[iCollided].radius < 150)
            {
                triangleArray[iCollided].numGuidance += 4;
            }
        }
        else
        {
            if(triangleArray[iCollided].numGuidance >= 4)
            {
                triangleArray[iCollided].numGuidance -= 4;
            }
        }
    }
    *maxGuidance = maxGuide;
    return isCollided;

}


void InitChallengeTexture(SDL_Texture *challengeTex, Texture *textureArray, int numTextures, bool isLeft)
{
    int yVal = -50 * numTextures;
    for(int i = 0; i < numTextures; i++)
    {
        Texture Challenge = InitTexture(challengeTex, 0, yVal); 
        //Randomize width
        int WidthVal = rand() % 50 + 10;
        int xVal = 0;
        if(!isLeft)
        {
            xVal = GAMEWIDTH - WidthVal;
        }

        Challenge.mW = WidthVal;
        Challenge.mX = xVal;
        textureArray[i] = Challenge;
        yVal += 50;
    }
}


void InitMidChallengeTexture(SDL_Texture *challengeTex, Texture *textureArray, int numTextures)
{
    for(int i = 0; i < numTextures; i++)
    {
        int xVal= (rand() % (GAMEWIDTH - 70)) + 70;
        int yVal = (rand() % GAMEHEIGHT) -1000;

        Texture Challenge = InitTexture(challengeTex, xVal, yVal); 
        textureArray[i] = Challenge;
    }
}

void IncrementChallengeTextures(Texture *textureArray, int numTextures, bool isLeft)
{

    for(int i = 0; i < numTextures; i++)
    {
        if(textureArray[i].mY >= GAMEHEIGHT)
        {
            int newWidth = rand() % 60 + 10;
            textureArray[i].mW = newWidth;
            textureArray[i].mY = -50;

            if(!isLeft)
            {
                textureArray[i].mX = GAMEWIDTH - newWidth;
            }
        }
        else
        {
            textureArray[i].mY += 1;
        }
    }

}

void IncrementMidChallengeTextures(Texture *textureArray, int numTextures)
{

    for(int i = 0; i < numTextures; i++)
    {
        if(textureArray[i].mY >= GAMEHEIGHT)
        {

            int xVal= (rand() % (GAMEWIDTH - 70)) + 70;
            int yVal = (rand() % GAMEHEIGHT) -1000;

            textureArray[i].mX = xVal;
            textureArray[i].mY = yVal;
        }
        else
        {
            textureArray[i].mY += 1;
        }
    }

}

CollisionMarker CheckChallengePolygonCollision(Texture *challengeArray, int numChallege, Triangle *triangleArray)
{
    CollisionMarker col;
    col.colState = 0;

    //Go through every triangle and see if any of them collides with challenge
    for(int i = 0; i < 8; i++)
    { 
        Triangle polygonTri = triangleArray[i];
        //find leftmost point

        int leftX = polygonTri.startPoint.x;
        int leftY = polygonTri.startPoint.y;

        int rightX = polygonTri.endPoint.x;
        int rightY = polygonTri.endPoint.y;

        if(polygonTri.endPoint.x < leftX)
        {
            leftX = polygonTri.endPoint.x;
            leftY = polygonTri.endPoint.y;
            rightX = polygonTri.startPoint.x;
            rightY = polygonTri.startPoint.y;
        }


        int gCollision = -1;
        for(int c_i = 0; c_i < numChallege; c_i++)
        {
            Texture guildance = challengeArray[c_i];

            //Check x collision
            if(rightX >= guildance.mX && (guildance.mX + guildance.mW) >= leftX)
            {
                //Check y collision
                if(rightY >= guildance.mY && (guildance.mY + guildance.mH) >= leftY)
                {
                    gCollision = c_i;
                    col.startPoint.x = leftX;
                    col.startPoint.y = leftY;
                    col.endPoint.x = rightX;
                    col.endPoint.y = rightY;
                }
            }
        }
        if(gCollision != -1)
        {
            if(polygonTri.numGuidance >= 4)
            {
                polygonTri.numGuidance -= 4;
                triangleArray[i] = polygonTri;
            }
            else
            {
                //decrement radius of all triangles
                for(int t_i = 0; t_i < 8; t_i++)
                {
                    if(triangleArray[t_i].radius > 25)
                    {
                        if(triangleArray[t_i].radius - 4 < 24)
                        {
                            triangleArray[t_i].radius = 24;
                        }
                        triangleArray[t_i].radius -= 4;
                    }
                    else
                    {
                        cout << "RIP\n";
                        col.colState = 2;
                        return col;
                    }
                }
            }
            col.index = gCollision;
            col.colState = 1;
            return col;
        }
    }

    return col; 
}

void RenderTextureArray(SDL_Renderer *renderer, Texture *textureArray, int numTextures)
{
    for(int i = 0; i < numTextures; i++)
    {
        RenderTexture(renderer, textureArray[i]);
    }

}


AudioClip InitAudio(string filepath)
{
    AudioClip clip;
    clip.wavPath = filepath;
    if (SDL_LoadWAV(clip.wavPath.c_str(), &clip.wavSpec, &clip.wavBuffer, &clip.wavLength) == NULL) 
    {
        fprintf(stderr, "Could not open wav %s: %s\n", filepath.c_str(), SDL_GetError());
    } 
    return clip;
}


void PlayAudio(SDL_AudioDeviceID audioDevice, AudioClip clip)
{
    int success = SDL_QueueAudio(audioDevice, clip.wavBuffer, clip.wavLength);
    if(success < 0)
    {
        printf("SDL_QueueAudio failed %s, err: %s", clip.wavPath.c_str(), SDL_GetError()); 
    }

}
