/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_ComputedStyle.cpp
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
* --------------------------------------------------------------------------------------------------------------------*/

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "UI_ComputedStyle.h"
#include "UI_PropertyRegistry.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_COMPUTEDSTYLE::UI_COMPUTEDSTYLE(): UI_STYLE()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_COMPUTEDSTYLE::UI_COMPUTEDSTYLE(): UI_STYLE()
{

}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_COMPUTEDSTYLE::~UI_COMPUTEDSTYLE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_COMPUTEDSTYLE::~UI_COMPUTEDSTYLE()
{

}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_COMPUTEDSTYLE::GetAliased(XCHAR* primarykey, XCHAR* secondarykey, XSTRING& value)
* @brief      Get aliased
* @ingroup    USERINTERFACE
*
* @param[in]  primarykey : Historical GEN attribute name, tried first.
* @param[in]  secondarykey : CSS-natural alias, tried only if primarykey was absent/empty.
* @param[out] value : Receives the winning value.
*
* @return     bool : true if either key resolved to a non-empty value; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_COMPUTEDSTYLE::GetAliased(XCHAR* primarykey, XCHAR* secondarykey, XSTRING& value)
{
  return UI_PROPERTYREGISTRY::GetAliased(*this, primarykey, secondarykey, value);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_COMPUTEDSTYLE::GetLength(XCHAR* key, UI_LENGTH& value)
* @brief      Get length
* @ingroup    USERINTERFACE
*
* @param[in]  key : Key to use.
* @param[out] value : Receives the parsed length.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_COMPUTEDSTYLE::GetLength(XCHAR* key, UI_LENGTH& value)
{
  XSTRING raw;
  if(!Get(key, raw)) return false;

  return value.Parse(raw);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_COMPUTEDSTYLE::GetLength(XCHAR* primarykey, XCHAR* secondarykey, UI_LENGTH& value)
* @brief      Get length
* @ingroup    USERINTERFACE
*
* @param[in]  primarykey : Historical GEN attribute name, tried first.
* @param[in]  secondarykey : CSS-natural alias, tried only if primarykey was absent/empty.
* @param[out] value : Receives the parsed length.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_COMPUTEDSTYLE::GetLength(XCHAR* primarykey, XCHAR* secondarykey, UI_LENGTH& value)
{
  XSTRING raw;
  if(!GetAliased(primarykey, secondarykey, raw)) return false;

  return value.Parse(raw);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_COMPUTEDSTYLE::GetColor(XCHAR* key, UI_COLOR& color)
* @brief      Get color
* @ingroup    USERINTERFACE
*
* @param[in]  key : Key to use.
* @param[out] color : Receives the parsed colour.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_COMPUTEDSTYLE::GetColor(XCHAR* key, UI_COLOR& color)
{
  XSTRING raw;
  if(!Get(key, raw) || raw.IsEmpty()) return false;

  return color.SetFromString(raw);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_COMPUTEDSTYLE::GetColor(XCHAR* primarykey, XCHAR* secondarykey, UI_COLOR& color)
* @brief      Get color
* @ingroup    USERINTERFACE
*
* @param[in]  primarykey : Historical GEN attribute name, tried first.
* @param[in]  secondarykey : CSS-natural alias, tried only if primarykey was absent/empty.
* @param[out] color : Receives the parsed colour.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_COMPUTEDSTYLE::GetColor(XCHAR* primarykey, XCHAR* secondarykey, UI_COLOR& color)
{
  XSTRING raw;
  if(!GetAliased(primarykey, secondarykey, raw)) return false;

  return color.SetFromString(raw);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_COMPUTEDSTYLE::GetBoxSizing(UI_BOXSIZING& value)
* @brief      Get box sizing
* @ingroup    USERINTERFACE
*
* @param[out] value : Receives UI_BOXSIZING_CONTENTBOX or UI_BOXSIZING_BORDERBOX; always left with a usable
*             result (CONTENTBOX by default) even when this returns false.
*
* @return     bool : true if "box-sizing" was present and held one of the two recognized keywords; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_COMPUTEDSTYLE::GetBoxSizing(UI_BOXSIZING& value)
{
  value = UI_BOXSIZING_CONTENTBOX;

  XSTRING raw;
  if(!Get(_L("box-sizing"), raw) || raw.IsEmpty()) return false;

  if(!raw.Compare(_L("border-box"), true))    { value = UI_BOXSIZING_BORDERBOX;  return true; }
  if(!raw.Compare(_L("content-box"), true))   { value = UI_BOXSIZING_CONTENTBOX; return true; }

  return false;                                 // unrecognized keyword: left at the CSS initial default above
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_COMPUTEDSTYLE::ClampToMinMax(double value, XCHAR* minkey, XCHAR* maxkey, UI_LENGTH_CONTEXT& context)
* @brief      Clamp to min max
* @ingroup    USERINTERFACE
*
* @param[in]  value : Already-resolved value (pixels) to clamp.
* @param[in]  minkey : Key of the lower bound, e.g. "min-width"; absent or non-numeric leaves the lower bound open.
* @param[in]  maxkey : Key of the upper bound, e.g. "max-width"; absent or non-numeric leaves the upper bound open.
* @param[in]  context : Reference values used to resolve whichever of minkey/maxkey is present (same one "value"
*             itself should have been resolved with).
*
* @return     double : "value" clamped between the two bounds (MIN wins over MAX on conflict, per CSS).
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_COMPUTEDSTYLE::ClampToMinMax(double value, XCHAR* minkey, XCHAR* maxkey, UI_LENGTH_CONTEXT& context)
{
  bool   havemin = false;
  bool   havemax = false;
  double minvalue = 0.0;
  double maxvalue = 0.0;

  UI_LENGTH minlength;
  if(GetLength(minkey, minlength) && minlength.Resolve(context, minvalue)) havemin = true;

  UI_LENGTH maxlength;
  if(GetLength(maxkey, maxlength) && maxlength.Resolve(context, maxvalue)) havemax = true;

  double result = value;

  if(havemax && result > maxvalue) result = maxvalue;
  if(havemin && result < minvalue) result = minvalue;   // applied AFTER max, so a MIN above MAX still wins

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_COMPUTEDSTYLE::ResolveContentSize(double specifiedsize, UI_BOXSIZING boxsizing, double paddingandborder)
* @brief      Resolve content size
* @note       STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  specifiedsize : The resolved width/height value as the author wrote it.
* @param[in]  boxsizing : Which box "specifiedsize" was written against.
* @param[in]  paddingandborder : That axis's padding + border-width, both sides, already resolved to pixels;
*             ignored for CONTENT-BOX.
*
* @return     double : The content-box size, never negative.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_COMPUTEDSTYLE::ResolveContentSize(double specifiedsize, UI_BOXSIZING boxsizing, double paddingandborder)
{
  double result = specifiedsize;

  if(boxsizing == UI_BOXSIZING_BORDERBOX) result -= paddingandborder;

  if(result < 0.0) result = 0.0;

  return result;
}
