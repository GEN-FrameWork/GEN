/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_Element_Tooltip.cpp
*
* @class      UI_ELEMENT_TOOLTIP
* @brief      Floating tooltip with TextBox-rich text (inline ![IMAGE] / ![COLOR] markup)
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
*---------------------------------------------------------------------------------------------------------------------*/

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "UI_Element_Tooltip.h"
#include "UI_Manager.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_TOOLTIP::UI_ELEMENT_TOOLTIP()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_TOOLTIP::UI_ELEMENT_TOOLTIP()
{
  Clean();

  // Override TextBox type/string — markup APIs stay on the base class.
  SetType(UI_ELEMENT_TYPE_TOOLTIP);
  GetTypeString()->Set(NL("tooltip"));

  // Floating overlay: hidden until the app calls Show / ShowAt.
  SetVisible(false);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_TOOLTIP::~UI_ELEMENT_TOOLTIP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_TOOLTIP::~UI_ELEMENT_TOOLTIP()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_TOOLTIP::Show()
* @brief      Makes the tooltip visible and marks it dirty.
* @ingroup    USERINTERFACE
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_TOOLTIP::Show()
{
  SetVisible(true);
  SetMustReDraw(true);
  GEN_USERINTERFACE.Elements_SetToRedraw(this);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_TOOLTIP::Hide()
* @brief      Hides the tooltip.
* @ingroup    USERINTERFACE
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_TOOLTIP::Hide()
{
  SetVisible(false);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_TOOLTIP::ShowAt(double x_position, double y_position)
* @brief      Positions the tooltip (GEN EdgeBottom Y) and shows it.
* @ingroup    USERINTERFACE
*
* @param[in]  x_position : Left edge in layout/screen coordinates.
* @param[in]  y_position : Bottom edge (EdgeBottom) in layout/screen coordinates.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_TOOLTIP::ShowAt(double x_position, double y_position)
{
  if(GetBoundaryLine())
    {
      GetBoundaryLine()->x = x_position;
      GetBoundaryLine()->y = y_position;
    }

  SetXPosition(x_position);
  SetYPosition(y_position);

  return Show();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_TOOLTIP::SetContent(XCHAR* text)
* @brief      Sets the display text (supports TextBox ![IMAGE]/![COLOR] markup) and dirties the element.
* @ingroup    USERINTERFACE
*
* @param[in]  text : Content string (literal; clears MaskText).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_TOOLTIP::SetContent(XCHAR* text)
{
  GetMaskText()->Empty();
  if(text) GetText()->Set(text);
   else    GetText()->Empty();

  SetMustReDraw(true);
  if(IsVisible()) GEN_USERINTERFACE.Elements_SetToRedraw(this);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_TOOLTIP::SetContent(XSTRING& text)
* @brief      Sets the display text from an XSTRING.
* @ingroup    USERINTERFACE
*
* @param[in]  text : Content string.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_TOOLTIP::SetContent(XSTRING& text)
{
  return SetContent(text.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD UI_ELEMENT_TOOLTIP::GetShowDelayMs()
* @brief      Returns the suggested hover delay before showing (app-driven timers).
* @ingroup    USERINTERFACE
*
* @return     XDWORD : Delay in milliseconds.
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_ELEMENT_TOOLTIP::GetShowDelayMs()
{
  return show_delay_ms;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_TOOLTIP::SetShowDelayMs(XDWORD delay_ms)
* @brief      Sets the suggested hover delay before showing (app-driven timers).
* @ingroup    USERINTERFACE
*
* @param[in]  delay_ms : Delay in milliseconds.
*
*---------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_TOOLTIP::SetShowDelayMs(XDWORD delay_ms)
{
  show_delay_ms = delay_ms;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_TOOLTIP::Clean()
* @brief      Cleans the object internal state
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_TOOLTIP::Clean()
{
  show_delay_ms = UI_ELEMENT_TOOLTIP_DEFAULT_SHOW_DELAY_MS;
}
