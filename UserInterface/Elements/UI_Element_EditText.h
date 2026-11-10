/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_EditText.h
* 
* @class      UI_ELEMENT_EDITTEXT
* @brief      User Interface Element Edit Text class
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

#include "UI_Element.h"
#include "UI_Element_Text.h"
#include "UI_Property_Selectable.h"
#include "UI_Property_Editable.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define UI_ELEMENT_EDITTEXT_DEFAULTCUSORTIMEBLINK  500
#define UI_ELEMENT_EDITTEXT_SPIN_WIDTH             26.0

enum UI_ELEMENT_EDITTEXT_INPUTMODE
{
  UI_ELEMENT_EDITTEXT_INPUTMODE_TEXT     = 0 ,
  UI_ELEMENT_EDITTEXT_INPUTMODE_NUMERIC      ,
  UI_ELEMENT_EDITTEXT_INPUTMODE_PASSWORD     ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;


class UI_ELEMENT_EDITTEXT : public UI_ELEMENT_TEXT, public UI_PROPERTY_SELECTABLE, public UI_PROPERTY_EDITABLE
{
  public:
																		  UI_ELEMENT_EDITTEXT				  ();
    virtual													 ~UI_ELEMENT_EDITTEXT				  ();

    bool                              Cursor_IsVisible            ();  
    bool                              Cursor_SetVisible           (bool on);  
    XTIMER*                           Cursor_GetXTimerBlink       ();
    XDWORD                            Cursor_GetTimeBlink         ();
    void                              Cursor_SetTimeBlink         (XDWORD timeblink);
    bool                              Cursor_GetStateBlink        ();
    void                              Cursor_ChangeStateBlink     ();

    UI_ELEMENT_EDITTEXT_INPUTMODE     GetInputMode                ();
    void                              SetInputMode                (UI_ELEMENT_EDITTEXT_INPUTMODE mode);
    bool                              SetInputModeFromString      (XCHAR* modestring);

    int                               GetValueMin                 ();
    void                              SetValueMin                 (int valuemin);
    int                               GetValueMax                 ();
    void                              SetValueMax                 (int valuemax);
    int                               GetValueStep                ();
    void                              SetValueStep                (int valuestep);

    bool                              IsCharAllowed               (XCHAR character);
    bool                              SanitizeTextToMode          ();
    int                               GetNumericValue             ();
    bool                              SetNumericValue             (int value);
    bool                              StepNumericValue            (int direction);
    bool                              BuildDisplayText            (XSTRING& display);

    double                            GetSpinWidth                ();
    // Returns 1 = up (+), -1 = down (-), 0 = not in spin zone. Coords are absolute design space.
    int                               HitTestSpin                 (int design_x, int design_y);

    
  private:

		void															Clean												();
    
    bool                              cursor_isvisible;
    XTIMER*                           cursor_xtimerblink;
    XDWORD                            cursor_timeblink;  
    bool                              cursor_stateblink;

    UI_ELEMENT_EDITTEXT_INPUTMODE     inputmode;
    int                               valuemin;
    int                               valuemax;
    int                               valuestep;
          
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
