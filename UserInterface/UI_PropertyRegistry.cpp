/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_PropertyRegistry.cpp
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
* --------------------------------------------------------------------------------------------------------------------*/

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "UI_PropertyRegistry.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_PROPERTYREGISTRY::GetAliased(UI_STYLE& style, XCHAR* primarykey, XCHAR* secondarykey, XSTRING& value)
* @brief      Get aliased
* @ingroup    USERINTERFACE
*
* @param[in]  style : Style bag to read from.
* @param[in]  primarykey : Historical GEN attribute name, tried first.
* @param[in]  secondarykey : CSS-natural alias, tried only if primarykey was absent/empty.
* @param[out] value : Receives the winning value.
*
* @return     bool : true if either key resolved to a non-empty value; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTYREGISTRY::GetAliased(UI_STYLE& style, XCHAR* primarykey, XCHAR* secondarykey, XSTRING& value)
{
  if(!style.Get(primarykey, value) || value.IsEmpty())
    {
      style.Get(secondarykey, value);
    }

  return !value.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD UI_PROPERTYREGISTRY::TokenizeNumbers(XSTRING& raw, double* outvalues, XDWORD maxvalues)
* @brief      Tokenize numbers
* @ingroup    USERINTERFACE
*
* @param[in]  raw : Text to split, e.g. "4 8 4 8" or "4,8,4,8".
* @param[out] outvalues : Receives up to maxvalues parsed numbers, in order.
* @param[in]  maxvalues : Capacity of outvalues.
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_PROPERTYREGISTRY::TokenizeNumbers(XSTRING& raw, double* outvalues, XDWORD maxvalues)
{
  XDWORD n   = 0;
  XDWORD len = raw.GetSize();
  XDWORD p   = 0;

  if(!outvalues) return 0;

  while((p < len) && (n < maxvalues))
    {
      while(p < len)
        {
          XCHAR ch = raw[(int)p];
          if((ch != __C(' ')) && (ch != __C('\t')) && (ch != __C(','))) break;
          p++;
        }
      if(p >= len) break;

      XDWORD start = p;
      while(p < len)
        {
          XCHAR ch = raw[(int)p];
          if((ch == __C(' ')) || (ch == __C('\t')) || (ch == __C(','))) break;
          p++;
        }

      XSTRING tok;
      raw.Copy((int)start, (int)p, tok);
      outvalues[n++] = tok.ConvertToDouble();
    }

  return n;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD UI_PROPERTYREGISTRY::TokenizeTokens(XSTRING& raw, XSTRING* outtokens, XDWORD maxvalues)
* @brief      Tokenize length tokens (keep unit suffixes)
* @ingroup    USERINTERFACE
*
* @param[in]  raw : Text to split, e.g. "8 0.5rem 10%".
* @param[out] outtokens : Receives up to maxvalues tokens, in order.
* @param[in]  maxvalues : Capacity of outtokens.
*
* @return     XDWORD : Count of tokens found.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_PROPERTYREGISTRY::TokenizeTokens(XSTRING& raw, XSTRING* outtokens, XDWORD maxvalues)
{
  XDWORD n   = 0;
  XDWORD len = raw.GetSize();
  XDWORD p   = 0;

  if(!outtokens) return 0;

  while((p < len) && (n < maxvalues))
    {
      while(p < len)
        {
          XCHAR ch = raw[(int)p];
          if((ch != __C(' ')) && (ch != __C('\t')) && (ch != __C(','))) break;
          p++;
        }
      if(p >= len) break;

      XDWORD start = p;
      while(p < len)
        {
          XCHAR ch = raw[(int)p];
          if((ch == __C(' ')) || (ch == __C('\t')) || (ch == __C(','))) break;
          p++;
        }

      raw.Copy((int)start, (int)p, outtokens[n++]);
    }

  return n;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_PROPERTYREGISTRY::ExpandCSSShorthand4(XSTRING& raw, double out[4])
* @brief      Expand CSS shorthand4
* @ingroup    USERINTERFACE
*
* @param[in]  raw : Shorthand text, 1 to 4 numbers separated by comma/whitespace.
* @param[out] out : Receives the 4 expanded slots, CSS positional order (see header for the expansion table).
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_PROPERTYREGISTRY::ExpandCSSShorthand4(XSTRING& raw, double out[4])
{
  double vals[4] = { 0.0, 0.0, 0.0, 0.0 };
  XDWORD n       = TokenizeNumbers(raw, vals, 4);

  switch(n)
    {
      case 1  : out[0] = out[1] = out[2] = out[3] = vals[0];                            break;
      case 2  : out[0] = out[2] = vals[0]; out[1] = out[3] = vals[1];                   break;
      case 3  : out[0] = vals[0]; out[1] = out[3] = vals[1]; out[2] = vals[2];          break;
      default : out[0] = vals[0]; out[1] = vals[1]; out[2] = vals[2]; out[3] = vals[3]; break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_PROPERTYREGISTRY::ResolveLengthToken(XSTRING& raw, UI_LENGTH_CONTEXT& context, double& out)
* @brief      Resolve one CSS length token through UI_LENGTH
* @ingroup    USERINTERFACE
* 
* @param[in]  raw : 
* @param[in]  context : 
* @param[in]  out : 
* 
* @return     bool : true if is succesful.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTYREGISTRY::ResolveLengthToken(XSTRING& raw, UI_LENGTH_CONTEXT& context, double& out)
{
  if(raw.IsEmpty()) return false;

  UI_LENGTH length;
  if(!length.Parse(raw)) return false;

  UI_LENGTH_TYPE t = length.GetType();
  if((t == UI_LENGTH_TYPE_UNDEFINED) || (t == UI_LENGTH_TYPE_KEYWORD)) return false;

  return length.Resolve(context, out);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_PROPERTYREGISTRY::ExpandCSSShorthand4Lengths(XSTRING& raw, UI_LENGTH_CONTEXT& context, double out[4])
* @brief      Expand 1..4 length tokens (rem/vw/vh/%/em/px) into CSS TRBL slots
* @ingroup    USERINTERFACE
* 
* @param[in]  raw : 
* @param[in]  context : 
* @param[in]  out : 
* 
* @return     bool : true if is succesful.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTYREGISTRY::ExpandCSSShorthand4Lengths(XSTRING& raw, UI_LENGTH_CONTEXT& context, double out[4])
{
  if(!out) return false;

  XSTRING toks[4];
  XDWORD  n = TokenizeTokens(raw, toks, 4);
  if(n == 0) return false;

  double vals[4] = { 0.0, 0.0, 0.0, 0.0 };
  for(XDWORD i = 0; i < n; i++)
    {
      if(!ResolveLengthToken(toks[i], context, vals[i])) return false;
    }

  switch(n)
    {
      case 1  : out[0] = out[1] = out[2] = out[3] = vals[0];                            break;
      case 2  : out[0] = out[2] = vals[0]; out[1] = out[3] = vals[1];                   break;
      case 3  : out[0] = vals[0]; out[1] = out[3] = vals[1]; out[2] = vals[2];          break;
      default : out[0] = vals[0]; out[1] = vals[1]; out[2] = vals[2]; out[3] = vals[3]; break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_PROPERTYREGISTRY::ParseBoxShadow(XSTRING& raw, double& outoffsetx, double& outoffsety, double& outblur, XSTRING& outcolor)
* @brief      Parse box shadow
* @ingroup    USERINTERFACE
*
* @param[in]  raw : Shadow text ("<x> <y> <color>" or "<x> <y> <blur> <color>").
* @param[out] outoffsetx : Receives the X offset.
* @param[out] outoffsety : Receives the Y offset.
* @param[out] outblur : Receives the blur radius (0 when the 3-token form was used).
* @param[out] outcolor : Receives the colour token, unparsed (caller resolves it, e.g. UI_COLOR::SetFromString()).
*
* @return     bool : true if the mandatory offsets and a colour were found; otherwise false (outputs untouched).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTYREGISTRY::ParseBoxShadow(XSTRING& raw, double& outoffsetx, double& outoffsety, double& outblur, XSTRING& outcolor)
{
  if(raw.IsEmpty()) return false;

  XVECTOR<XSTRING*> tokens;
  XDWORD             len = raw.GetSize();
  XDWORD             p   = 0;

  while(p < len)
    {
      while(p < len)
        {
          XCHAR ch = raw[(int)p];
          if(ch != __C(' ') && ch != __C('\t')) break;
          p++;
        }
      if(p >= len) break;

      XDWORD start = p;
      while(p < len)
        {
          XCHAR ch = raw[(int)p];
          if(ch == __C(' ') || ch == __C('\t')) break;
          p++;
        }

      XSTRING* tok = GEN_NEW XSTRING();
      if(tok)
        {
          raw.Copy((int)start, (int)p, *tok);
          tokens.Add(tok);
        }
    }

  // Classify: leading numeric tokens are (offset-x, offset-y, blur); the first non-numeric one starts the
  // colour. A token is numeric when its first character is digit, '.', '-' or '+' AND it has no interior comma
  // (GEN's "R,G,B[,A]" colour tuple form can start with a digit, e.g. "0,0,0,120").
  double  x     = 0.0;
  double  y     = 0.0;
  double  blur  = 0.0;
  XSTRING color;
  int     nnum  = 0;

  for(XDWORD c=0; c<tokens.GetSize(); c++)
    {
      XSTRING* tok = tokens.Get(c);
      if(!tok || tok->IsEmpty()) continue;

      XCHAR first          = (*tok)[0];
      bool  starts_numeric = (first == __C('-')) || (first == __C('+')) || (first == __C('.')) ||
                              (first >= __C('0') && first <= __C('9'));
      bool  has_comma      = tok->FindCharacter(__C(',')) >= 0;
      bool  isnum          = starts_numeric && !has_comma;

      if(isnum && nnum < 3)
        {
          double v = tok->ConvertToDouble();
          if     (nnum == 0) x    = v;
          else if(nnum == 1) y    = v;
          else                blur = v;
          nnum++;
        }
       else
        {
          color = *tok;
          break;
        }
    }

  bool valid = (nnum >= 2) && !color.IsEmpty();

  if(valid)
    {
      outoffsetx = x;
      outoffsety = y;
      outblur    = blur;
      outcolor   = color;
    }

  for(XDWORD c=0; c<tokens.GetSize(); c++)
    {
      XSTRING* tok = tokens.Get(c);
      if(tok) GEN_DELETE tok;
    }
  tokens.DeleteAll();

  return valid;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_PROPERTYREGISTRY::ResolveMarginEdges(UI_STYLE& style, bool use_css_trbl, bool apply_longhands, double out_lrud[4], UI_LENGTH_CONTEXT* lengthctx)
* @brief      Resolve margin shorthand + optional longhands into LEFT/RIGHT/UP/DOWN edges.
* @ingroup    USERINTERFACE
*
* @param[in]  style : Style bag (XML attrs + CSS cascade + inline style already merged).
* @param[in]  use_css_trbl : true = 4-value shorthand is TOP,RIGHT,BOTTOM,LEFT; false = LEFT,RIGHT,UP,DOWN.
* @param[in]  apply_longhands : true = honour margin-top/right/bottom/left after the shorthand.
* @param[out] out_lrud : Receives LEFT, RIGHT, UP, DOWN when any margin key was present.
* @param[in]  lengthctx : Optional; when non-NULL with use_css_trbl, resolve rem/vw/vh/% via UI_LENGTH.
*
* @return     bool : true if at least one margin key was present; otherwise false (out_lrud untouched).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_PROPERTYREGISTRY::ResolveMarginEdges(UI_STYLE& style, bool use_css_trbl, bool apply_longhands, double out_lrud[4], UI_LENGTH_CONTEXT* lengthctx)
{
  if(!out_lrud) return false;

  double left = 0.0, right = 0.0, up = 0.0, down = 0.0;
  bool   any  = false;
  bool   use_lengths = (lengthctx != NULL) && use_css_trbl;

  XSTRING marginstr;
  if(style.Get(__L("margin"), marginstr))
    {
      any = true;

      if(use_lengths)
        {
          double out[4] = { 0.0, 0.0, 0.0, 0.0 };
          if(ExpandCSSShorthand4Lengths(marginstr, *lengthctx, out))
            {
              up    = out[0];
              right = out[1];
              down  = out[2];
              left  = out[3];
            }
           else
            {
              // Fallback: plain numbers if a token was not a resolvable length.
              ExpandCSSShorthand4(marginstr, out);
              up    = out[0];
              right = out[1];
              down  = out[2];
              left  = out[3];
            }
        }
       else
        {
          double vals[4] = { 0.0, 0.0, 0.0, 0.0 };
          XDWORD n = TokenizeNumbers(marginstr, vals, 4);

          if(n == 4 && !use_css_trbl)
            {
              // Legacy 4-value form (no stylesheet): LEFT, RIGHT, UP, DOWN.
              left  = vals[0];
              right = vals[1];
              up    = vals[2];
              down  = vals[3];
            }
           else
            {
              // CSS TRBL: 4 values TOP,RIGHT,BOTTOM,LEFT; 1-3 via ExpandCSSShorthand4 table.
              double out[4] = { 0.0, 0.0, 0.0, 0.0 };
              if(n == 4)
                {
                  out[0] = vals[0]; out[1] = vals[1]; out[2] = vals[2]; out[3] = vals[3];
                }
               else
                {
                  ExpandCSSShorthand4(marginstr, out);
                }

              up    = out[0];
              right = out[1];
              down  = out[2];
              left  = out[3];
            }
        }
    }

  if(apply_longhands)
    {
      if(use_lengths)
        {
          XSTRING mvstr;
          double  mv = 0.0;
          if(style.Get(__L("margin-left")  , mvstr) && ResolveLengthToken(mvstr, *lengthctx, mv)) { left  = mv; any = true; }
          if(style.Get(__L("margin-right") , mvstr) && ResolveLengthToken(mvstr, *lengthctx, mv)) { right = mv; any = true; }
          if(style.Get(__L("margin-top")   , mvstr) && ResolveLengthToken(mvstr, *lengthctx, mv)) { up    = mv; any = true; }
          if(style.Get(__L("margin-bottom"), mvstr) && ResolveLengthToken(mvstr, *lengthctx, mv)) { down  = mv; any = true; }
        }
       else
        {
          double mv = 0.0;
          if(style.Get(__L("margin-left")  , mv)) { left  = mv; any = true; }
          if(style.Get(__L("margin-right") , mv)) { right = mv; any = true; }
          if(style.Get(__L("margin-top")   , mv)) { up    = mv; any = true; }
          if(style.Get(__L("margin-bottom"), mv)) { down  = mv; any = true; }
        }
    }

  if(!any) return false;

  out_lrud[0] = left;
  out_lrud[1] = right;
  out_lrud[2] = up;
  out_lrud[3] = down;
  return true;
}
