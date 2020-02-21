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
  if(DEBUG == 1)
  {
    windowType = SDL_WINDOW_RESIZABLE;
  }
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

