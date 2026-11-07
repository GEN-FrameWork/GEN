/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_RadioGroup.cpp
* 
* @class      UI_ELEMENT_RADIOGROUP
* @brief      User Interface Element RadioGroup class
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

#include "UI_Element_RadioGroup.h"
#include "UI_Element_RadioButton.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_RADIOGROUP::UI_ELEMENT_RADIOGROUP()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_RADIOGROUP::UI_ELEMENT_RADIOGROUP()    
{ 
  Clean();   

  SetType(UI_ELEMENT_TYPE_RADIOGROUP);
  GetTypeString()->Set(NL("radiogroup"));

  // Default SCROLLEABLE overflow is AUTO (clips). That made PreSelectElement reject radios as
  // "partially outside the viewport". A radiogroup is a non-scrolling peer list — keep VISIBLE.
  Scroll_SetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_ALL, UI_OVERFLOW_VISIBLE);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_RADIOGROUP::~UI_ELEMENT_RADIOGROUP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_RADIOGROUP::~UI_ELEMENT_RADIOGROUP()    
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_ELEMENT_RADIOGROUP::GetTitle()
* @brief      Get title
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Title string pointer.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_ELEMENT_RADIOGROUP::GetTitle()
{
  return &title;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_RADIOGROUP::SetTitle(XCHAR* title)
* @brief      Set title
* @ingroup    USERINTERFACE
* 
* @param[in]  title : Title text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_RADIOGROUP::SetTitle(XCHAR* title)
{
  this->title = title;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_RADIOGROUP::SetTitle(XSTRING& title)
* @brief      Set title
* @ingroup    USERINTERFACE
* 
* @param[in]  title : Title text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_RADIOGROUP::SetTitle(XSTRING& title)
{
  this->title = title;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_RADIOGROUP::SelectExclusive(UI_ELEMENT_RADIOBUTTON* selected)
* @brief      Select one radiobutton and clear status on peer radiobuttons in this group
* @ingroup    USERINTERFACE
* 
* @param[in]  selected : Radiobutton to select (may be NULL to clear all).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_RADIOGROUP::SelectExclusive(UI_ELEMENT_RADIOBUTTON* selected)
{
  if(!GetComposeElements()) return false;

  for(XDWORD c=0; c<GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* child = GetComposeElements()->Get(c);
      if(!child) continue;
      if(child->GetType() != UI_ELEMENT_TYPE_RADIOBUTTON) continue;

      UI_ELEMENT_RADIOBUTTON* radio = (UI_ELEMENT_RADIOBUTTON*)child;
      radio->SetStatus(radio == selected);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_RADIOBUTTON* UI_ELEMENT_RADIOGROUP::GetSelected()
* @brief      Get the currently selected radiobutton in this group
* @ingroup    USERINTERFACE
* 
* @return     UI_ELEMENT_RADIOBUTTON* : Selected radiobutton; NULL if none.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_RADIOBUTTON* UI_ELEMENT_RADIOGROUP::GetSelected()
{
  if(!GetComposeElements()) return NULL;

  for(XDWORD c=0; c<GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* child = GetComposeElements()->Get(c);
      if(!child) continue;
      if(child->GetType() != UI_ELEMENT_TYPE_RADIOBUTTON) continue;

      UI_ELEMENT_RADIOBUTTON* radio = (UI_ELEMENT_RADIOBUTTON*)child;
      if(radio->GetStatus()) return radio;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_RADIOGROUP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_RADIOGROUP::Clean()
{
  title.Empty();
}
