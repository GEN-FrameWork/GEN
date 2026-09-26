/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPBitmapSequence.h
* 
* @class      GRPBITMAPSEQUENCE
* @brief      Graphics Bitmap Sequence class
* @ingroup    GRAPHIC
* 
* @copyright  EndoraSoft. All rights reserved.
* 
* @cond
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* @endcond
* 
* --------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"

#include "GRPBitmap.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum GRPBITMAPSEQUENCESTATUS
{
  GRPBITMAPSEQUENCESTATUS_NONE     = 0,
  GRPBITMAPSEQUENCESTATUS_INIT        ,
  GRPBITMAPSEQUENCESTATUS_PLAY        ,
  GRPBITMAPSEQUENCESTATUS_STOP        ,
  GRPBITMAPSEQUENCESTATUS_PAUSE       ,
  GRPBITMAPSEQUENCESTATUS_END
};

#define GRPBITMAPSEQUENCE_INFINITE      0
#define GRPBITMAPSEQUENCE_FRAMESSECOND  12

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class GRPRECT;
class GRPBITMAP;


class GRPBITMAPFRAME
{
  public:

                                  GRPBITMAPFRAME                ();
    virtual                      ~GRPBITMAPFRAME                ();

    GRPBITMAP*                    GetBitmap                     ();
    int                           GetAjustX                     ();
    int                           GetAjustY                     ();

    void                          SetBitmap                     (GRPBITMAP* bitmap);
    void                          SetAjustX                     (int ajustx);
    void                          SetAjustY                     (int ajusty);

  private:

    void                          Clean                         ();

    GRPBITMAP*                    bitmap;
    int                           ajustx;
    int                           ajusty;
};

class GRPBITMAPSEQUENCE
{
  public:
                                  GRPBITMAPSEQUENCE             ();
    virtual                      ~GRPBITMAPSEQUENCE             ();

    bool                          AddFrame                      (GRPBITMAP* bitmap, int ajustx = 0, int ajusty = 0);
    bool                          DelAllSequence                (bool delbitmaps = true);

    GRPBITMAPFRAME*               GetActualFrame                ();
    int                           GetActualFrameIndex           ();
    GRPBITMAPFRAME*               GetFrame                      (XWORD nframe);
    int                           GetNFrames                    ();

    int                           GetNLoops                     ();
    int                           GetNLoopsMade                 ();

    bool                          SetNLoops                     (int nloops);
    bool                          SetNLoopsMade                 (int nloopsmade);

    bool                          AjustAllSequence              (int ajustx = 0, int ajusty = 0);

    GRPBITMAPFRAME*               Play                          (int nloops = GRPBITMAPSEQUENCE_INFINITE, bool ajusttimeauto = true, bool reverse = false);
    bool                          Stop                          ();
    bool                          Pause                         ();
    bool                          Finished                      ();

    bool                          IsPlaying                     ();
    bool                          IsEndSequence                 ();

    GRPBITMAPFRAME*               Update                        ();

    bool                          AjustTime                     (int framebysec);

    GRPBITMAPSEQUENCE*            Copy                          ();

    bool                          Flip                          (bool horizontal);
    
  private:

    void                          Clean                         ();

    int                           framebysec;
    float                         timebyframe;
    XTIMER*                       timer;

    bool                          repeat;
    GRPBITMAPSEQUENCESTATUS       status;

    int                           frameindex;
    XVECTOR<GRPBITMAPFRAME*>      frames;

    int                           nloops;
    int                           nloopsmade;

    bool                          reverse;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





