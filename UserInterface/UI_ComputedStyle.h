/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_ComputedStyle.h
*
* @class      UI_COMPUTEDSTYLE
* @brief      User Interface : typed accessors layered on top of the neutral UI_STYLE property bag.
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
* SCOPE (Phase 1 -- "estilo calculado tipado", second increment)
*   UI_COMPUTEDSTYLE IS-A UI_STYLE: it inherits the existing (key -> string) bag and every FillFrom*() front-end
*   untouched, and adds typed getters on top (GetLength(), GetColor()) plus alias-aware variants that go through
*   UI_PROPERTYREGISTRY::GetAliased() instead of a caller re-implementing the "try the legacy name, fall back to
*   the CSS name" pattern by hand.
*
*   This is the type GetLayoutElement_Base() now builds and hands back to its per-widget callers (see the new
*   GetLayoutElement_Base(..., UI_COMPUTEDSTYLE&, ...) overload in UI_Manager.h/.cpp), so a per-type builder like
*   GetLayoutElement_Text() can read "sizefont" from the SAME fully-resolved bag (XML attributes < CSS rules <
*   inline style) that GetLayoutElement_Base() already used for "color"/"width"/etc., instead of re-reading the
*   raw XFILEXMLELEMENT* and silently losing any CSS/inline override -- the exact P0 gap the UI/CSS analysis
*   report identified as 50 direct-read call sites across UI_Manager.cpp's per-widget builders.
*
* SCOPE (Phase 3 -- "caja, unidades y posicionamiento", second increment: "pequeños primero")
*   Adds "box-sizing" and min/max clamping as the same kind of typed, self-tested accessor GetLength()/GetColor()
*   already are -- still deliberately NOT wired into UI_Manager.cpp or any builder (that lands with the layout
*   box/engine increments later in Phase 3, once there is an actual containing block to size against):
*
*   - GetBoxSizing() classifies the "box-sizing" keyword into UI_BOXSIZING_CONTENTBOX (the CSS initial value --
*     width/height apply to the content box only) or UI_BOXSIZING_BORDERBOX (width/height include padding and
*     border). Same "false means absent/unrecognized, but 'value' is still left holding a usable default"
*     contract already used by the rest of this class where that makes the caller's life easier.
*
*   - ClampToMinMax() reads an optional "min-*"/"max-*" pair (already-resolved UI_LENGTH, through the SAME
*     UI_LENGTH_CONTEXT the caller resolved its own value with) and clamps a value between them, honouring the
*     CSS rule that MIN always wins over MAX (so an author's own "max-width" smaller than their own "min-width"
*     is not a contradiction the layout has to fail on -- max is applied first, then min, so min has the last
*     word). A bound that is absent, or whose value is a non-numeric keyword ("none", "auto" -- CSS's own way
*     of saying "unconstrained"), is simply not applied, rather than being coerced into 0.
*
*   - ResolveContentSize() is the box-sizing arithmetic itself: given the resolved value an author wrote for
*     width/height and the box-sizing it should be interpreted under, returns the CONTENT-BOX size (what the
*     rest of a box model needs to lay out children) -- pass-through for CONTENT-BOX, minus padding+border for
*     BORDER-BOX, never negative (CSS clamps a box that would go negative to 0, it does not turn it negative).
*     A static, pure-logic method (no bag lookup), grouped here rather than in UI_PROPERTYREGISTRY because it
*     exists to consume the UI_BOXSIZING type this class just introduced.
*
* @author     Abraham J. Velez / EndoraSoft
*
* ---------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "UI_Style.h"
#include "UI_Length.h"
#include "UI_Color.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum UI_BOXSIZING
{
  UI_BOXSIZING_CONTENTBOX = 0,      // CSS initial value: width/height size the content box only
  UI_BOXSIZING_BORDERBOX,           // width/height include padding + border
};


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class UI_COMPUTEDSTYLE : public UI_STYLE
{
  public:
                                    UI_COMPUTEDSTYLE            ();
    virtual                        ~UI_COMPUTEDSTYLE            ();

    bool                            GetAliased                  (XCHAR* primarykey, XCHAR* secondarykey, XSTRING& value);

    bool                            GetLength                   (XCHAR* key, UI_LENGTH& value);
    bool                            GetLength                   (XCHAR* primarykey, XCHAR* secondarykey, UI_LENGTH& value);

    bool                            GetColor                     (XCHAR* key, UI_COLOR& color);
    bool                            GetColor                     (XCHAR* primarykey, XCHAR* secondarykey, UI_COLOR& color);

    bool                            GetBoxSizing                 (UI_BOXSIZING& value);

    double                          ClampToMinMax                (double value, XCHAR* minkey, XCHAR* maxkey, UI_LENGTH_CONTEXT& context);

    static double                   ResolveContentSize           (double specifiedsize, UI_BOXSIZING boxsizing, double paddingandborder);
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
