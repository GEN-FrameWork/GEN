/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Property_Selectable.h
* 
* @class      UI_PROPERTY_SELECTABLE
* @brief      User Interface Property Selectable class
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

#include "XString.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum UI_PROPERTY_SELECTABLE_STATE
{
  UI_PROPERTY_SELECTABLE_STATE_DEACTIVE							    = 0 ,	
	UI_PROPERTY_SELECTABLE_STATE_ACTIVE						          	,	
	UI_PROPERTY_SELECTABLE_STATE_PRESELECT							  	  ,			
	UI_PROPERTY_SELECTABLE_STATE_SELECTED								      ,			
	
	UI_PROPERTY_SELECTABLE_STATE_MAX
};

#define UI_PROPERTY_SELECTABLE_DEFAULT_TIMESELECTED      100

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;

class UI_PROPERTY_SELECTABLE 
{
  public:
																		          UI_PROPERTY_SELECTABLE		        ();
    virtual											         	   ~UI_PROPERTY_SELECTABLE	  	      ();
     
    XDWORD                                    GetTimeSelected                   ();
    void                                      SetTimeSelected                   (XDWORD timeselected = UI_PROPERTY_SELECTABLE_DEFAULT_TIMESELECTED);

    XTIMER*																    GetXTimerSelected                 ();		

    UI_PROPERTY_SELECTABLE_STATE              GetSelectableState                ();

    bool                                      SetSelectableState                (UI_PROPERTY_SELECTABLE_STATE state);    
    UI_PROPERTY_SELECTABLE_STATE              SetSelectableStateFromString      (XCHAR* selectablestr);
    UI_PROPERTY_SELECTABLE_STATE              SetSelectableStateFromString      (XSTRING& selectablestr);

  protected:

    UI_PROPERTY_SELECTABLE_STATE              selectable_state;
    XTIMER*																    xtimerselected;		
		XDWORD																    timeselected;
      	
  private:

		void															        Clean											        ();      
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




