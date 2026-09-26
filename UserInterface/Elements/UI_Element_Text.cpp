/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_Text.cpp
* 
* @class      UI_ELEMENT_TEXT
* @brief      User Interface Element Text class
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

#include "UI_Element_Text.h"
#include "UI_Style.h"
#include "UI_StyleSheet.h"
#include "UI_PropertyRegistry.h"
#include "UI_Layout.h"
#include "UI_LayoutEngine.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_TEXT::UI_ELEMENT_TEXT()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_TEXT::UI_ELEMENT_TEXT()    
{ 
  Clean();   

  SetType(UI_ELEMENT_TYPE_TEXT);
  GetTypeString()->Set(__L("text"));  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_TEXT::~UI_ELEMENT_TEXT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_TEXT::~UI_ELEMENT_TEXT()    
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_ELEMENT_TEXT::GetSizeFont()
* @brief      Get size font
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_ELEMENT_TEXT::GetSizeFont()
{
  return sizefont;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_TEXT::SetSizeFont(XDWORD sizefont)
* @brief      Set size font
* @ingroup    USERINTERFACE
* 
* @param[in]  sizefont : Sizefont value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_TEXT::SetSizeFont(XDWORD sizefont)
{
  this->sizefont = sizefont;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_ELEMENT_TEXT::GetText()
* @brief      Get text
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_ELEMENT_TEXT::GetText()
{
  return &text;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_ELEMENT_TEXT::GetMaskText()
* @brief      Get mask text
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_ELEMENT_TEXT::GetMaskText()
{
  return &masktext;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_ELEMENT_TEXT::GetMaxSizeText()
* @brief      Get max size text
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_ELEMENT_TEXT::GetMaxSizeText()
{
  return maxsizetext;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_TEXT::SetMaxSizeText(XDWORD maxsizetext)
* @brief      Set max size text
* @ingroup    USERINTERFACE
* 
* @param[in]  maxsizetext : Maxsizetext value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_TEXT::SetMaxSizeText(XDWORD maxsizetext)
{
  this->maxsizetext = maxsizetext;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_TEXT::ReapplyStyleVisual()
* @brief      Reapply Style Visual
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_TEXT::ReapplyStyleVisual()
{
  XDWORD oldsize = sizefont;

  UI_ELEMENT::ReapplyStyleVisual();

  if(!GetStyleHasStateRules()) return;

  UI_LAYOUT* layout = GetLayout();
  UI_STYLESHEET* sheet = layout ? layout->GetStyleSheet() : NULL;
  if(!sheet) return;

  UI_STYLE bag;
  bag.FillFromCSSDeclarations(sheet, this);

  double sf = 0.0;
  XSTRING sfstr;
  if(UI_PROPERTYREGISTRY::GetAliased(bag, __L("sizefont"), __L("font-size"), sfstr) && !sfstr.IsEmpty())
    {
      sf = sfstr.ConvertToDouble();
      if(sf > 0.0) SetSizeFont((XDWORD)sf);
    }

  // Phase 3.3: if sizefont changed inside a flex/grid father, re-run local layout.
  if(sizefont != oldsize)
    {
      UI_ELEMENT* father = GetFather();
      if(father && (father->IsFlexContainer() || father->IsGridContainer()))
        {
          UI_LAYOUTENGINE::RunLayout(father, UI_LAYOUTSTRATEGY_CSS);
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_TEXT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_TEXT::Clean()
{
  sizefont     = 0;
  maxsizetext  = 0;
}










