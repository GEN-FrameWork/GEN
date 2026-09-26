/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_PropertyRegistry.h
*
* @class      UI_PROPERTYREGISTRY
* @brief      User Interface : formal legacy<->CSS property alias table and shared CSS shorthand parsing.
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
* SCOPE (Phase 1 -- "estilo calculado tipado", first increment)
*   Two things that today live as ad-hoc, copy-pasted logic scattered across UI_MANAGER::GetLayoutElement_Base()
*   and UI_ELEMENT::ReapplyStyleVisual() become single, shared, tested entry points here:
*
*   1. The legacy-attribute-name <-> CSS-natural-name alias pattern ("bckgrdcolor"/"background-color",
*      "textalignment"/"text-align"): GetAliased() is the ONE place that decides "first hit wins", so load
*      time and re-style-on-state-change time can never drift apart on which spelling wins a tie.
*
*   2. The CSS 1-to-4-value box shorthand grammar (used today by "padding" and "border-radius", independently
*      re-implemented in each with an identical tokenizer): ExpandCSSShorthand4() is the one implementation,
*      so a third consumer ("margin", see UI_Manager.cpp) does not need a third copy.
*
*   This is deliberately NOT yet the full typed UI_COMPUTEDSTYLE / UI_LENGTH value-object layer described in the
*   UI/CSS analysis report's Phase 1 section -- that is a larger, separately-verified increment. This header is
*   the alias/shorthand groundwork it builds on, extracted now because it was already duplicated and already
*   risked drifting.
*
* @author     Abraham J. Velez / EndoraSoft
*
* ---------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XString.h"

#include "UI_Style.h"
#include "UI_Length.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


/**
* @brief  UI_PROPERTYREGISTRY has no instance state: every member is a static, pure-logic helper, so it can be
*         called from both load-time building (UI_MANAGER::GetLayoutElement_Base()) and re-style-on-state-change
*         (UI_ELEMENT::ReapplyStyleVisual()) without either one owning a copy of the alias table.
*/
class UI_PROPERTYREGISTRY
{
  public:

    static bool                    GetAliased                  (UI_STYLE& style, XCHAR* primarykey, XCHAR* secondarykey, XSTRING& value);

    static XDWORD                  TokenizeNumbers              (XSTRING& raw, double* outvalues, XDWORD maxvalues);

    static XDWORD                  TokenizeTokens               (XSTRING& raw, XSTRING* outtokens, XDWORD maxvalues);

    static void                    ExpandCSSShorthand4          (XSTRING& raw, double out[4]);

    static bool                    ResolveLengthToken           (XSTRING& raw, UI_LENGTH_CONTEXT& context, double& out);

    static bool                    ExpandCSSShorthand4Lengths   (XSTRING& raw, UI_LENGTH_CONTEXT& context, double out[4]);

    static bool                    ParseBoxShadow               (XSTRING& raw, double& outoffsetx, double& outoffsety, double& outblur, XSTRING& outcolor);

    static bool                    ResolveMarginEdges           (UI_STYLE& style, bool use_css_trbl, bool apply_longhands, double out_lrud[4], UI_LENGTH_CONTEXT* lengthctx = NULL);
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
