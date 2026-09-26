/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_Length.cpp
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
* --------------------------------------------------------------------------------------------------------------------*/

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "UI_Length.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @class      UI_LENGTHCALCNODE
* @brief      User Interface : one node of a parsed calc() expression tree.
* @ingroup    USERINTERFACE
*
* SINGLE PURPOSE
*   A private, file-local AST node: either a leaf operand (a NUMBER/PERCENT/EM/REM/VW/VH magnitude, exactly what
*   UI_LENGTH itself would hold for that unit) or an operator node ('+'/'-'/'*'/'/') owning its two operands.
*   Nothing outside UI_Length.cpp ever names this type -- UI_Length.h only forward-declares it so UI_LENGTH can
*   hold an owned pointer to a tree root.
*
* --------------------------------------------------------------------------------------------------------------------*/
class UI_LENGTHCALCNODE
{
  public:
    UI_LENGTHCALCNODE()
    {
      isoperator = false;
      op         = 0;
      left       = NULL;
      right      = NULL;
      leaftype   = UI_LENGTH_TYPE_UNDEFINED;
      leafvalue  = 0.0;
    }

    bool                 isoperator;     // true: this is an operator node (op/left/right); false: a leaf (leaftype/leafvalue)
    XCHAR                 op;             // '+', '-', '*' or '/' -- meaningful only when isoperator
    UI_LENGTHCALCNODE*    left;           // owned -- meaningful only when isoperator
    UI_LENGTHCALCNODE*    right;          // owned -- meaningful only when isoperator
    UI_LENGTH_TYPE         leaftype;       // NUMBER / PERCENT / EM / REM / VW / VH -- meaningful only when !isoperator
    double                 leafvalue;      // the number before the unit -- meaningful only when !isoperator
};


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LENGTH::UI_LENGTH()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LENGTH::UI_LENGTH()
{
  calcroot = NULL;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LENGTH::~UI_LENGTH()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LENGTH::~UI_LENGTH()
{
  DeleteCalcNode(calcroot);
  calcroot = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_LENGTH::Parse(XSTRING& raw)
* @brief      Parse
* @ingroup    USERINTERFACE
*
* @param[in]  raw : Text to use.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LENGTH::Parse(XSTRING& raw)
{
  Clean();

  if(raw.IsEmpty()) return false;

  int rawsize = (int)raw.GetSize();
  int start   = 0;
  int end     = rawsize;

  while(start < end && (raw[start]==__C(' ')||raw[start]==__C('\t')||raw[start]==__C('\r')||raw[start]==__C('\n'))) start++;
  while(end > start && (raw[end-1]==__C(' ')||raw[end-1]==__C('\t')||raw[end-1]==__C('\r')||raw[end-1]==__C('\n'))) end--;

  if(start >= end) return false;                 // whitespace-only string: treat like empty

  // calc(...) : the WHOLE trimmed value must be one calc() wrapper (real CSS syntax; a unit suffix after the
  // closing paren, e.g. "calc(1px + 1px)px", is not calc() at all and falls through to the KEYWORD branch below).
  if((end - start) > 5 && raw[end-1]==__C(')') &&
     (raw[start+0]==__C('c')||raw[start+0]==__C('C')) && (raw[start+1]==__C('a')||raw[start+1]==__C('A')) &&
     (raw[start+2]==__C('l')||raw[start+2]==__C('L')) && (raw[start+3]==__C('c')||raw[start+3]==__C('C')) &&
     raw[start+4]==__C('('))
    {
      int pos = start + 5;
      int innerend = end - 1;

      UI_LENGTHCALCNODE* root = ParseCalcExpression(raw, pos, innerend);

      SkipCalcWhitespace(raw, pos, innerend);

      if(root && pos == innerend)
        {
          type    = UI_LENGTH_TYPE_CALC;
          calcroot = root;

          return true;
        }

      DeleteCalcNode(root);
      // fall through to KEYWORD below -- a malformed calc() is not an error, see class banner
    }

  XCHAR first = raw[start];

  bool starts_numeric = (first == __C('-')) || (first == __C('+')) || (first == __C('.')) ||
                         ((first >= __C('0')) && (first <= __C('9')));

  if(!starts_numeric)
    {
      type    = UI_LENGTH_TYPE_KEYWORD;
      keyword = raw;

      return true;
    }

  UI_LENGTH_TYPE unittype  = UI_LENGTH_TYPE_NUMBER;
  int             numericend = end;

  if(MatchesUnitSuffix(raw, end-3, end, __L("rem")))      { unittype = UI_LENGTH_TYPE_REM;     numericend = end-3; }
   else if(MatchesUnitSuffix(raw, end-2, end, __L("em"))) { unittype = UI_LENGTH_TYPE_EM;      numericend = end-2; }
   else if(MatchesUnitSuffix(raw, end-2, end, __L("vw"))) { unittype = UI_LENGTH_TYPE_VW;      numericend = end-2; }
   else if(MatchesUnitSuffix(raw, end-2, end, __L("vh"))) { unittype = UI_LENGTH_TYPE_VH;      numericend = end-2; }
   else if(raw[end-1] == __C('%'))                        { unittype = UI_LENGTH_TYPE_PERCENT; numericend = end-1; }

  XSTRING numeric;
  raw.Copy(start, numericend, numeric);

  type  = unittype;
  value = numeric.ConvertToDouble();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_LENGTH::MatchesUnitSuffix(XSTRING& text, int pos, int end, XCHAR* unit)
* @brief      Case-insensitive check for "unit" starting exactly at "pos" and ending exactly at "end", with the
*             character right after it (if any, i.e. still inside the enclosing [., end) window when this is
*             used from inside calc()) not itself alphabetic, so e.g. "1em" is not mistaken for the start of
*             a longer identifier.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]  text : Text to use.
* @param[in]  pos : Index where "unit" must start (may be negative or past "end" -- always returns false, never crashes).
* @param[in]  end : One past the last index "unit" may occupy.
* @param[in]  unit : Lowercase unit text to match, e.g. L"em".
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LENGTH::MatchesUnitSuffix(XSTRING& text, int pos, int end, XCHAR* unit)
{
  if(pos < 0) return false;

  int unitlen = (int)XSTRING::GetSize(unit);
  if((pos + unitlen) > end) return false;

  for(int c=0; c<unitlen; c++)
    {
      XCHAR a = text[pos+c];
      XCHAR b = unit[c];

      if(a>=__C('A') && a<=__C('Z')) a = (XCHAR)(a - __C('A') + __C('a'));
      if(b>=__C('A') && b<=__C('Z')) b = (XCHAR)(b - __C('A') + __C('a'));

      if(a != b) return false;
    }

  int after = pos + unitlen;
  if(after < end)
    {
      XCHAR next = text[after];
      bool  isalpha = (next>=__C('a') && next<=__C('z')) || (next>=__C('A') && next<=__C('Z'));

      if(isalpha) return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LENGTH::SkipCalcWhitespace(XSTRING& text, int& pos, int end)
* @brief      Advances "pos" past any run of space/tab/CR/LF, never past "end".
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]  text : Text to use.
* @param[in,out] pos : Index to advance.
* @param[in]  end : One past the last valid index.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LENGTH::SkipCalcWhitespace(XSTRING& text, int& pos, int end)
{
  while(pos < end && (text[pos]==__C(' ')||text[pos]==__C('\t')||text[pos]==__C('\r')||text[pos]==__C('\n'))) pos++;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LENGTHCALCNODE* UI_LENGTH::ParseCalcExpression(XSTRING& text, int& pos, int end)
* @brief      Expression := Term (('+'|'-') Term)*
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]  text : Text to use.
* @param[in,out] pos : Cursor into "text"; advanced past whatever this call consumes.
* @param[in]  end : One past the last index available to this expression (the calc(...)'s own closing paren,
*             or an enclosing '(' ... ')' pair's closing paren).
*
* @return     UI_LENGTHCALCNODE* : Owned tree root, or NULL if "text" from "pos" is not a valid expression
*             (pos is left indeterminate on failure -- callers must abandon parsing, not retry from it).
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LENGTHCALCNODE* UI_LENGTH::ParseCalcExpression(XSTRING& text, int& pos, int end)
{
  UI_LENGTHCALCNODE* left = ParseCalcTerm(text, pos, end);
  if(!left) return NULL;

  for( ; ; )
    {
      SkipCalcWhitespace(text, pos, end);
      if(pos >= end) break;

      XCHAR op = text[pos];
      if(op != __C('+') && op != __C('-')) break;

      pos++;

      UI_LENGTHCALCNODE* right = ParseCalcTerm(text, pos, end);
      if(!right) { DeleteCalcNode(left); return NULL; }

      UI_LENGTHCALCNODE* node = GEN_NEW UI_LENGTHCALCNODE();
      node->isoperator = true;
      node->op         = op;
      node->left       = left;
      node->right      = right;

      left = node;
    }

  return left;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LENGTHCALCNODE* UI_LENGTH::ParseCalcTerm(XSTRING& text, int& pos, int end)
* @brief      Term := Factor (('*'|'/') Factor)*
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]  text : Text to use.
* @param[in,out] pos : Cursor into "text"; advanced past whatever this call consumes.
* @param[in]  end : One past the last index available.
*
* @return     UI_LENGTHCALCNODE* : Owned tree root, or NULL on failure (see ParseCalcExpression()).
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LENGTHCALCNODE* UI_LENGTH::ParseCalcTerm(XSTRING& text, int& pos, int end)
{
  UI_LENGTHCALCNODE* left = ParseCalcFactor(text, pos, end);
  if(!left) return NULL;

  for( ; ; )
    {
      SkipCalcWhitespace(text, pos, end);
      if(pos >= end) break;

      XCHAR op = text[pos];
      if(op != __C('*') && op != __C('/')) break;

      pos++;

      UI_LENGTHCALCNODE* right = ParseCalcFactor(text, pos, end);
      if(!right) { DeleteCalcNode(left); return NULL; }

      UI_LENGTHCALCNODE* node = GEN_NEW UI_LENGTHCALCNODE();
      node->isoperator = true;
      node->op         = op;
      node->left       = left;
      node->right      = right;

      left = node;
    }

  return left;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LENGTHCALCNODE* UI_LENGTH::ParseCalcFactor(XSTRING& text, int& pos, int end)
* @brief      Factor := '(' Expression ')' | number-with-optional-unit
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]  text : Text to use.
* @param[in,out] pos : Cursor into "text"; advanced past whatever this call consumes.
* @param[in]  end : One past the last index available.
*
* @return     UI_LENGTHCALCNODE* : Owned leaf or subtree, or NULL on failure (see ParseCalcExpression()).
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LENGTHCALCNODE* UI_LENGTH::ParseCalcFactor(XSTRING& text, int& pos, int end)
{
  SkipCalcWhitespace(text, pos, end);
  if(pos >= end) return NULL;

  if(text[pos] == __C('('))
    {
      pos++;

      UI_LENGTHCALCNODE* inner = ParseCalcExpression(text, pos, end);

      SkipCalcWhitespace(text, pos, end);

      if(!inner || pos >= end || text[pos] != __C(')')) { DeleteCalcNode(inner); return NULL; }

      pos++;

      return inner;
    }

  int leafstart = pos;

  if(pos < end && (text[pos]==__C('-') || text[pos]==__C('+'))) pos++;

  bool hasdigits = false;
  while(pos < end && ((text[pos]>=__C('0') && text[pos]<=__C('9')) || text[pos]==__C('.')))
    {
      hasdigits = true;
      pos++;
    }

  if(!hasdigits) { pos = leafstart; return NULL; }

  int numericend = pos;

  UI_LENGTH_TYPE leaftype = UI_LENGTH_TYPE_NUMBER;

  if(MatchesUnitSuffix(text, pos, end, __L("rem")))      { leaftype = UI_LENGTH_TYPE_REM;     pos += 3; }
   else if(MatchesUnitSuffix(text, pos, end, __L("em"))) { leaftype = UI_LENGTH_TYPE_EM;      pos += 2; }
   else if(MatchesUnitSuffix(text, pos, end, __L("vw"))) { leaftype = UI_LENGTH_TYPE_VW;      pos += 2; }
   else if(MatchesUnitSuffix(text, pos, end, __L("vh"))) { leaftype = UI_LENGTH_TYPE_VH;      pos += 2; }
   else if(pos < end && text[pos]==__C('%'))             { leaftype = UI_LENGTH_TYPE_PERCENT; pos += 1; }

  XSTRING numeric;
  text.Copy(leafstart, numericend, numeric);

  UI_LENGTHCALCNODE* leaf = GEN_NEW UI_LENGTHCALCNODE();
  leaf->isoperator = false;
  leaf->leaftype   = leaftype;
  leaf->leafvalue  = numeric.ConvertToDouble();

  return leaf;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LENGTH::DeleteCalcNode(UI_LENGTHCALCNODE* node)
* @brief      Recursively frees "node" and, if it is an operator node, both of its owned operands.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]  node : Tree (sub)root to free; NULL is a no-op.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LENGTH::DeleteCalcNode(UI_LENGTHCALCNODE* node)
{
  if(!node) return;

  if(node->isoperator)
    {
      DeleteCalcNode(node->left);
      DeleteCalcNode(node->right);
    }

  GEN_DELETE node;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_LENGTH::EvaluateCalcNode(UI_LENGTHCALCNODE* node, UI_LENGTH_CONTEXT& context, double& out)
* @brief      Recursively resolves "node" (and, for an operator node, both operands) into a pixel value.
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* @param[in]  node : Tree (sub)root to evaluate.
* @param[in]  context : Reference values (basis/fontsize/rootfontsize/viewport) for PERCENT/EM/REM/VW/VH leaves.
* @param[out] out : Receives the resolved value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LENGTH::EvaluateCalcNode(UI_LENGTHCALCNODE* node, UI_LENGTH_CONTEXT& context, double& out)
{
  if(!node) return false;

  if(!node->isoperator)
    {
      switch(node->leaftype)
        {
          case UI_LENGTH_TYPE_NUMBER  : out = node->leafvalue;                                  return true;
          case UI_LENGTH_TYPE_PERCENT : out = (node->leafvalue / 100.0) * context.basis;         return true;
          case UI_LENGTH_TYPE_EM      : out = node->leafvalue * context.fontsize;                return true;
          case UI_LENGTH_TYPE_REM     : out = node->leafvalue * context.rootfontsize;            return true;
          case UI_LENGTH_TYPE_VW      : out = (node->leafvalue / 100.0) * context.viewportwidth; return true;
          case UI_LENGTH_TYPE_VH      : out = (node->leafvalue / 100.0) * context.viewportheight; return true;
          default                     :                                                           return false;
        }
    }

  double leftvalue  = 0.0;
  double rightvalue = 0.0;

  if(!EvaluateCalcNode(node->left, context, leftvalue))  return false;
  if(!EvaluateCalcNode(node->right, context, rightvalue)) return false;

  switch(node->op)
    {
      case __C('+') : out = leftvalue + rightvalue;                                 return true;
      case __C('-') : out = leftvalue - rightvalue;                                 return true;
      case __C('*') : out = leftvalue * rightvalue;                                 return true;
      case __C('/') : if(rightvalue == 0.0) return false; out = leftvalue / rightvalue; return true;
      default       :                                                               return false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LENGTH_TYPE UI_LENGTH::GetType()
* @brief      Get type
* @ingroup    USERINTERFACE
*
* @return     UI_LENGTH_TYPE : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LENGTH_TYPE UI_LENGTH::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_LENGTH::GetValue()
* @brief      Get value
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LENGTH::GetValue()
{
  return value;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_LENGTH::IsKeyword(XCHAR* keyword)
* @brief      Is keyword
* @ingroup    USERINTERFACE
*
* @param[in]  keyword : Keyword value.
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LENGTH::IsKeyword(XCHAR* keyword)
{
  if(type != UI_LENGTH_TYPE_KEYWORD) return false;

  return !this->keyword.Compare(keyword, true);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_LENGTH::Resolve(double basis, double& out)
* @brief      Resolve
* @ingroup    USERINTERFACE
*
* @param[in]  basis : Basis value used to resolve a PERCENT length; ignored for NUMBER.
* @param[out] out : Receives the resolved value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LENGTH::Resolve(double basis, double& out)
{
  switch(type)
    {
      case UI_LENGTH_TYPE_NUMBER  : out = value;               return true;
      case UI_LENGTH_TYPE_PERCENT : out = (value / 100.0) * basis; return true;
      default                     :                            return false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_LENGTH::Resolve(UI_LENGTH_CONTEXT& context, double& out)
* @brief      Resolve
* @ingroup    USERINTERFACE
*
* @param[in]  context : Reference values (basis/fontsize/rootfontsize/viewport) for PERCENT/EM/REM/VW/VH/CALC.
* @param[out] out : Receives the resolved value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LENGTH::Resolve(UI_LENGTH_CONTEXT& context, double& out)
{
  switch(type)
    {
      case UI_LENGTH_TYPE_NUMBER  : out = value;                                          return true;
      case UI_LENGTH_TYPE_PERCENT : out = (value / 100.0) * context.basis;                 return true;
      case UI_LENGTH_TYPE_EM      : out = value * context.fontsize;                        return true;
      case UI_LENGTH_TYPE_REM     : out = value * context.rootfontsize;                    return true;
      case UI_LENGTH_TYPE_VW      : out = (value / 100.0) * context.viewportwidth;         return true;
      case UI_LENGTH_TYPE_VH      : out = (value / 100.0) * context.viewportheight;        return true;
      case UI_LENGTH_TYPE_CALC    : return EvaluateCalcNode(calcroot, context, out);
      default                     :                                                        return false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LENGTH::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LENGTH::Clean()
{
  DeleteCalcNode(calcroot);
  calcroot = NULL;

  type  = UI_LENGTH_TYPE_UNDEFINED;
  value = 0.0;

  keyword.Empty();
}
