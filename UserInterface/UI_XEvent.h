/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_XEvent.h
* 
* @class      UI_XEVENT
* @brief      User Interface eXtended Event class
* @ingroup    USERINTERFACE
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

#include "XEvent.h"
#include "XPath.h"
#include "XSubject.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum UI_XEVENT_TYPE
{
  UI_XEVENT_TYPE_UNKNOWN                    = XEVENT_TYPE_USERINTERFACE   ,

  UI_XEVENT_TYPE_INPUT_CURSOR_MOVE                                        ,
  UI_XEVENT_TYPE_INPUT_CURSOR_MOTION                                      ,
  UI_XEVENT_TYPE_INPUT_SELECCTION                                         ,

  UI_XEVENT_TYPE_OUTPUT_SELECTED                                          ,
  UI_XEVENT_TYPE_OUTPUT_TEXTTOCHANGE                                      ,
  UI_XEVENT_TYPE_OUTPUT_CHANGECONTENTS                                    ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class XPUBLISHER;
class INPCURSORMOTION;
class UI_ELEMENT;
class UI_SKIN;

class UI_XEVENT : public XEVENT
{
  public:
                                UI_XEVENT                       (XSUBJECT* subject, XDWORD type = UI_XEVENT_TYPE_UNKNOWN, XDWORD family = XEVENT_TYPE_USERINTERFACE);
    virtual                    ~UI_XEVENT                       ();
      
    XDWORD                      GetXPos                         ();
    void                        SetXPos                         (XDWORD xpos);

    XDWORD                      GetYPos                         ();
    void                        SetYPos                         (XDWORD ypos);

    INPCURSORMOTION*            GetCursorMotion                 ();
    void                        SetCursorMotion                 (INPCURSORMOTION* cursormotion);                         

    UI_ELEMENT*                 GetElement                      ();
    bool                        SetElement                      (UI_ELEMENT* element);

    UI_SKIN*                    GetSkin                         ();
    bool                        SetSkin                         (UI_SKIN* skin);

    XSTRING*                    GetMaskTextValue                ();      
    XSTRING*                    GetMaskTextResolved             ();      

  private:

    void                        Clean                           ();

    XDWORD                      xpos;
    XDWORD                      ypos;

    INPCURSORMOTION*            cursormotion;

    UI_ELEMENT*                 element;
    UI_SKIN*                    skin;

    XSTRING                     masktextvalue;      
    XSTRING                     masktextresolved;      
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




