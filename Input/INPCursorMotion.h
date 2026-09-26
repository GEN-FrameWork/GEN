/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPCursorMotion.h
* 
* @class      INPCURSORMOTION
* @brief      Input Cursor Motion
* @ingroup    INPUT
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

#include "XBase.h"
#include "XVector.h"
#include "XString.h"
#include "XRect.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum INPCURSORMOTION_DIR
{
  INPCURSORMOTION_DIR_UNKNOWN                = 0 ,

  INPCURSORMOTION_DIR_UP                         ,
  INPCURSORMOTION_DIR_DOWN                       ,
  INPCURSORMOTION_DIR_RIGHT                      ,
  INPCURSORMOTION_DIR_LEFT                       ,

  INPCURSORMOTION_DIR_RIGHTUP                    ,
  INPCURSORMOTION_DIR_LEFTUP                     ,
  INPCURSORMOTION_DIR_RIGHTDOWN                  ,
  INPCURSORMOTION_DIR_LEFTDOWN                   ,
};

enum INPCURSORMOTION_REDUCEDMODE
{
  INPCURSORMOTION_REDUCEDMODE_NONE           = 0 ,
  INPCURSORMOTION_REDUCEDMODE_BYSTEPS            ,
  INPCURSORMOTION_REDUCEDMODE_NMAX               ,
  INPCURSORMOTION_REDUCEDMODE_NMAXPERCENT        ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class INPCURSORMOTIONPOINT
{
  public:
                                      INPCURSORMOTIONPOINT            ();
    virtual                          ~INPCURSORMOTIONPOINT            ();

    virtual float                     GetX                            ();
    virtual float                     GetY                            ();
    virtual float                     GetZ                            ();

    virtual void                      Set                             (float x = 0, float y = 0, float z = 0);
    virtual void                      SetX                            (float x = 0);
    virtual void                      SetY                            (float y = 0);
    virtual void                      SetZ                            (float z = 0);

  protected:

    float                             x;
    float                             y;
    float                             z;

  private:

     void                             Clean                           ();
};

class INPCURSORMOTION
{
  public:
                                      INPCURSORMOTION                 ();
    virtual                          ~INPCURSORMOTION                 ();

    XVECTOR<INPCURSORMOTIONPOINT*>*   Get                             ();

    int                               GetNPoints                      ();

    INPCURSORMOTIONPOINT*             GetFirstPoint                   ();
    INPCURSORMOTIONPOINT*             GetLastPoint                    ();

    bool                              AddPoint                        (float x = 0, float y = 0, float z = 0);
    bool                              DeleteAllPoints                 ();

    bool                              AddFromLine                     (INPCURSORMOTION_REDUCEDMODE reducedmode, int reducedfactor, int x1, int y1, int x2, int y2);
    bool                              AddFromCircle                   (INPCURSORMOTION_REDUCEDMODE reducedmode, int reducedfactor, int x, int y, int radio);
    bool                              AddFromElipse                   (INPCURSORMOTION_REDUCEDMODE reducedmode, int reducedfactor, int x, int y, int rx, int ry);

    float                             GetAngle                        ();
    int                               GetModule                       ();

    bool                              IsInRect                        (int x, int y, int width, int height);

    bool                              IsInCurse                       ();
    bool                              SetIsInCurse                    (bool isincurse);

    bool                              IsReadyToTest                   (XDWORD npoints_min = 0);

    bool                              InvertYAxis                     (XDWORD screen_height);  

    INPCURSORMOTION_DIR               GetDirectionByAngle             (bool indetail);
    INPCURSORMOTION_DIR               GetDirectionByDifferential      (XDWORD* differential = NULL);

    XQWORD                            GetTimeElapsed                  ();
    bool                              SetTimeElapsed                  (XQWORD timeelapsed);

    bool                              Reset                           ();

  protected:

    XVECTOR<INPCURSORMOTIONPOINT*>    points;

  private:

    int                               CalculateReducedModeSteps   (INPCURSORMOTION_REDUCEDMODE mode, int factor, int npixels);
    int                               Round                       (double a);

    void                              Clean                       ();

    bool                              isincurse;
    XQWORD                            timeelapsed;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





