/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_Length.h
*
* @class      UI_LENGTH
* @brief      User Interface : a typed CSS-style length value (number / percentage / bare keyword).
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
*   Today UI_MANAGER::GetLayoutElement_Base() sniffs "xpos"/"ypos"/"width"/"height" by hand, four near-identical
*   times, with a chain of XSTRING::Compare() against keywords ("left"/"right"/"center"/"up"/"down"/"max"/"auto")
*   before falling back to ResolvePercentValue() or a plain number. UI_LENGTH packages that classification (is
*   this raw string a number, a "N%" percentage, or some other bare keyword?) and the number/percentage ->
*   pixel resolution into one typed, tested value, so a caller reads the RESULT of parsing once instead of
*   re-deriving it from string comparisons. The KEYWORD case is deliberately left uninterpreted here: which
*   keywords are legal, and what each one means, is still property-specific (xpos's "left" is not width's
*   "auto"), so that mapping stays where it already lives, in GetLayoutElement_Base() itself.
*
* SCOPE (Phase 3 -- "caja, unidades y posicionamiento", first increment: "pequeños primero")
*   Adds the CSS relative/viewport units (em, rem, vw, vh) and a basic calc() expression (+, -, *, /, with
*   parentheses and standard precedence, operands being numbers/percentages/any of the units above) on top of
*   the existing NUMBER/PERCENT/KEYWORD classification, WITHOUT changing any existing type's behaviour: a plain
*   number still resolves via Resolve(double,double&), unaffected. Resolving a unit or a calc() expression needs
*   more context than a single percentage basis (an em needs the element's own font-size, a rem needs the root's,
*   a vw/vh needs the viewport), so a new UI_LENGTH_CONTEXT bundles that and a new Resolve(UI_LENGTH_CONTEXT&,
*   double&) overload consumes it; the OLD Resolve(double,double&) deliberately still only understands
*   NUMBER/PERCENT and returns false for every new type (see its own comment) rather than silently guessing a
*   context. Fase 8 wires UI_LENGTH_CONTEXT into GetLayoutElement_Base() for layouts that own a stylesheet
*   (xpos/ypos/width/height/padding/margin); XML-only layouts keep ResolvePercentValue / ConvertToDouble.
*   RunLayout is unchanged in this increment.
*
*   calc() parsing note: a malformed calc() (mismatched parentheses, a dangling operator, an empty operand...)
*   does NOT make Parse() fail -- consistent with this class's existing "always succeeds except for empty input"
*   contract (see Parse()'s own comment) -- it instead falls back to UI_LENGTH_TYPE_KEYWORD holding the original
*   text verbatim, exactly what would have happened to that same text before calc() support existed.
*
* @author     Abraham J. Velez / EndoraSoft
*
* ---------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum UI_LENGTH_TYPE
{
  UI_LENGTH_TYPE_UNDEFINED = 0,                 // empty / absent source string: Parse() was never given anything usable
  UI_LENGTH_TYPE_NUMBER,                        // plain number, e.g. "42"
  UI_LENGTH_TYPE_PERCENT,                       // "N%", e.g. "50%" (GetValue() returns 50.0, not 0.5)
  UI_LENGTH_TYPE_KEYWORD,                       // anything else non-numeric, e.g. "left", "auto", "max"
  UI_LENGTH_TYPE_EM,                            // "Nem", relative to the element's own font-size
  UI_LENGTH_TYPE_REM,                           // "Nrem", relative to the root element's font-size
  UI_LENGTH_TYPE_VW,                            // "Nvw", relative to 1/100 of the viewport width
  UI_LENGTH_TYPE_VH,                            // "Nvh", relative to 1/100 of the viewport height
  UI_LENGTH_TYPE_CALC,                          // "calc(...)", an arithmetic expression over the types above
};


/**
* @struct  UI_LENGTH_CONTEXT
* @brief   The extra reference values a PERCENT/EM/REM/VW/VH/CALC length needs to resolve to pixels.
*/
struct UI_LENGTH_CONTEXT
{
  double basis;
  double fontsize;
  double rootfontsize;
  double viewportwidth;
  double viewportheight;
};


// AST node for a parsed calc() expression: a private, file-local implementation detail (fully defined next to
// the parsing/evaluation methods that use it, in UI_Length.cpp) -- nothing outside UI_LENGTH itself ever sees
// or names this type, exactly like the ancestor-provider helper classes added for the Phase 2 combinators.
class UI_LENGTHCALCNODE;


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class UI_LENGTH
{
  public:
                                    UI_LENGTH                   ();
    virtual                        ~UI_LENGTH                  ();

    bool                            Parse                       (XSTRING& raw);

    UI_LENGTH_TYPE                  GetType                     ();

    double                          GetValue                    ();

    bool                            IsKeyword                   (XCHAR* keyword);

    bool                            Resolve                     (double basis, double& out);

    bool                            Resolve                     (UI_LENGTH_CONTEXT& context, double& out);

  private:

    void                            Clean                       ();

    UI_LENGTH_TYPE                   type;
    double                           value;
    XSTRING                          keyword;
    UI_LENGTHCALCNODE*               calcroot;

    UI_LENGTHCALCNODE*               ParseCalcExpression         (XSTRING& text, int& pos, int end);
    UI_LENGTHCALCNODE*               ParseCalcTerm               (XSTRING& text, int& pos, int end);
    UI_LENGTHCALCNODE*               ParseCalcFactor             (XSTRING& text, int& pos, int end);
    void                              DeleteCalcNode              (UI_LENGTHCALCNODE* node);
    bool                              EvaluateCalcNode            (UI_LENGTHCALCNODE* node, UI_LENGTH_CONTEXT& context, double& out);
    bool                              MatchesUnitSuffix           (XSTRING& text, int pos, int end, XCHAR* unit);
    void                              SkipCalcWhitespace          (XSTRING& text, int& pos, int end);
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
