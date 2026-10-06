/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileSVGObjText.cpp
* 
* @class      GRPVECTORFILESVGOBJTEXT
* @brief      Graphic Vector File SVG Object Text class (&lt;text&gt; / &lt;tspan&gt;)
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

#include "GRPVectorFileSVGObjText.h"

#include "XFileXML.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJTEXT::GRPVECTORFILESVGOBJTEXT()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJTEXT::GRPVECTORFILESVGOBJTEXT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGOBJTEXT::~GRPVECTORFILESVGOBJTEXT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGOBJTEXT::~GRPVECTORFILESVGOBJTEXT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJTEXT::ApplyData(XFILEXMLELEMENT* element)
* @brief      Apply data : read common attributes plus x, y, font-size, text-anchor and the text content
* @ingroup    GRAPHIC
* 
* @param[in]  element : xml element
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJTEXT::ApplyData(XFILEXMLELEMENT* element)
{
  if(!element) return false;

  GRPVECTORFILESVGOBJ::ApplyData(element);

  hasx = (element->GetValueAttribute(_L("x")) != NULL);
  hasy = (element->GetValueAttribute(_L("y")) != NULL);

  x  = element->GetFloatValueAttribute(_L("x"));
  y  = element->GetFloatValueAttribute(_L("y"));
  dx = element->GetFloatValueAttribute(_L("dx"));
  dy = element->GetFloatValueAttribute(_L("dy"));

  XSTRING valuefontsize;
  if(GetProperty(element, _L("font-size"), valuefontsize))
    {
      double parsed = valuefontsize.ConvertToDouble(0, NULL, false);           // tolerates "16px" -> 16
      if(parsed > 0.0)
        {
          fontsize    = parsed;
          hasfontsize = true;
        }
    }

  XSTRING valueanchor;
  if(GetProperty(element, _L("text-anchor"), valueanchor))
    {
      if(!valueanchor.Compare(_L("middle"), true))     textanchor = GRPVECTORFILESVGTEXTANCHOR_MIDDLE;
       else if(!valueanchor.Compare(_L("end"), true))  textanchor = GRPVECTORFILESVGTEXTANCHOR_END;
       else                                             textanchor = GRPVECTORFILESVGTEXTANCHOR_START;
    }

  //  Text content : trim leading / trailing whitespace.
  XSTRING rawtext(element->GetValue());

  int len = (int)rawtext.GetSize();
  int ini = 0;
  int end = len;

  while((ini < len) && ((rawtext[ini] == _C(' ')) || (rawtext[ini] == _C('\t')) || (rawtext[ini] == _C('\n')) || (rawtext[ini] == _C('\r'))))  ini++;
  while((end > ini) && ((rawtext[end-1] == _C(' ')) || (rawtext[end-1] == _C('\t')) || (rawtext[end-1] == _C('\n')) || (rawtext[end-1] == _C('\r'))))  end--;

  if(end > ini)  rawtext.Copy(ini, end, text);
   else          text.Empty();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* GRPVECTORFILESVGOBJTEXT::GetText()
* @brief      Get text
* @ingroup    GRAPHIC
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* GRPVECTORFILESVGOBJTEXT::GetText()   { return &text; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJTEXT::GetX()
* @brief      Get x
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double                     GRPVECTORFILESVGOBJTEXT::GetX()           { return x;          }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJTEXT::GetY()
* @brief      Get y
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double                     GRPVECTORFILESVGOBJTEXT::GetY()           { return y;          }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJTEXT::HasX()
* @brief      Check whether x exists
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool                       GRPVECTORFILESVGOBJTEXT::HasX()           { return hasx;       }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJTEXT::HasY()
* @brief      Check whether y exists
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool                       GRPVECTORFILESVGOBJTEXT::HasY()           { return hasy;       }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJTEXT::GetDX()
* @brief      Get dx
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double                     GRPVECTORFILESVGOBJTEXT::GetDX()          { return dx;         }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJTEXT::GetDY()
* @brief      Get dy
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double                     GRPVECTORFILESVGOBJTEXT::GetDY()          { return dy;         }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double GRPVECTORFILESVGOBJTEXT::GetFontSize()
* @brief      Get font size
* @ingroup    GRAPHIC
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double                     GRPVECTORFILESVGOBJTEXT::GetFontSize()    { return fontsize;   }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJTEXT::HasFontSize()
* @brief      Check whether font size exists
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool                       GRPVECTORFILESVGOBJTEXT::HasFontSize()    { return hasfontsize;}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPVECTORFILESVGTEXTANCHOR GRPVECTORFILESVGOBJTEXT::GetTextAnchor()
* @brief      Get text anchor
* @ingroup    GRAPHIC
* 
* @return     GRPVECTORFILESVGTEXTANCHOR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPVECTORFILESVGTEXTANCHOR GRPVECTORFILESVGOBJTEXT::GetTextAnchor()  { return textanchor; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPVECTORFILESVGOBJTEXT::GetProperty(XFILEXMLELEMENT* element, XCHAR* name, XSTRING& outvalue)
* @brief      Get property : read a property from its attribute or from the inline style="" string
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* @param[in]  element : element
* @param[in]  name : property name
* @param[in]  outvalue : property value
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPVECTORFILESVGOBJTEXT::GetProperty(XFILEXMLELEMENT* element, XCHAR* name, XSTRING& outvalue)
{
  XCHAR* attribute = element->GetValueAttribute(name);
  if(attribute)
    {
      outvalue = attribute;
      return true;
    }

  XCHAR* style = element->GetValueAttribute(_L("style"));
  if(!style) return false;

  XSTRING stylestr(style);

  XSTRING key(name);
  key += _L(":");

  int index = stylestr.Find(key.Get(), true, 0);
  if(index < 0) return false;

  int start = index + (int)key.GetSize();
  int end   = stylestr.Find(_L(";"), false, start);
  if(end < 0)  end = (int)stylestr.GetSize();

  stylestr.Copy(start, end, outvalue);
  outvalue.DeleteCharacter(_C(' '));

  return (!outvalue.IsEmpty());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRPVECTORFILESVGOBJTEXT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRPVECTORFILESVGOBJTEXT::Clean()
{
  x           = 0.0;
  y           = 0.0;
  hasx        = false;
  hasy        = false;
  dx          = 0.0;
  dy          = 0.0;
  fontsize    = 16.0;                                                          // SVG medium default
  hasfontsize = false;
  textanchor  = GRPVECTORFILESVGTEXTANCHOR_START;
}
