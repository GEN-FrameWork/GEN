/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_Option.h
* 
* @class      UI_ELEMENT_OPTION
* @brief      User Interface Element Option class
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
#include "UI_Property_Selectable.h"
#include "UI_Property_Editable.h"
#include "UI_Element_Text.h"
#include "UI_Element_Animation.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE
{
  UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_NONE     = 0 ,
  UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_DOWN         ,
  UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_UP           ,
  UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_RIGHT        ,
  UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_LEFT         ,
  UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_CENTER       ,
};

#define UI_ELEMENT_OPTION_VISIBLE_LIMIT_NONE          0x0000
#define UI_ELEMENT_OPTION_VISIBLE_LIMIT_ACTIVE        0x0001
#define UI_ELEMENT_OPTION_VISIBLE_LIMIT_DEACTIVE      0x0002
#define UI_ELEMENT_OPTION_VISIBLE_LIMIT_PRESELECT     0x0004
#define UI_ELEMENT_OPTION_VISIBLE_LIMIT_SELECT        0x0008

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class UI_ELEMENT_OPTION : public UI_ELEMENT, public UI_PROPERTY_SELECTABLE
{
  public:
																		              UI_ELEMENT_OPTION					  ();
    virtual											    	        	 ~UI_ELEMENT_OPTION					  ();

    XDWORD                                        GetVisibleLimitType         ();
    void                                          SetVisibleLimitType         (XDWORD visiblelimittype);

    UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE        GetAllocationTextType       ();
    void                                          SetAllocationTextType       (UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE allocationtexttype);

    UI_ELEMENT_TEXT*                              Get_UIText                  ();
    bool                                          Set_UIText                  (UI_ELEMENT_TEXT* elementtext);

    UI_ELEMENT_ANIMATION*                         Get_UIAnimation             ();
    bool                                          Set_UIAnimation             (UI_ELEMENT_ANIMATION* elementanimation);
    
  private:

		void															            Clean												();

    XDWORD                                        visiblelimittype;  

    UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE        allocationtexttype;

    UI_ELEMENT_TEXT*                              UItext;
    UI_ELEMENT_ANIMATION*                         UIanimation;   
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




