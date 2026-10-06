/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_ProgressImage.cpp
* 
* @class      UI_ELEMENT_PROGRESS_IMAGE
* @brief      User Interface Element Progress Image class (two-graphic clip-reveal progress indicator)
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

#include "UI_Element_ProgressImage.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_PROGRESS_IMAGE::UI_ELEMENT_PROGRESS_IMAGE()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_PROGRESS_IMAGE::UI_ELEMENT_PROGRESS_IMAGE()
{
  Clean();

  SetType(UI_ELEMENT_TYPE_PROGRESSIMAGE);
  GetTypeString()->Set(_L("progressimage"));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_PROGRESS_IMAGE::~UI_ELEMENT_PROGRESS_IMAGE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_PROGRESS_IMAGE::~UI_ELEMENT_PROGRESS_IMAGE()
{
  // imageempty / imagefull are owned by the animation/bitmap cache, not by this element: do not delete.

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* UI_ELEMENT_PROGRESS_IMAGE::GetImageEmpty()
* @brief      Get the empty (0%) graphic
* @ingroup    USERINTERFACE
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* UI_ELEMENT_PROGRESS_IMAGE::GetImageEmpty()
{
  return imageempty;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_PROGRESS_IMAGE::SetImageEmpty(GRPBITMAP* image)
* @brief      Set the empty (0%) graphic (pointer is cache-owned, not copied nor deleted by this element)
* @ingroup    USERINTERFACE
* 
* @param[in]  image : the empty graphic
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_PROGRESS_IMAGE::SetImageEmpty(GRPBITMAP* image)
{
  this->imageempty = image;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPBITMAP* UI_ELEMENT_PROGRESS_IMAGE::GetImageFull()
* @brief      Get the full (100%) graphic
* @ingroup    USERINTERFACE
* 
* @return     GRPBITMAP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPBITMAP* UI_ELEMENT_PROGRESS_IMAGE::GetImageFull()
{
  return imagefull;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_PROGRESS_IMAGE::SetImageFull(GRPBITMAP* image)
* @brief      Set the full (100%) graphic (pointer is cache-owned, not copied nor deleted by this element)
* @ingroup    USERINTERFACE
* 
* @param[in]  image : the full graphic
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_PROGRESS_IMAGE::SetImageFull(GRPBITMAP* image)
{
  this->imagefull = image;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_ELEMENT_PROGRESS_IMAGE::GetOffsetStart()
* @brief      get offset start
* @ingroup    USERINTERFACE
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT_PROGRESS_IMAGE::GetOffsetStart()
{ 
  return offsetstart; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_PROGRESS_IMAGE::SetOffsetStart(double offsetstart)
* @brief      set offset start
* @ingroup    USERINTERFACE
* 
* @param[in]  offsetstart : Offsetstart value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_PROGRESS_IMAGE::SetOffsetStart(double offsetstart)  
{ 
  if(offsetstart < 0.0) offsetstart = 0.0; 
  this->offsetstart = offsetstart; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_ELEMENT_PROGRESS_IMAGE::GetOffsetEnd()
* @brief      get offset end
* @ingroup    USERINTERFACE
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT_PROGRESS_IMAGE::GetOffsetEnd()
{ 
  return offsetend; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_PROGRESS_IMAGE::SetOffsetEnd(double offsetend)
* @brief      set offset end
* @ingroup    USERINTERFACE
* 
* @param[in]  offsetend : Offsetend value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_PROGRESS_IMAGE::SetOffsetEnd(double offsetend)
{ 
  if(offsetend < 0.0) offsetend = 0.0; this->offsetend = offsetend; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float UI_ELEMENT_PROGRESS_IMAGE::GetLevel()
* @brief      Get level
* @ingroup    USERINTERFACE
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float UI_ELEMENT_PROGRESS_IMAGE::GetLevel()
{
  return level;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_PROGRESS_IMAGE::SetLevel(float level)
* @brief      Set level (clamped to [0 .. 100])
* @ingroup    USERINTERFACE
* 
* @param[in]  level : new level
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_PROGRESS_IMAGE::SetLevel(float level)
{
  if(level < 0)   level = 0;
  if(level > 100) level = 100;

  this->level = level;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE UI_ELEMENT_PROGRESS_IMAGE::GetAlpha()
* @brief      Get alpha (0 .. 100)
* @ingroup    USERINTERFACE
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE UI_ELEMENT_PROGRESS_IMAGE::GetAlpha()
{
  return alpha;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_PROGRESS_IMAGE::SetAlpha(XBYTE alpha)
* @brief      Set alpha (0 .. 100)
* @ingroup    USERINTERFACE
* 
* @param[in]  alpha : alpha
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_PROGRESS_IMAGE::SetAlpha(XBYTE alpha)
{
  if(alpha > 100) alpha = 100;

  this->alpha = alpha;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT_PROGRESS_IMAGE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_PROGRESS_IMAGE::Clean()
{
  imageempty  = NULL;
  imagefull   = NULL;
  offsetstart = 0.0;
  offsetend   = 0.0;
  level       = 0.0f;
  alpha       = 100;
}
