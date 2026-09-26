/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPCursor.h
* 
* @class      INPCURSOR
* @brief      Input Cursor Class
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

#include "INPCursorMotion.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum INPCURSOR_ID
{
  INPCURSOR_ID_NONE             = 0 ,

  INPCURSOR_ID_MOUSE                ,
  INPCURSOR_ID_TOUCHSCREEN1         ,
  INPCURSOR_ID_TOUCHSCREEN2         ,
  INPCURSOR_ID_TOUCHSCREEN3         ,
  INPCURSOR_ID_TOUCHSCREEN4         ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XTIMER;
class INPBUTTON;

class INPCURSOR : public INPCURSORMOTIONPOINT
{
  public:
                            INPCURSOR                 ();
    virtual                ~INPCURSOR                 ();

    INPCURSOR_ID            GetID                     ();
    void                    SetID                     (INPCURSOR_ID ID);

    XTIMER*                 GetTimer                  ();

    bool                    HavePreSelect             ();
    bool                    SetHavePreSelect          (bool havepreselect);

    bool                    IsChanged                 ();
    bool                    SetIsChanged              (bool ischanged);

    bool                    IsPositionInRect          (int x = 0, int y = 0, int width = 0, int height = 0);

    INPCURSORMOTION*        GetMotion                 ();
    bool                    AddPointToMotion          (bool ispressed);

    void                    Set                       (float x = 0, float y = 0, float z = 0);
    void                    SetX                      (float x = 0);
    void                    SetY                      (float y = 0);
    void                    SetZ                      (float z = 0);

  private:

    void                    Clean                     ();

    XTIMER*                 timer;
    INPCURSOR_ID            ID;
    bool                    havepreselect;
    bool                    ischanged;

    XRECT                   source;
    XRECT                   destination;

    INPCURSORMOTION         motion;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





