/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Property_Selectable.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "UI_Property_Selectable.h"

#include "XFactory.h"
#include "XTimer.h"

#include "GRPBitmap.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_PROPERTY_SELECTABLE::UI_PROPERTY_SELECTABLE()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_PROPERTY_SELECTABLE::UI_PROPERTY_SELECTABLE()    
{ 
  Clean(); 

  xtimerselected=GEN_XFACTORY.CreateTimer();
  timeselected   = UI_PROPERTY_SELECTABLE_DEFAULT_TIMESELECTED; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_PROPERTY_SELECTABLE::~UI_PROPERTY_SELECTABLE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_PROPERTY_SELECTABLE::~UI_PROPERTY_SELECTABLE()    
{   
  if(xtimerselected) 
    {
      GEN_XFACTORY.DeleteTimer(xtimerselected);
      xtimerselected = NULL;
    }

  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_PROPERTY_SELECTABLE::GetTimeSelected()
* @brief      Get time selected
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_PROPERTY_SELECTABLE::GetTimeSelected()
{
  return timeselected;
}																		


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SELECTABLE::SetTimeSelected(XDWORD timeselected)
* @brief      Set time selected
* @ingroup    USERINTERFACE
* 
* @param[in]  timeselected : Timeselected value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SELECTABLE::SetTimeSelected(XDWORD timeselected)
{
  this->timeselected = timeselected;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* UI_PROPERTY_SELECTABLE::GetXTimerSelected()
* @brief      Get X timer selected
* @ingroup    USERINTERFACE
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* UI_PROPERTY_SELECTABLE::GetXTimerSelected()
{
  return xtimerselected;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_PROPERTY_SELECTABLE_STATE UI_PROPERTY_SELECTABLE::GetSelectableState()
* @brief      Get selectable state
* @ingroup    USERINTERFACE
* 
* @return     UI_PROPERTY_SELECTABLE_STATE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_PROPERTY_SELECTABLE_STATE UI_PROPERTY_SELECTABLE::GetSelectableState()
{
  return selectable_state;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_PROPERTY_SELECTABLE::SetSelectableState(UI_PROPERTY_SELECTABLE_STATE state)
* @brief      Set selectable state
* @ingroup    USERINTERFACE
* 
* @param[in]  state : State value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTY_SELECTABLE::SetSelectableState(UI_PROPERTY_SELECTABLE_STATE state)
{
  this->selectable_state = state;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_PROPERTY_SELECTABLE_STATE UI_PROPERTY_SELECTABLE::SetSelectableStateFromString(XCHAR* selectablestr)
* @brief      Set selectable state from string
* @ingroup    USERINTERFACE
* 
* @param[in]  selectablestr : Selectablestr pointer to use.
* 
* @return     UI_PROPERTY_SELECTABLE_STATE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_PROPERTY_SELECTABLE_STATE UI_PROPERTY_SELECTABLE::SetSelectableStateFromString(XCHAR* selectablestr)
{ 
  if(!selectablestr)  return UI_PROPERTY_SELECTABLE_STATE_DEACTIVE;

  XSTRING _selectablestr;
  
  selectable_state = UI_PROPERTY_SELECTABLE_STATE_DEACTIVE;
  
  _selectablestr = selectablestr;

  if(!_selectablestr.Compare(_L("active"), true))  selectable_state = UI_PROPERTY_SELECTABLE_STATE_ACTIVE;
    else if(!_selectablestr.Compare(_L("deactive"), true))  selectable_state = UI_PROPERTY_SELECTABLE_STATE_DEACTIVE;
      else if(!_selectablestr.Compare(_L("preselect"), true)) selectable_state = UI_PROPERTY_SELECTABLE_STATE_PRESELECT;
         else if(!_selectablestr.Compare(_L("selected"), true)) selectable_state = UI_PROPERTY_SELECTABLE_STATE_SELECTED;
  
  return selectable_state;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_PROPERTY_SELECTABLE_STATE UI_PROPERTY_SELECTABLE::SetSelectableStateFromString(XSTRING& selectablestr)
* @brief      Set selectable state from string
* @ingroup    USERINTERFACE
* 
* @param[in]  selectablestr : Selectablestr value.
* 
* @return     UI_PROPERTY_SELECTABLE_STATE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_PROPERTY_SELECTABLE_STATE UI_PROPERTY_SELECTABLE::SetSelectableStateFromString(XSTRING& selectablestr)
{
  return SetSelectableStateFromString(selectablestr.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_SELECTABLE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_SELECTABLE::Clean()
{  
  selectable_state = UI_PROPERTY_SELECTABLE_STATE_DEACTIVE;

  xtimerselected   = NULL;		
	timeselected     = 0; 
}



