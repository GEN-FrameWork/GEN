/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_Animation.h
* 
* @class      UI_ELEMENT_ANIMATION
* @brief      User Interface Element Animation class
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

#include "XVector.h" 

#include "UI_Element.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum UI_ELEMENT_ANIMATION_STATE
{
  UI_ELEMENT_ANIMATION_STATE_UNKNOWN        = 0 ,
  UI_ELEMENT_ANIMATION_STATE_PLAY               ,
  UI_ELEMENT_ANIMATION_STATE_STOP               ,
  UI_ELEMENT_ANIMATION_STATE_PAUSE              ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;

class UI_ELEMENT_ANIMATION : public UI_ELEMENT
{
  public:
																		  UI_ELEMENT_ANIMATION				();
    virtual													 ~UI_ELEMENT_ANIMATION			  ();

    bool                              Play                        ();
    bool                              Play                        (int millisecdelay);
    bool                              Stop                        ();
    bool                              Pause                       ();

    UI_ELEMENT_ANIMATION_STATE        GetState                    ();

    XTIMER*                           GetXTimerPlay               (); 

    int                               GetMilliSecondsDelay        ();  
    void                              SetMilliSecondsDelay        (int millisecdelay = 35);  

    int                               IndexImage_Get              ();
    bool                              IndexImage_Set              (int indeximage);
    bool                              IndexImage_Add              (int inc = 1);
    bool                              IndexImage_Reset            ();

    XBYTE                             GetAlpha                    (); 
    void                              SetAlpha                    (XBYTE alpha); 

  private:

		void															Clean												();
   
    XTIMER*                           xtimerplay;   
    int                               millisecdelay;    
    UI_ELEMENT_ANIMATION_STATE        state;    
    XDWORD                            indeximage;
    XBYTE                             alpha;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





