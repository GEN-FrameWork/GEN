/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Property_Editable.cpp
* 
* @class      UI_PROPERTY_EDITABLE
* @brief      User Interface Property Editable class
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

#include "UI_Property_Editable.h"

#include "XString.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_PROPERTY_EDITABLE::UI_PROPERTY_EDITABLE()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_PROPERTY_EDITABLE::UI_PROPERTY_EDITABLE()    
{ 
  Clean();   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_PROPERTY_EDITABLE::~UI_PROPERTY_EDITABLE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_PROPERTY_EDITABLE::~UI_PROPERTY_EDITABLE()    
{   
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_PROPERTY_EDITABLE::Cursor_GetPosition()
* @brief      Cursor get position
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_PROPERTY_EDITABLE::Cursor_GetPosition()
{
  return cursor_position;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_EDITABLE::Cursor_SetPosition(XDWORD cursor_position)
* @brief      Cursor set position
* @ingroup    USERINTERFACE
* 
* @param[in]  cursor_position : Cursor position value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_EDITABLE::Cursor_SetPosition(XDWORD cursor_position)
{
  this->cursor_position = cursor_position;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_PROPERTY_EDITABLE::GetMaxSize()
* @brief      Get max size
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_PROPERTY_EDITABLE::GetMaxSize()
{
  return maxsize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_EDITABLE::SetMaxSize(XDWORD maxsize)
* @brief      Set max size
* @ingroup    USERINTERFACE
* 
* @param[in]  maxsize : Maxsize value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_EDITABLE::SetMaxSize(XDWORD maxsize)
{
  this->maxsize = maxsize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD UI_PROPERTY_EDITABLE::Selection_GetAnchor()
* @brief      Get selection anchor (fixed end of the range)
* @ingroup    USERINTERFACE
*
* @return     XDWORD : Requested value.
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_PROPERTY_EDITABLE::Selection_GetAnchor()
{
  return sel_anchor;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_PROPERTY_EDITABLE::Selection_SetAnchor(XDWORD anchor)
* @brief      Set selection anchor (fixed end of the range)
* @ingroup    USERINTERFACE
*
* @param[in]  anchor : Anchor character index.
*
*---------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_EDITABLE::Selection_SetAnchor(XDWORD anchor)
{
  sel_anchor = anchor;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_PROPERTY_EDITABLE::Selection_Has()
* @brief      True when a non-empty text selection exists
* @ingroup    USERINTERFACE
*
* @return     bool : true if selection spans at least one character.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTY_EDITABLE::Selection_Has()
{
  return (sel_anchor != cursor_position);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_PROPERTY_EDITABLE::Selection_Clear()
* @brief      Collapse selection to the caret
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_EDITABLE::Selection_Clear()
{
  sel_anchor = cursor_position;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_PROPERTY_EDITABLE::Selection_SelectAll(XDWORD textsize)
* @brief      Select the whole field
* @ingroup    USERINTERFACE
*
* @param[in]  textsize : Current text length.
*
*---------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_EDITABLE::Selection_SelectAll(XDWORD textsize)
{
  sel_anchor      = 0;
  cursor_position = textsize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_PROPERTY_EDITABLE::Selection_GetNormalized(XDWORD& start, XDWORD& end)
* @brief      Get selection as [start, end) with start <= end
* @ingroup    USERINTERFACE
*
* @param[out] start : Inclusive start index.
* @param[out] end : Exclusive end index.
*
*---------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_EDITABLE::Selection_GetNormalized(XDWORD& start, XDWORD& end)
{
  if(sel_anchor <= cursor_position)
    {
      start = sel_anchor;
      end   = cursor_position;
    }
   else
    {
      start = cursor_position;
      end   = sel_anchor;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_PROPERTY_EDITABLE::Selection_GetText(XSTRING* source, XSTRING& out)
* @brief      Copy the selected substring into out
* @ingroup    USERINTERFACE
*
* @param[in]  source : Source text.
* @param[out] out : Selected text (cleared when empty/no selection).
*
* @return     bool : true if a non-empty selection was copied.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTY_EDITABLE::Selection_GetText(XSTRING* source, XSTRING& out)
{
  out.Empty();
  if(!source) return false;
  if(!Selection_Has()) return false;

  XDWORD start = 0;
  XDWORD end   = 0;
  Selection_GetNormalized(start, end);
  if(end > source->GetSize()) end = source->GetSize();
  if(start >= end) return false;

  source->Copy((int)start, (int)end, out);
  return !out.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_PROPERTY_EDITABLE::Selection_Delete(XSTRING* source)
* @brief      Delete the selected range and place the caret at the start
* @ingroup    USERINTERFACE
*
* @param[in]  source : Source text to mutate.
*
* @return     bool : true if characters were deleted.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTY_EDITABLE::Selection_Delete(XSTRING* source)
{
  if(!source) return false;
  if(!Selection_Has()) return false;

  XDWORD start = 0;
  XDWORD end   = 0;
  Selection_GetNormalized(start, end);
  if(end > source->GetSize()) end = source->GetSize();
  if(start >= end) return false;

  source->DeleteCharacters(start, end - start);
  cursor_position = start;
  sel_anchor      = start;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_PROPERTY_EDITABLE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTY_EDITABLE::Clean()
{
  cursor_position   = 0;
  maxsize           = 0;
  sel_anchor        = 0;
}



