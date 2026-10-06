/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_TextBox.cpp
* 
* @class      UI_ELEMENT_TEXTBOX
* @brief      User Interface Element Text Box class
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

#include "UI_Element_TextBox.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_TEXTBOX::UI_ELEMENT_TEXTBOX()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_TEXTBOX::UI_ELEMENT_TEXTBOX()    
{ 
  Clean();   

  SetType(UI_ELEMENT_TYPE_TEXTBOX);

  GetTypeString()->Set(_L("textbox"));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_TEXTBOX::~UI_ELEMENT_TEXTBOX()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_TEXTBOX::~UI_ELEMENT_TEXTBOX()    
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_ELEMENT_TEXTBOX::GetSizeFont()
* @brief      Get size font
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_ELEMENT_TEXTBOX::GetSizeFont()
{
  return sizefont;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_TEXTBOX::SetSizeFont(XDWORD sizefont)
* @brief      Set size font
* @ingroup    USERINTERFACE
* 
* @param[in]  sizefont : Sizefont value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_TEXTBOX::SetSizeFont(XDWORD sizefont)
{
  this->sizefont = sizefont;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_ELEMENT_TEXTBOX::GetLineSpacing()
* @brief      Get line spacing
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_ELEMENT_TEXTBOX::GetLineSpacing()
{
  return linespacing;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_ELEMENT_TEXTBOX::GetContentHeight()
* @brief      Gets the total laid-out text height (set by the skin during draw). Used as the vertical scroll extent.
* @ingroup    USERINTERFACE
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT_TEXTBOX::GetContentHeight()
{
  return contentheight;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_TEXTBOX::SetContentHeight(double contentheight)
* @brief      Sets the total laid-out text height (called by the skin after generating the lines).
* @ingroup    USERINTERFACE
* 
* @param[in]  contentheight : total text height in pixels.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_TEXTBOX::SetContentHeight(double contentheight)
{
  this->contentheight = contentheight;
}
  

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_TEXTBOX::SetLineSpacing(XDWORD linespacing)
* @brief      Set line spacing
* @ingroup    USERINTERFACE
* 
* @param[in]  linespacing : Linespacing value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_TEXTBOX::SetLineSpacing(XDWORD linespacing)
{
  this->linespacing = linespacing;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_ELEMENT_TEXTBOX::GetImageSeparation()
* @brief      Get the separation (in pixels) kept between an inline ![IMAGE] graphic and the text around it (all edges).
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_ELEMENT_TEXTBOX::GetImageSeparation()
{
  return imageseparation;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_TEXTBOX::SetImageSeparation(XDWORD imageseparation)
* @brief      Set the separation (in pixels) kept between an inline ![IMAGE] graphic and the text around it (all edges).
* @note       Defaults to UI_ELEMENT_TEXTBOX_DEFAULTIMAGESEPARATION (5 px).
* @ingroup    USERINTERFACE
* 
* @param[in]  imageseparation : gap in pixels.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_TEXTBOX::SetImageSeparation(XDWORD imageseparation)
{
  this->imageseparation = imageseparation;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_TYPE_ALIGN UI_ELEMENT_TEXTBOX::GetTextAlignment()
* @brief      Get text alignment
* @ingroup    USERINTERFACE
* 
* @return     UI_ELEMENT_TYPE_ALIGN : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_TYPE_ALIGN UI_ELEMENT_TEXTBOX::GetTextAlignment()
{
  return textalignment;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_TEXTBOX::SetTextAlignment(UI_ELEMENT_TYPE_ALIGN textalignment)
* @brief      Set text alignment
* @ingroup    USERINTERFACE
* 
* @param[in]  textalignment : Textalignment value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_TEXTBOX::SetTextAlignment(UI_ELEMENT_TYPE_ALIGN textalignment)
{
  this->textalignment = textalignment;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT_TEXTBOX::IsWordClipping()
* @brief      Is word clipping
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_TEXTBOX::IsWordClipping()
{
  return iswordclipping;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_TEXTBOX::SetIsWordClipping(bool iswordclipping)
* @brief      Set is word clipping
* @ingroup    USERINTERFACE
* 
* @param[in]  iswordclipping : Iswordclipping value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_TEXTBOX::SetIsWordClipping(bool iswordclipping)
{
  this->iswordclipping = iswordclipping;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_ELEMENT_TEXTBOX::GetText()
* @brief      Get text
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_ELEMENT_TEXTBOX::GetText()
{
  return &text;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_ELEMENT_TEXTBOX::GetMaskText()
* @brief      Get mask text
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_ELEMENT_TEXTBOX::GetMaskText()
{
  return &masktext;    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_TEXTBOX::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_TEXTBOX::Clean()
{
  sizefont        = 0;
  linespacing     = 0;
  imageseparation = UI_ELEMENT_TEXTBOX_DEFAULTIMAGESEPARATION;
  textalignment   = UI_ELEMENT_TYPE_ALIGN_NOTDETERMINED;
  iswordclipping  = false;
  contentheight   = 0.0f;
}



