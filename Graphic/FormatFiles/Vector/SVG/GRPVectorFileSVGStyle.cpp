/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGStyle.cpp
* 
* @class      GRPVECTORFILESVGSTYLE
* @brief      Graphic Vector File SVG Style class (fill, stroke, stroke-width, opacity, fill-rule)
* @ingroup    GRAPHIC
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

#include "GRPVectorFileSVGStyle.h"

#include "GRPVectorFileSVGCSSStyleSheet.h"

#include "XFileXML.h"
#include "XVector.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGSTYLE::GRPVECTORFILESVGSTYLE()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGSTYLE::GRPVECTORFILESVGSTYLE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGSTYLE::~GRPVECTORFILESVGSTYLE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGSTYLE::~GRPVECTORFILESVGSTYLE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGSTYLE::ApplyData(XFILEXMLELEMENT* element, GRPVECTORFILESVGCSSSTYLESHEET* stylesheet)
* @brief      Apply data : the document-wide universal ("*") rule if any, then presentation attributes, then the
*             class="" CSS rules, then the inline style attribute (each step overrides the previous one, same
*             cascade order browsers use -- the universal selector goes first because it has the LOWEST
*             specificity of any selector, real CSS included, so anything more specific below simply overwrites it)
* @ingroup    GRAPHIC
*
* @param[in]  element : xml element
* @param[in]  stylesheet : document-wide CSS class + universal-selector rules collected from every &lt;style&gt;
*                          element (NULL = ignore class="" and the universal rule both)
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGSTYLE::ApplyData(XFILEXMLELEMENT* element, GRPVECTORFILESVGCSSSTYLESHEET* stylesheet)
{
  if(!element) return false;

  // Document-wide universal selector ("* {fill:#RRGGBB}" in a <style> block -- see the NOTE in
  // GRPVECTORFILESVGCSSSTYLESHEET::ParseStyleSheetText). Applied first and unconditionally: every element in the
  // document goes through this same ApplyData, so this is exactly equivalent to a real "*" CSS rule matching
  // every element, at its correct (lowest) specificity -- every step below can still override it per element.
  if(stylesheet)
    {
      XSTRING* universal = stylesheet->Get(_L("*"));
      if(universal) ParseStyleAttribute(*universal);
    }

  static XCHAR* presentationattrs[] = { _L("fill")          ,
                                        _L("stroke")        ,
                                        _L("stroke-width")  ,
                                        _L("opacity")       ,
                                        _L("fill-opacity")  ,
                                        _L("stroke-opacity"),
                                        _L("fill-rule")     ,
                                        NULL                 };

  for(int c=0; presentationattrs[c]; c++)
    {
      XCHAR* value = element->GetValueAttribute(presentationattrs[c]);
      if(value)
        {
          XSTRING name(presentationattrs[c]);
          XSTRING strvalue(value);

          ApplyProperty(name, strvalue);
        }
    }

  // class="fil0 fil1" : resolved against the <style> CSS rules collected document wide (CorelDRAW/Illustrator/Figma
  // exports commonly use this instead of direct fill="" attributes). A class rule overrides a presentation
  // attribute, same as in a browser.
  if(stylesheet)
    {
      XCHAR* valueclass = element->GetValueAttribute(_L("class"));
      if(valueclass)
        {
          XSTRING classnames(valueclass);

          XVECTOR<XSTRING*> classlist;
          classnames.Split(_C(' '), classlist, false);

          for(XDWORD c=0; c<classlist.GetSize(); c++)
            {
              XSTRING* classname = classlist.Get(c);
              if(classname && !classname->IsEmpty())
                {
                  XSTRING* declarations = stylesheet->Get(*classname);
                  if(declarations) ParseStyleAttribute(*declarations);
                }
            }

          classlist.DeleteContents();
          classlist.DeleteAll();
        }
    }

  // The inline style attribute overrides the presentation attributes and the class="" CSS rules.
  XCHAR* style = element->GetValueAttribute(_L("style"));
  if(style)
    {
      XSTRING strstyle(style);
      ParseStyleAttribute(strstyle);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGSTYLE::InheritFrom(GRPVECTORFILESVGSTYLE& parent)
* @brief      Inherit from : for every property not specified locally, take the parent value
* @ingroup    GRAPHIC
* 
* @param[in]  parent : parent (effective) style
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGSTYLE::InheritFrom(GRPVECTORFILESVGSTYLE& parent)
{
  if(!fillspecified)
    {
      fillspecified     = parent.IsFillSpecified();
      fillnone          = !parent.HasFill();
      fillcolor         = parent.GetFillColor();
      fillispaintserver = parent.IsFillPaintServer();
      if(fillispaintserver)  fillpaintid = (*parent.GetFillPaintID());
    }

  if(!strokespecified)
    {
      strokespecified     = parent.IsStrokeSpecified();
      strokenone          = !parent.HasStroke();
      strokecolor         = parent.GetStrokeColor();
      strokeispaintserver = parent.IsStrokePaintServer();
      if(strokeispaintserver)  strokepaintid = (*parent.GetStrokePaintID());
    }

  if(!strokewidthspecified)
    {
      strokewidthspecified = parent.IsStrokeWidthSpecified();
      strokewidth          = parent.GetStrokeWidth();
    }

  // opacity is not inherited in SVG, but fill-opacity / stroke-opacity are.
  fillopacity   = parent.GetFillOpacity()   * fillopacity;
  strokeopacity = parent.GetStrokeOpacity() * strokeopacity;

  // fill-rule is inherited the same way fill/stroke are (not multiplied like the opacities above): if this node
  // never specified its own fill-rule (neither as a presentation attribute, nor via class="", nor inline style),
  // take the parent's effective (possibly itself inherited) value instead of silently staying at the
  // GRP2DPATHFILLRULE_NONZERO default set by Clean(). This is what lets a "fill-rule:evenodd" declared once on the
  // <svg> root's own style="" (the common CorelDRAW/Illustrator export pattern) reach every descendant <path>,
  // including ones several levels down (e.g. root -> <g> -> <path>), instead of only affecting the root itself.
  if(!fillrulespecified)
    {
      fillrulespecified = parent.IsFillRuleSpecified();
      fillrule          = parent.GetFillRule();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGSTYLE::HasFill()
* @brief      Has fill
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGSTYLE::HasFill()                       { return !fillnone; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGSTYLE::IsFillSpecified()
* @brief      Is fill specified
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGSTYLE::IsFillSpecified()               { return fillspecified; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCOLOR_RGBA8& GRPVECTORFILESVGSTYLE::GetFillColor()
* @brief      Get fill color
* @ingroup    GRAPHIC
* 
* @return     GRP2DCOLOR_RGBA8& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCOLOR_RGBA8& GRPVECTORFILESVGSTYLE::GetFillColor()     { return fillcolor; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGSTYLE::HasStroke()
* @brief      Has stroke
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGSTYLE::HasStroke()                     { return (strokespecified && !strokenone); }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGSTYLE::IsStrokeSpecified()
* @brief      Is stroke specified
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGSTYLE::IsStrokeSpecified()             { return strokespecified; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DCOLOR_RGBA8& GRPVECTORFILESVGSTYLE::GetStrokeColor()
* @brief      Get stroke color
* @ingroup    GRAPHIC
* 
* @return     GRP2DCOLOR_RGBA8& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DCOLOR_RGBA8& GRPVECTORFILESVGSTYLE::GetStrokeColor()   { return strokecolor; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGSTYLE::IsFillPaintServer()
* @brief      Is fill paint server : the fill is a url(\#id) reference (gradient / pattern)
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGSTYLE::IsFillPaintServer()             { return fillispaintserver; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPVECTORFILESVGSTYLE::GetFillPaintID()
* @brief      Get fill paint id : the referenced id (without '#')
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPVECTORFILESVGSTYLE::GetFillPaintID()            { return &fillpaintid; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGSTYLE::IsStrokePaintServer()
* @brief      Is stroke paint server : the stroke is a url(\#id) reference
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGSTYLE::IsStrokePaintServer()           { return strokeispaintserver; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPVECTORFILESVGSTYLE::GetStrokePaintID()
* @brief      Get stroke paint id : the referenced id (without '#')
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPVECTORFILESVGSTYLE::GetStrokePaintID()          { return &strokepaintid; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGSTYLE::ExtractPaintServerID(XSTRING& value, XSTRING& id)
* @brief      Extract paint server id : parse the id from a "url(\#id)" / "url(id)" value
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  value : the url(...) value
* @param[in]  id : extracted id (without '#')
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGSTYLE::ExtractPaintServerID(XSTRING& value, XSTRING& id)
{
  id.Empty();

  int open  = value.Find(_L("("), false, 0);
  int close = value.Find(_L(")"), false, 0);

  if((open < 0) || (close <= open)) return;

  int start = open + 1;

  if(value[start] == _C('#'))  start++;

  value.Copy(start, close, id);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGSTYLE::GetStrokeWidth()
* @brief      Get stroke width
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGSTYLE::GetStrokeWidth()              { return strokewidth; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGSTYLE::IsStrokeWidthSpecified()
* @brief      Is stroke width specified
* @ingroup    GRAPHIC
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGSTYLE::IsStrokeWidthSpecified()        { return strokewidthspecified; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGSTYLE::GetOpacity()
* @brief      Get opacity
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGSTYLE::GetOpacity()                  { return opacity; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGSTYLE::GetFillOpacity()
* @brief      Get fill opacity
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGSTYLE::GetFillOpacity()              { return fillopacity; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGSTYLE::GetStrokeOpacity()
* @brief      Get stroke opacity
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double GRPVECTORFILESVGSTYLE::GetStrokeOpacity()            { return strokeopacity; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DPATHFILLRULE GRPVECTORFILESVGSTYLE::GetFillRule()
* @brief      Get fill rule
* @ingroup    GRAPHIC
* 
* @return     GRP2DPATHFILLRULE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DPATHFILLRULE GRPVECTORFILESVGSTYLE::GetFillRule()      { return fillrule; }


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool GRPVECTORFILESVGSTYLE::IsFillRuleSpecified()
* @brief      Is fill rule specified : true if THIS node declared its own fill-rule (attribute, class="", or inline
*             style); false if the current value is either the NONZERO default or an inherited one -- used by
*             InheritFrom() to decide whether a parent's fill-rule should keep propagating further down the tree.
* @ingroup    GRAPHIC
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGSTYLE::IsFillRuleSpecified()            { return fillrulespecified; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGSTYLE::ApplyProperty(XSTRING& name, XSTRING& value)
* @brief      Apply property : set a single style property by name
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  name : property name
* @param[in]  value : property value
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGSTYLE::ApplyProperty(XSTRING& name, XSTRING& value)
{
  XSTRING propname(name);
  propname.DeleteCharacter(_C(' '));
  propname.ToLowerCase();

  XSTRING propvalue(value);
  propvalue.DeleteCharacter(_C(' '));

  if(propvalue.IsEmpty()) return false;

  if(!propname.Compare(_L("fill"), true))
    {
      if(propvalue.Find(_L("url("), true, 0) == 0)
        {
          ExtractPaintServerID(propvalue, fillpaintid);
          fillispaintserver = true;
          fillnone          = false;
          fillspecified     = true;
        }
       else
        {
          bool isnone = false;
          if(ParseColor(propvalue, fillcolor, isnone))
            {
              fillnone          = isnone;
              fillispaintserver = false;
              fillspecified     = true;
            }
        }
    }
   else if(!propname.Compare(_L("stroke"), true))
    {
      if(propvalue.Find(_L("url("), true, 0) == 0)
        {
          ExtractPaintServerID(propvalue, strokepaintid);
          strokeispaintserver = true;
          strokenone          = false;
          strokespecified     = true;
        }
       else
        {
          bool isnone = false;
          if(ParseColor(propvalue, strokecolor, isnone))
            {
              strokenone          = isnone;
              strokeispaintserver = false;
              strokespecified     = true;
            }
        }
    }
   else if(!propname.Compare(_L("stroke-width"), true))
    {
      strokewidth          = propvalue.ConvertToDouble(0, NULL, false);
      strokewidthspecified = true;
    }
   else if(!propname.Compare(_L("opacity"), true))
    {
      opacity = propvalue.ConvertToDouble(0, NULL, false);
    }
   else if(!propname.Compare(_L("fill-opacity"), true))
    {
      fillopacity = propvalue.ConvertToDouble(0, NULL, false);
    }
   else if(!propname.Compare(_L("stroke-opacity"), true))
    {
      strokeopacity = propvalue.ConvertToDouble(0, NULL, false);
    }
   else if(!propname.Compare(_L("fill-rule"), true))
    {
      if(!propvalue.Compare(_L("evenodd"), true))  fillrule = GRP2DPATHFILLRULE_EVENODD;
       else                                         fillrule = GRP2DPATHFILLRULE_NONZERO;

      fillrulespecified = true;
    }
   else
    {
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGSTYLE::ParseStyleAttribute(XSTRING& style)
* @brief      Parse style attribute : "fill:#f00;stroke:black;stroke-width:2"
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  style : style attribute value
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGSTYLE::ParseStyleAttribute(XSTRING& style)
{
  XVECTOR<XSTRING*> declarations;
  style.Split(_C(';'), declarations, false);

  for(XDWORD c=0; c<declarations.GetSize(); c++)
    {
      XSTRING* declaration = declarations.Get(c);
      if(declaration && !declaration->IsEmpty())
        {
          int colon = declaration->Find(_L(":"), false, 0);
          if(colon > 0)
            {
              XSTRING name;
              XSTRING value;

              declaration->Copy(0, colon, name);
              declaration->Copy(colon+1, (int)declaration->GetSize(), value);

              ApplyProperty(name, value);
            }
        }
    }

  declarations.DeleteContents();
  declarations.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGSTYLE::ParseColor(XSTRING& value, GRP2DCOLOR_RGBA8& color, bool& isnone)
* @brief      Parse color : \#rgb, \#rrggbb, rgb(r,g,b), none, basic named colors
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  value : color string
* @param[in]  color : output color
* @param[in]  isnone : true if the color is 'none'
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGSTYLE::ParseColor(XSTRING& value, GRP2DCOLOR_RGBA8& color, bool& isnone)
{
  isnone = false;

  XSTRING str(value);
  str.DeleteCharacter(_C(' '));

  if(str.IsEmpty()) return false;

  if(!str.Compare(_L("none"), true))
    {
      isnone = true;
      return true;
    }

  if(str[0] == _C('#'))
    {
      int len = (int)str.GetSize() - 1;

      if(len == 3)
        {
          int r = HexValue(str[1]);
          int g = HexValue(str[2]);
          int b = HexValue(str[3]);

          color = GRP2DCOLOR_RGBA8((r*16)+r, (g*16)+g, (b*16)+b);
          return true;
        }
       else if(len >= 6)
        {
          int r = (HexValue(str[1])*16) + HexValue(str[2]);
          int g = (HexValue(str[3])*16) + HexValue(str[4]);
          int b = (HexValue(str[5])*16) + HexValue(str[6]);

          color = GRP2DCOLOR_RGBA8(r, g, b);
          return true;
        }

      return false;
    }

  if(str.Find(_L("rgb("), true, 0) == 0)
    {
      int open  = str.Find(_L("("), false, 0);
      int close = str.Find(_L(")"), false, 0);

      if((open >= 0) && (close > open))
        {
          XSTRING inside;
          str.Copy(open+1, close, inside);
          inside.Replace(_L(","), _L(" "));

          XVECTOR<XSTRING*> tokens;
          inside.Split(_C(' '), tokens, false);

          int comp[3] = { 0, 0, 0 };
          int n       = 0;

          for(XDWORD c=0; c<tokens.GetSize() && n<3; c++)
            {
              XSTRING* token = tokens.Get(c);
              if(token && !token->IsEmpty())
                {
                  comp[n] = token->ConvertToInt(0, NULL, false);
                  n++;
                }
            }

          tokens.DeleteContents();
          tokens.DeleteAll();

          if(n >= 3)
            {
              color = GRP2DCOLOR_RGBA8(comp[0], comp[1], comp[2]);
              return true;
            }
        }

      return false;
    }

  // Basic named colors.
  struct GRPVECTORFILESVGNAMEDCOLOR { XCHAR* name;  int r;  int g;  int b; };

  static GRPVECTORFILESVGNAMEDCOLOR namedcolors[] = { { _L("black")  ,   0,   0,   0 },
                                                      { _L("white")  , 255, 255, 255 },
                                                      { _L("red")    , 255,   0,   0 },
                                                      { _L("green")  ,   0, 128,   0 },
                                                      { _L("blue")   ,   0,   0, 255 },
                                                      { _L("yellow") , 255, 255,   0 },
                                                      { _L("cyan")   ,   0, 255, 255 },
                                                      { _L("aqua")   ,   0, 255, 255 },
                                                      { _L("magenta"), 255,   0, 255 },
                                                      { _L("fuchsia"), 255,   0, 255 },
                                                      { _L("gray")   , 128, 128, 128 },
                                                      { _L("grey")   , 128, 128, 128 },
                                                      { _L("silver") , 192, 192, 192 },
                                                      { _L("maroon") , 128,   0,   0 },
                                                      { _L("olive")  , 128, 128,   0 },
                                                      { _L("lime")   ,   0, 255,   0 },
                                                      { _L("teal")   ,   0, 128, 128 },
                                                      { _L("navy")   ,   0,   0, 128 },
                                                      { _L("purple") , 128,   0, 128 },
                                                      { _L("orange") , 255, 165,   0 },
                                                      { NULL          ,   0,   0,   0 } };

  for(int c=0; namedcolors[c].name; c++)
    {
      if(!str.Compare(namedcolors[c].name, true))
        {
          color = GRP2DCOLOR_RGBA8(namedcolors[c].r, namedcolors[c].g, namedcolors[c].b);
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int GRPVECTORFILESVGSTYLE::HexValue(XCHAR character)
* @brief      Hex value : convert one hexadecimal character to its numeric value (0..15)
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  character : hexadecimal character
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int GRPVECTORFILESVGSTYLE::HexValue(XCHAR character)
{
  if((character >= _C('0')) && (character <= _C('9')))  return (int)(character - _C('0'));
  if((character >= _C('a')) && (character <= _C('f')))  return (int)(character - _C('a')) + 10;
  if((character >= _C('A')) && (character <= _C('F')))  return (int)(character - _C('A')) + 10;

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGSTYLE::Clean()
* @brief      Clean the attributes of the class: Default initialize (SVG defaults)
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGSTYLE::Clean()
{
  fillspecified        = false;
  fillnone             = false;
  fillcolor            = GRP2DCOLOR_RGBA8(0, 0, 0);                            // SVG default fill is black
  fillispaintserver    = false;
  fillpaintid.Empty();

  strokespecified      = false;
  strokenone           = true;                                               // SVG default stroke is none
  strokecolor          = GRP2DCOLOR_RGBA8(0, 0, 0);
  strokeispaintserver  = false;
  strokepaintid.Empty();

  strokewidthspecified = false;
  strokewidth          = 1.0;                                               // SVG default stroke-width is 1

  opacity              = 1.0;
  fillopacity          = 1.0;
  strokeopacity        = 1.0;

  fillrule             = GRP2DPATHFILLRULE_NONZERO;
  fillrulespecified    = false;
}




