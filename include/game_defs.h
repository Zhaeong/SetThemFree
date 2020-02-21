#ifndef GAMEDEFS_H
#define GAMEDEFS_H

#include <string>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>


using namespace std;

//Colors
/*
Base: 192
Light: 236

 */



//Be carefull to always set a size divisible cleanly by 3
#define GAMEWIDTH  640
#define GAMEHEIGHT 480


#define DEBRISWIDTH 700
#define DEBRISHEIGHT 540

#define PI 3.14159265

#define DEBUG 0 

//Extern so that any other files that include this file will be able to use this variable
extern int SHADE;

const Uint8 MOD_R = 100;
const Uint8 MOD_G = 100;
const Uint8 MOD_B = 100;;

const Uint32 TEXTUREFORMAT = SDL_PIXELFORMAT_RGBA8888;

struct vect2
{
  float x;
  float y;
};

struct vect2int
{
  int x;
  int y;
};

struct TextObj
{
  string mString = "";
  int mX = 0;
  int mY = 0;
  int mWidth = 0;
  unsigned int mDelay = 0;
  unsigned int mRenderDelay = 0;
  unsigned int mLastTime = 0;
  unsigned int mLetters = 0;
  unsigned int mTimeElapsed = 0;
  bool enabled = true;
  bool finished = 0;
};

//GAME STATES
const string STATE_INTRO = "INTRO";
const string STATE_GAME = "GAME";
const string STATE_PAUSE = "PAUSE";

//StarBackground
const int NUM_STARS = 50;

//SPACE UI
const int NUM_SPACE_UI = 19;

const string GRAYBACKGROUND            = "res/spaceUI/uiGray.png";

const string BTN_LEFTCURSOR            = "res/spaceUI/leftCursor.png";
const string BTN_LEFTCURSOR_ACTIVE     = "res/spaceUI/leftCursorActive.png";

const string BTN_RIGHTCURSOR           = "res/spaceUI/rightCursor.png";
const string BTN_RIGHTCURSOR_ACTIVE    = "res/spaceUI/rightCursorActive.png";

const string BTN_STOPROT               = "res/spaceUI/stopRotate.png";
const string BTN_STOPROT_ACTIVE        = "res/spaceUI/stopRotateActive.png";

const string BTN_INCREASESPEED         = "res/spaceUI/increaseSpeed.png";
const string BTN_INCREASESPEED_ACTIVE  = "res/spaceUI/increaseSpeedActive.png";

const string BTN_DECREASESPEED         = "res/spaceUI/decreaseSpeed.png";
const string BTN_DECREASESPEED_ACTIVE  = "res/spaceUI/decreaseSpeedActive.png";

const string BTN_HARVESTDEBRIS         = "res/spaceUI/harvestDebrisGray.png";
const string BTN_HARVESTDEBRIS_ENABLE  = "res/spaceUI/harvestDebrisEnable.png";
const string BTN_HARVESTDEBRIS_ACTIVE  = "res/spaceUI/harvestDebrisActive.png";

const string DEBRIS_TRACKER  = "res/spaceUI/debrisTracker.png";

const string SPEED_0  = "res/spaceUI/speed0.png";
const string SPEED_1  = "res/spaceUI/speed1.png";
const string SPEED_2  = "res/spaceUI/speed2.png";
const string SPEED_3  = "res/spaceUI/speed3.png";

const string COMMENDATION_1 = "res/spaceUI/commendation1.png";
const string CITATION       = "res/spaceUI/commendation2.png";

//INTRO UI
const int NUM_INTRO_UI = 2;
const string BTN_STARTGAME     = "res/dialogUI/startGame.png";


//INTER LEVEL UI
const int NUM_INTERUI = 2;
const string BTN_CONTINUE     = "res/dialogUI/continue.png";

//DEBRIS
const int NUM_DEBRIS = 10;
const int NUM_DEBRIS_TEX = 3;

const string DEBRIS_IMG = "res/debris/debris1.png";
const string DEBRIS_IMG2 = "res/debris/debris2.png";
const string DEBRIS_IMG3 = "res/debris/debris3.png";


//TEXT DISPLAY

/*
0 - Num Remaining Debris
1 - NUM Debris gotteth
2 - DEBUG x,y screen mouse coords
3 - DEBUG x,y world mouse coords
4 - DEBUG GameTime
5 - DEBUG HOLDDOWNTIME
*/
const int NUM_TEXT = 6;

/*
0 - Main Text
1 - Choice A Text
2 - Choice B Text
3 = Response A Text
4 = Response B Text
*/
const int NUM_TEXT_SURVEY = 5;

const int NUM_TEXT_INTRO = 11;

/*
0 - Main Text
1 - NUM Debris
2 - DEBUG x,y screen mouse coords
3 - DEBUG x,y world mouse coords
4 - DEBUG GameTime
5 - DEBUG HOLDDOWNTIME
*/

//Vector functions

static float vect2dot(vect2 v1, vect2 v2)
{
  return v1.x * v2.x + v1.y * v2.y;
}


#endif // GAMEDEFS_H
 
