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

    //Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
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
        Uint32 colorKeyGray = SDL_MapRGBA(mappingFormat, 153, 153, 153, 0xFF);

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
        vect2 secondDirection;
        SDL_Point firstPoint, secondPoint;

        firstPoint.x = center.x + (polygonTri.direction.x * polygonTri.radius);
        firstPoint.y = center.y + (polygonTri.direction.y * polygonTri.radius);

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

        secondPoint.x = center.x + (secondDirection.x * polygonTri.radius);
        secondPoint.y = center.y + (secondDirection.y * polygonTri.radius);

        SDL_RenderDrawLine(renderer, firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);
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
