/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_Element_Tooltip.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "UI_Element_TextBox.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define UI_ELEMENT_TOOLTIP_DEFAULT_SHOW_DELAY_MS   400

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

/**
* @brief Tooltip panel: inherits TextBox text APIs (sizefont, wordclipping, GetText/GetMaskText) and the same
*        inline markup (`![IMAGE path]`, `![COLOR r,g,b,a]` / `![END COLOR]`). Apps show/hide and position it
*        (not modal). Skin draws framed chrome then the TextBox content path.
*/
class UI_ELEMENT_TOOLTIP : public UI_ELEMENT_TEXTBOX
{
  public:
                                    UI_ELEMENT_TOOLTIP                      ();
    virtual                        ~UI_ELEMENT_TOOLTIP                      ();

    bool                            Show                                    ();
    bool                            Hide                                    ();
    bool                            ShowAt                                  (double x_position, double y_position);

    bool                            SetContent                              (XCHAR* text);
    bool                            SetContent                              (XSTRING& text);

    XDWORD                          GetShowDelayMs                          ();
    void                            SetShowDelayMs                          (XDWORD delay_ms);

  private:

    void                            Clean                                   ();

    XDWORD                          show_delay_ms;
};

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
