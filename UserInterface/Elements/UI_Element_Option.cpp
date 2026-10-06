/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_Option.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "UI_Element_Option.h"

#include "GRPBitmap.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_OPTION::UI_ELEMENT_OPTION()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_OPTION::UI_ELEMENT_OPTION()    
{ 
  Clean();   

  SetType(UI_ELEMENT_TYPE_OPTION);
  GetTypeString()->Set(_L("option"));   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_OPTION::~UI_ELEMENT_OPTION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_OPTION::~UI_ELEMENT_OPTION()    
{   
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_ELEMENT_OPTION::GetVisibleLimitType()
* @brief      Get visible limit type
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_ELEMENT_OPTION::GetVisibleLimitType()
{
  return visiblelimittype;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_OPTION::SetVisibleLimitType(XDWORD visiblelimittype)
* @brief      Set visible limit type
* @ingroup    USERINTERFACE
* 
* @param[in]  visiblelimittype : Visiblelimittype value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_OPTION::SetVisibleLimitType(XDWORD visiblelimittype)
{
  this->visiblelimittype = visiblelimittype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE UI_ELEMENT_OPTION::GetAllocationTextType()
* @brief      Get allocation text type
* @ingroup    USERINTERFACE
* 
* @return     UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE UI_ELEMENT_OPTION::GetAllocationTextType()
{
  return allocationtexttype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_OPTION::SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE allocationtexttype)
* @brief      Set allocation text type
* @ingroup    USERINTERFACE
* 
* @param[in]  allocationtexttype : Allocationtexttype value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_OPTION::SetAllocationTextType(UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE allocationtexttype)
{
  this->allocationtexttype = allocationtexttype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_TEXT* UI_ELEMENT_OPTION::Get_UIText()
* @brief      Get UI text
* @ingroup    USERINTERFACE
* 
* @return     UI_ELEMENT_TEXT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_TEXT* UI_ELEMENT_OPTION::Get_UIText()
{ 
  return UItext;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_OPTION::Set_UIText(UI_ELEMENT_TEXT* elementtext)
* @brief      Set UI text
* @ingroup    USERINTERFACE
* 
* @param[in]  elementtext : Elementtext pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_OPTION::Set_UIText(UI_ELEMENT_TEXT* elementtext)
{
  this->UItext = elementtext;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_ANIMATION* UI_ELEMENT_OPTION::Get_UIAnimation()
* @brief      Get UI animation
* @ingroup    USERINTERFACE
* 
* @return     UI_ELEMENT_ANIMATION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_ANIMATION* UI_ELEMENT_OPTION::Get_UIAnimation()
{
  return UIanimation;
}
 	

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_OPTION::Set_UIAnimation(UI_ELEMENT_ANIMATION* elementanimation)
* @brief      Set UI animation
* @ingroup    USERINTERFACE
* 
* @param[in]  elementanimation : Elementanimation pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_OPTION::Set_UIAnimation(UI_ELEMENT_ANIMATION* elementanimation)
{
  this->UIanimation = elementanimation;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_OPTION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_OPTION::Clean()
{  
  visiblelimittype    =  UI_ELEMENT_OPTION_VISIBLE_LIMIT_NONE;
  
  allocationtexttype  = UI_ELEMENT_OPTION_ALLOCATION_TEXT_TYPE_NONE;

  UItext              = NULL;
  UIanimation         = NULL; 
}



