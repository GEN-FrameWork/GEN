/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_RadioButton.cpp
* 
* @class      UI_ELEMENT_RADIOBUTTON
* @brief      User Interface Element RadioButton class
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

#include "UI_Element_RadioButton.h"

#include "GRPBitmap.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_RADIOBUTTON::UI_ELEMENT_RADIOBUTTON()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_RADIOBUTTON::UI_ELEMENT_RADIOBUTTON()    
{ 
  Clean();   

  SetType(UI_ELEMENT_TYPE_RADIOBUTTON);
  GetTypeString()->Set(NL("radiobutton"));   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_RADIOBUTTON::~UI_ELEMENT_RADIOBUTTON()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_RADIOBUTTON::~UI_ELEMENT_RADIOBUTTON()    
{   
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_RADIOBUTTON::GetStatus()
* @brief      Get status
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool  UI_ELEMENT_RADIOBUTTON::GetStatus()
{
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_RADIOBUTTON::SetStatus(bool status)
* @brief      Set status
* @ingroup    USERINTERFACE
* 
* @param[in]  status : Status value to read or update.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool  UI_ELEMENT_RADIOBUTTON::SetStatus(bool status)
{
  this->status = status;

  // Keep the composed animation frame in sync immediately (not only on the next Draw_RadioButton),
  // so a programmatic SetStatus (e.g. Color Theme sync) cannot leave a stale glyph.
  UI_ELEMENT_ANIMATION* animation = Get_UIAnimation();
  if(animation) animation->IndexImage_Set(status ? 1 : 0);

  // Drive CSS :selected / :hover cascade when a stylesheet is present (exclusive peer chrome).
  // TimeSelected 0 = latched (same contract as nav :selected); default 100 ms would clear it.
  SetTimeSelected(0);
  SetSelected(status);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_RADIOBUTTON::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_RADIOBUTTON::Clean()
{  
   status = false;
}
