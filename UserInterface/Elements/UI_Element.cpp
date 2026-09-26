/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element.cpp
* 
* @class      UI_ELEMENT
* @brief      User Interface Element class
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

#include "UI_Element.h"

#include "XFactory.h"
#include "XTimer.h"

#include "UI_Color.h"
#include "UI_Element_Scroll.h"
#include "UI_Layout.h"
#include "UI_Manager.h"
#include "UI_Style.h"
#include "UI_StyleSheet.h"
#include "UI_PropertyRegistry.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT::UI_ELEMENT()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT::UI_ELEMENT()    
{ 
  computed_style = NULL;
  Clean();     

  isvisible      = true;  
  isactive       = true;

  blink_xtimer=GEN_XFACTORY.CreateTimer();
  blink_state    = true;

  style_transition_xtimer = GEN_XFACTORY.CreateTimer();

  z_level        = 1;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT::~UI_ELEMENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT::~UI_ELEMENT()
{
  if(blink_xtimer)
    {
      GEN_XFACTORY.DeleteTimer(blink_xtimer);
      blink_xtimer = NULL;
    }

  if(style_transition_xtimer)
    {
      GEN_XFACTORY.DeleteTimer(style_transition_xtimer);
      style_transition_xtimer = NULL;
    }

  // Owned per-class split view. SetClassNames() also clears this vector, but the destructor may be reached
  // without any Set call having ever run, so we still walk it defensively.
  for(XDWORD c=0; c<class_names.GetSize(); c++)
    {
      XSTRING* name = class_names.Get(c);
      if(name) GEN_DELETE name;
    }
  class_names.DeleteAll();

  DeleteAllComposeElements();

  if(computed_style)
    {
      GEN_DELETE computed_style;
      computed_style = NULL;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_ELEMENT::GetName()
* @brief      Get name
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_ELEMENT::GetName()
{
  return &name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_TYPE UI_ELEMENT::GetType()
* @brief      Get type
* @ingroup    USERINTERFACE
* 
* @return     UI_ELEMENT_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_TYPE UI_ELEMENT::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetType(UI_ELEMENT_TYPE type)
* @brief      Set type
* @ingroup    USERINTERFACE
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetType(UI_ELEMENT_TYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_ELEMENT::GetTypeString()
* @brief      Get type string
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_ELEMENT::GetTypeString()
{
  return &type_string;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* UI_ELEMENT::GetClassNamesRaw()
* @brief      Get raw "class" attribute string as authored ("chip primary big"). Empty when no class was set.
* @ingroup    USERINTERFACE
*
* @return     XSTRING* : Pointer to the raw class attribute string.
*
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_ELEMENT::GetClassNamesRaw()
{
  return &class_names_raw;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<XSTRING*>* UI_ELEMENT::GetClassNames()
* @brief      Get the split class list (one XSTRING per class name). Empty vector when no class was set.
* @ingroup    USERINTERFACE
*
* @return     XVECTOR<XSTRING*>* : Pointer to the owned class-name vector.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XSTRING*>* UI_ELEMENT::GetClassNames()
{
  return &class_names;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetClassNames(XCHAR* rawlist)
* @brief      Replace the class list. Splits `rawlist` on whitespace and rebuilds the vector view. Passing NULL
*             or an empty string clears the list. Idempotent.
* @ingroup    USERINTERFACE
*
* @param[in]  rawlist : Space-separated class list, as authored in the "class" XML attribute.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetClassNames(XCHAR* rawlist)
{
  // Discard any previous split view (owned).
  for(XDWORD c=0; c<class_names.GetSize(); c++)
    {
      XSTRING* name = class_names.Get(c);
      if(name) GEN_DELETE name;
    }
  class_names.DeleteAll();

  class_names_raw.Empty();
  if(!rawlist) return;

  class_names_raw.Set(rawlist);

  // Split on whitespace (space, tab, CR, LF). XSTRING::Split is single-char; we walk the string manually to
  // support the four whitespace characters at once and to skip empty runs.
  XDWORD  size = class_names_raw.GetSize();
  XDWORD  i    = 0;

  while(i < size)
    {
      // Skip leading whitespace.
      while(i < size)
        {
          XCHAR ch = class_names_raw[(int)i];
          if(ch != __C(' ') && ch != __C('\t') && ch != __C('\r') && ch != __C('\n')) break;
          i++;
        }

      if(i >= size) break;

      // Collect one class name.
      XDWORD start = i;
      while(i < size)
        {
          XCHAR ch = class_names_raw[(int)i];
          if(ch == __C(' ') || ch == __C('\t') || ch == __C('\r') || ch == __C('\n')) break;
          i++;
        }

      if(i > start)
        {
          XSTRING* one = GEN_NEW XSTRING();
          if(one)
            {
              class_names_raw.Copy((int)start, (int)i, *one);
              class_names.Add(one);
            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetClassNames(XSTRING& rawlist)
* @brief      Convenience overload; forwards to SetClassNames(XCHAR*).
* @ingroup    USERINTERFACE
*
* @param[in]  rawlist : Space-separated class list, as authored in the "class" XML attribute.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetClassNames(XSTRING& rawlist)
{
  SetClassNames(rawlist.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT::HasClass(XCHAR* classname)
* @brief      Case-insensitive membership test against the split class list.
* @ingroup    USERINTERFACE
*
* @param[in]  classname : Class name to look up.
*
* @return     bool : true if the class is present; false otherwise (including a NULL/empty query).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::HasClass(XCHAR* classname)
{
  if(!classname)      return false;
  if(!classname[0])   return false;

  for(XDWORD c=0; c<class_names.GetSize(); c++)
    {
      XSTRING* one = class_names.Get(c);
      if(one && one->Compare(classname, true) == 0) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_CHROMEROLE UI_ELEMENT::GetChromeRole()
* @brief      Get chrome role
* @ingroup    USERINTERFACE
* 
* @return     UI_ELEMENT_CHROMEROLE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_CHROMEROLE UI_ELEMENT::GetChromeRole()
{
  return chromerole;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetChromeRole(UI_ELEMENT_CHROMEROLE chromerole)
* @brief      Set chrome role
* @ingroup    USERINTERFACE
* 
* @param[in]  chromerole : Chromerole value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetChromeRole(UI_ELEMENT_CHROMEROLE chromerole)
{
  this->chromerole = chromerole;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT* UI_ELEMENT::GetFather()
* @brief      Get father
* @ingroup    USERINTERFACE
* 
* @return     UI_ELEMENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* UI_ELEMENT::GetFather()
{
  return father;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetFather(UI_ELEMENT* father)
* @brief      Set father
* @ingroup    USERINTERFACE
* 
* @param[in]  father : Father pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetFather(UI_ELEMENT* father)
{
  this->father = father;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LAYOUT* UI_ELEMENT::GetLayout()
* @brief      Get layout
* @ingroup    USERINTERFACE
*
* @return     UI_LAYOUT* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUT* UI_ELEMENT::GetLayout()
{
  return element_layout;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetLayout(UI_LAYOUT* layout)
* @brief      Set layout
* @ingroup    USERINTERFACE
*
* @param[in]  layout : Layout pointer to use.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetLayout(UI_LAYOUT* layout)
{
  element_layout = layout;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::IsDetached()
* @brief      Is detached
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::IsDetached()
{
  return isdetached;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetIsDetached(bool isdetached)
* @brief      Set is detached
* @ingroup    USERINTERFACE
* 
* @param[in]  isdetached : Isdetached value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetIsDetached(bool isdetached)
{
  this->isdetached = isdetached;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_COLOR* UI_ELEMENT::GetColor()
* @brief      Get color
* @ingroup    USERINTERFACE
* 
* @return     UI_COLOR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_COLOR* UI_ELEMENT::GetColor()
{
  return &color;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_COLOR* UI_ELEMENT::GetBackgroundColor()
* @brief      Get background color
* @ingroup    USERINTERFACE
* 
* @return     UI_COLOR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_COLOR* UI_ELEMENT::GetBackgroundColor()
{
  return &backgroundcolor;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT::IsColorSet()
* @brief      True iff the "color" property was authored (XML attribute or CSS declaration) on this element,
*             as opposed to left at its zero-initialised default. Consumers use this flag to skip inheriting
*             from ancestors, and to walk the parent chain when a child requests CSS "color: inherit".
* @ingroup    USERINTERFACE
*
* @return     bool : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::IsColorSet()
{
  return color_set;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetColorSet(bool value)
* @brief      Mark the "color" property as authored on this element.
* @ingroup    USERINTERFACE
*
* @param[in]  value : true = authored, false = default.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetColorSet(bool value)
{
  color_set = value;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT::IsBackgroundColorSet()
* @brief      True iff the "bckgrdcolor" (or CSS "background-color") property was authored on this element.
*             Draw_Form uses it to decide between the CSS-natural fill (background_color) and the pre-step-6
*             fallback of using the "color" property as the fill.
* @ingroup    USERINTERFACE
*
* @return     bool : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::IsBackgroundColorSet()
{
  return background_color_set;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetBackgroundColorSet(bool value)
* @brief      Mark the "bckgrdcolor" property as authored on this element.
* @ingroup    USERINTERFACE
*
* @param[in]  value : true = authored, false = default.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetBackgroundColorSet(bool value)
{
  background_color_set = value;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT::IsBoxShadowSet()
* @brief      True iff a CSS box-shadow was authored on this element. Consumers gate both the shadow render
*             and the rebuild-area expansion on this flag, so elements without a shadow pay nothing.
* @ingroup    USERINTERFACE
*
* @return     bool : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::IsBoxShadowSet()
{
  return box_shadow_set;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetBoxShadowSet(bool value)
* @brief      Latch the "shadow authored" flag. Called by the loader after the parser has filled the offset /
*             blur / colour components, and cleared to false when a stylesheet stops providing a shadow.
* @ingroup    USERINTERFACE
*
* @param[in]  value : Value to store.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetBoxShadowSet(bool value)
{
  box_shadow_set = value;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT::GetShadowOffsetX()
* @brief      Shadow horizontal offset in pixels. Positive shifts the shadow to the right.
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetShadowOffsetX()
{
  return shadow_offset_x;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetShadowOffsetX(double value)
* @brief      Set shadow horizontal offset.
* @ingroup    USERINTERFACE
*
* @param[in]  value : Value to store.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetShadowOffsetX(double value)
{
  shadow_offset_x = value;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT::GetShadowOffsetY()
* @brief      Shadow vertical offset in pixels. Positive shifts the shadow DOWN (screen y-down convention).
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetShadowOffsetY()
{
  return shadow_offset_y;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetShadowOffsetY(double value)
* @brief      Set shadow vertical offset.
* @ingroup    USERINTERFACE
*
* @param[in]  value : Value to store.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetShadowOffsetY(double value)
{
  shadow_offset_y = value;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT::GetShadowBlur()
* @brief      Shadow blur radius in pixels. Stored for authoring parity with CSS, but not rendered in this
*             rebanada; the skin currently draws a hard-edged shadow regardless of this value.
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetShadowBlur()
{
  return shadow_blur;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetShadowBlur(double value)
* @brief      Set shadow blur radius (accepted but not yet rendered).
* @ingroup    USERINTERFACE
*
* @param[in]  value : Value to store.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetShadowBlur(double value)
{
  shadow_blur = value;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_COLOR* UI_ELEMENT::GetShadowColor()
* @brief      Get the shadow tint colour.
* @ingroup    USERINTERFACE
*
* @return     UI_COLOR* : Pointer to the owned colour object.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_COLOR* UI_ELEMENT::GetShadowColor()
{
  return &shadow_color;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT::GetXPosition()
* @brief      Get X position
* @ingroup    USERINTERFACE
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetXPosition()
{
  return x_position;
}
	

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::SetXPosition(double x_position)
* @brief      Set X position
* @ingroup    USERINTERFACE
* 
* @param[in]  x_position : X position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::SetXPosition(double x_position)   
{
  this->x_position = x_position;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_ELEMENT::GetYPosition()
* @brief      Get Y position
* @ingroup    USERINTERFACE
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetYPosition()
{
  return y_position;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::SetYPosition(double y_position)
* @brief      Set Y position
* @ingroup    USERINTERFACE
* 
* @param[in]  y_position : Y position value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::SetYPosition(double y_position)
{
  this->y_position = y_position;

  return true;
}		


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_ELEMENT::GetZLevel()
* @brief      Get Z level
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_ELEMENT::GetZLevel()
{
  return z_level;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::SetZLevel(XDWORD z_level)
* @brief      Set Z level
* @ingroup    USERINTERFACE
* 
* @param[in]  z_level : Z level value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::SetZLevel(XDWORD z_level)
{
  this->z_level = z_level;

  return true;
}		


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_TYPE_DIRECTION UI_ELEMENT::GetDirection()
* @brief      Get direction
* @ingroup    USERINTERFACE
* 
* @return     UI_ELEMENT_TYPE_DIRECTION : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_TYPE_DIRECTION UI_ELEMENT::GetDirection()
{
  return direction;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::SetDirection(UI_ELEMENT_TYPE_DIRECTION direction)
* @brief      Set direction
* @ingroup    USERINTERFACE
* 
* @param[in]  direction : Direction value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::SetDirection(UI_ELEMENT_TYPE_DIRECTION direction)
{
  this->direction = direction;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_TYPE_ALIGN UI_ELEMENT::GetTextAlign()
* @brief      Get text align
* @ingroup    USERINTERFACE
*
* @return     UI_ELEMENT_TYPE_ALIGN : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_TYPE_ALIGN UI_ELEMENT::GetTextAlign()
{
  return textalign;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT::SetTextAlign(UI_ELEMENT_TYPE_ALIGN textalign)
* @brief      Set text align
* @ingroup    USERINTERFACE
*
* @param[in]  textalign : Textalign value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::SetTextAlign(UI_ELEMENT_TYPE_ALIGN textalign)
{
  this->textalign = textalign;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_ELEMENT::GetXPositionWithScroll()
* @brief      Get X position with scroll
* @ingroup    USERINTERFACE
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetXPositionWithScroll()
{
  return x_positionwithscroll;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::SetXPositionWithScroll(double x_positionwithscroll)
* @brief      Set X position with scroll
* @ingroup    USERINTERFACE
* 
* @param[in]  x_positionwithscroll : X positionwithscroll value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::SetXPositionWithScroll(double x_positionwithscroll)
{
  this->x_positionwithscroll = x_positionwithscroll;

  return true;
}		


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_ELEMENT::GetYPositionWithScroll()
* @brief      Get Y position with scroll
* @ingroup    USERINTERFACE
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetYPositionWithScroll()
{
  return y_positionwithscroll;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::SetYPositionWithScroll(double y_positionwithscroll)
* @brief      Set Y position with scroll
* @ingroup    USERINTERFACE
* 
* @param[in]  y_positionwithscroll : Y positionwithscroll value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::SetYPositionWithScroll(double y_positionwithscroll)
{
  this->y_positionwithscroll = y_positionwithscroll;
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_BOUNDARYLINE* UI_ELEMENT::GetBoundaryLine()
* @brief      Get boundary line
* @ingroup    USERINTERFACE
* 
* @return     UI_BOUNDARYLINE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_BOUNDARYLINE* UI_ELEMENT::GetBoundaryLine()
{
  return &boundaryline;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetBoundaryLine(UI_BOUNDARYLINE& boundaryline)
* @brief      Set boundary line
* @ingroup    USERINTERFACE
* 
* @param[in]  boundaryline : Boundaryline value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetBoundaryLine(UI_BOUNDARYLINE& boundaryline)
{
  this->boundaryline.CopyFrom(boundaryline);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_ELEMENT::GetLeftX()
* @brief      Resolved left screen edge from the stored position (single coordinate conversion point).
* @ingroup    USERINTERFACE
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetLeftX()
{
  return UI_BOUNDARYLINE_EdgeLeft(GetXPosition(), GetBoundaryLine()->width);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_ELEMENT::GetRightX()
* @brief      Resolved right screen edge from the stored position (single coordinate conversion point).
* @ingroup    USERINTERFACE
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetRightX()
{
  return UI_BOUNDARYLINE_EdgeRight(GetXPosition(), GetBoundaryLine()->width);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_ELEMENT::GetTopY()
* @brief      Resolved top screen edge from the stored position (single coordinate conversion point).
* @ingroup    USERINTERFACE
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetTopY()
{
  return UI_BOUNDARYLINE_EdgeTop(GetYPosition(), GetBoundaryLine()->height);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_ELEMENT::GetBottomY()
* @brief      Resolved bottom screen edge from the stored position (single coordinate conversion point).
* @ingroup    USERINTERFACE
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetBottomY()
{
  return UI_BOUNDARYLINE_EdgeBottom(GetYPosition(), GetBoundaryLine()->height);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         double UI_ELEMENT::GetMargin(UI_ELEMENT_TYPE_ALIGN position)
* @brief      Get margin
* @ingroup    USERINTERFACE
* 
* @param[in]  position : Position value.
* 
* @return     double : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetMargin(UI_ELEMENT_TYPE_ALIGN position)
{
  switch((int)position)
    {
      case UI_ELEMENT_TYPE_ALIGN_LEFT   : return margin[0]; break;
      case UI_ELEMENT_TYPE_ALIGN_RIGHT	: return margin[1]; break;
      case UI_ELEMENT_TYPE_ALIGN_UP		  : return margin[2]; break;
      case UI_ELEMENT_TYPE_ALIGN_DOWN		: return margin[3]; break;
    }
  
  return 0.0f;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetMargin(UI_ELEMENT_TYPE_ALIGN position, double value)
* @brief      Set margin
* @ingroup    USERINTERFACE
* 
* @param[in]  position : Position value.
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetMargin(UI_ELEMENT_TYPE_ALIGN position, double value)
{
  switch((int)position)
    {
      case UI_ELEMENT_TYPE_ALIGN_LEFT   : margin[0] = value; break;
      case UI_ELEMENT_TYPE_ALIGN_RIGHT	: margin[1] = value; break;
      case UI_ELEMENT_TYPE_ALIGN_UP		  : margin[2] = value; break;
      case UI_ELEMENT_TYPE_ALIGN_DOWN		: margin[3] = value; break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT::GetPadding(UI_ELEMENT_TYPE_ALIGN position)
* @brief      Get padding along one side (LEFT/RIGHT/UP/DOWN). Zero on unset elements.
* @ingroup    USERINTERFACE
*
* @param[in]  position : Side to query.
*
* @return     double : Requested value in pixels.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetPadding(UI_ELEMENT_TYPE_ALIGN position)
{
  switch((int)position)
    {
      case UI_ELEMENT_TYPE_ALIGN_LEFT   : return padding[0]; break;
      case UI_ELEMENT_TYPE_ALIGN_RIGHT  : return padding[1]; break;
      case UI_ELEMENT_TYPE_ALIGN_UP     : return padding[2]; break;
      case UI_ELEMENT_TYPE_ALIGN_DOWN   : return padding[3]; break;
    }

  return 0.0f;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetPadding(UI_ELEMENT_TYPE_ALIGN position, double value)
* @brief      Set padding along one side.
* @ingroup    USERINTERFACE
*
* @param[in]  position : Side to set.
* @param[in]  value : Value in pixels.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetPadding(UI_ELEMENT_TYPE_ALIGN position, double value)
{
  switch((int)position)
    {
      case UI_ELEMENT_TYPE_ALIGN_LEFT   : padding[0] = value; break;
      case UI_ELEMENT_TYPE_ALIGN_RIGHT  : padding[1] = value; break;
      case UI_ELEMENT_TYPE_ALIGN_UP     : padding[2] = value; break;
      case UI_ELEMENT_TYPE_ALIGN_DOWN   : padding[3] = value; break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT::GetBorderWidth()
* @brief      Get border stroke width in pixels. -1 means "use skin default" (historically 1.0 for containers);
*             0 means "no border drawn"; positive values are honoured verbatim.
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetBorderWidth()
{
  return border_width;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetBorderWidth(double borderwidth)
* @brief      Set border stroke width.
* @ingroup    USERINTERFACE
*
* @param[in]  borderwidth : Border width in pixels (-1 = default, 0 = none, >0 = px).
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetBorderWidth(double borderwidth)
{
  border_width = borderwidth;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_COLOR* UI_ELEMENT::GetBorderColor()
* @brief      Get border colour (base-level equivalent of the per-type linecolor). Consumers should check
*             IsBorderColorSet() before using: an unset border_color is left at UI_COLOR's default state and
*             consumers must fall back to the per-type linecolor to preserve pre-step-5 rendering.
* @ingroup    USERINTERFACE
*
* @return     UI_COLOR* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_COLOR* UI_ELEMENT::GetBorderColor()
{
  return &border_color;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT::IsBorderColorSet()
* @brief      True iff border_color was explicitly authored via XML/CSS. Discriminates a valid transparent
*             authored value (rgba 0,0,0,0) from the "default, use fallback" state.
* @ingroup    USERINTERFACE
*
* @return     bool : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::IsBorderColorSet()
{
  return border_color_set;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetBorderColorFromString(XCHAR* string)
* @brief      Set border colour from an authored string ("red", "#RRGGBBAA", "R,G,B[,A]"). Marks as authored.
* @ingroup    USERINTERFACE
*
* @param[in]  string : Colour string.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetBorderColorFromString(XCHAR* string)
{
  if(!string) return;

  border_color.SetFromString(string);
  border_color_set = true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetBorderColorFromString(XSTRING& string)
* @brief      Convenience overload; forwards to SetBorderColorFromString(XCHAR*).
* @ingroup    USERINTERFACE
*
* @param[in]  string : Colour string.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetBorderColorFromString(XSTRING& string)
{
  SetBorderColorFromString(string.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT::GetBorderRadius(UI_ELEMENT_BORDER_CORNER corner)
* @brief      Get raw per-corner border radius. Value -1 means "not authored". Use GetEffectiveBorderRadius()
*             for the resolved value with fallback to the element's uniform roundrect.
* @ingroup    USERINTERFACE
*
* @param[in]  corner : Corner index.
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetBorderRadius(UI_ELEMENT_BORDER_CORNER corner)
{
  if((corner < 0) || (corner >= UI_ELEMENT_BORDER_CORNER_MAX)) return -1.0;
  return border_radius[corner];
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetBorderRadius(UI_ELEMENT_BORDER_CORNER corner, double value)
* @brief      Set per-corner border radius (in pixels). Pass -1 to un-author a corner and re-enable the fallback
*             to the element's uniform roundrect.
* @ingroup    USERINTERFACE
*
* @param[in]  corner : Corner index.
* @param[in]  value : Radius in pixels, or -1 to clear.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetBorderRadius(UI_ELEMENT_BORDER_CORNER corner, double value)
{
  if((corner < 0) || (corner >= UI_ELEMENT_BORDER_CORNER_MAX)) return;
  border_radius[corner] = value;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT::GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER corner)
* @brief      Resolved per-corner radius: the authored value if >= 0, otherwise the element's uniform roundrect.
*             Never returns a negative value; a corner with no radius data and no roundrect returns 0.
* @ingroup    USERINTERFACE
*
* @param[in]  corner : Corner index.
*
* @return     double : Requested value in pixels.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetEffectiveBorderRadius(UI_ELEMENT_BORDER_CORNER corner)
{
  double raw = GetBorderRadius(corner);
  if(raw >= 0.0) return raw;

  return (double)roundrect;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT::HasAnyPerCornerRadius()
* @brief      True iff at least one corner carries an authored per-corner radius. Used by the skin as a
*             fast-path discriminator: when false, the classic single-radius canvas->RoundRect call is enough;
*             when true, the skin must build a custom path with per-corner arcs.
* @ingroup    USERINTERFACE
*
* @return     bool : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::HasAnyPerCornerRadius()
{
  for(int c=0; c<UI_ELEMENT_BORDER_CORNER_MAX; c++)
    {
      if(border_radius[c] >= 0.0) return true;
    }
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT::IsActive()
* @brief      Is active
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::IsActive()
{
  return isactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetActive(bool isactive)
* @brief      Set active
* @ingroup    USERINTERFACE
* 
* @param[in]  isactive : Isactive value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetActive(bool isactive)
{
  bool changed = (this->isactive != isactive);

  this->isactive = isactive;

  // State transition -> re-run CSS cascade with the new active-pseudo set. Cheap no-op when the element is not
  // targeted by any pseudo-carrying rule (guarded inside ReapplyStyleVisual by style_has_state_rules).
  if(changed) ReapplyStyleVisual();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::IsPreSelect()
* @brief      Is pre select
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::IsPreSelect()
{
  return ispreselect;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::SetPreSelect(bool ispreselect)
* @brief      Set pre select
* @note       Returns whether the state actually flipped, so a caller that ALSO needs to decide whether to
*             invalidate/redraw (a different concern than the CSS re-resolve done here) does not have to
*             duplicate this same "did it change" comparison -- see UI_MANAGER::PreSelectElement(), which used
*             to call Elements_SetToRedraw() unconditionally on every hovered tick even when nothing changed.
* @ingroup    USERINTERFACE
*
* @param[in]  ispreselect : Ispreselect value.
*
* @return     bool : true if the pre-select state actually changed; false if it was already "ispreselect".
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::SetPreSelect(bool ispreselect)
{
  bool changed = (this->ispreselect != ispreselect);

  this->ispreselect = ispreselect;

  if(changed) ReapplyStyleVisual();

  return changed;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::IsSelected()
* @brief      Is selected
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::IsSelected()
{  
  return isselected;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetSelected(bool isselected)
* @brief      Set selected
* @ingroup    USERINTERFACE
* 
* @param[in]  isselected : Isselected value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetSelected(bool isselected)
{
  bool changed = (this->isselected != isselected);

  this->isselected = isselected;

  if(changed)
    {
      ReapplyStyleVisual();

      // Descendants may match ancestor :selected combinators (e.g. form.nav-row:selected .nav-label).
      // Only walk when a stylesheet is present -- XML-only layouts stay no-op.
      if(element_layout && element_layout->GetStyleSheet())
        {
          XVECTOR<UI_ELEMENT*>* children = GetComposeElements();
          if(children)
            {
              for(XDWORD c=0; c<children->GetSize(); c++)
                {
                  UI_ELEMENT* child = children->Get(c);
                  if(child) child->ReapplyStyleVisualRecursive();
                }
            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::IsVisible()
* @brief      Is visible
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::IsVisible()
{
  return isvisible;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetVisible(bool isvisible)
* @brief      Set visible
* @ingroup    USERINTERFACE
* 
* @param[in]  isvisible : Isvisible value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetVisible(bool isvisible)
{
  if(this->isvisible != isvisible) 
    {      
      GEN_USERINTERFACE.Elements_SetToRedraw(this);
       
      SetTransitionStateShow(isvisible?UI_ELEMENT_TRANSITION_STATE_SHOW_TOSHOW:UI_ELEMENT_TRANSITION_STATE_SHOW_TOHIDE);
      this->isvisible = isvisible;      
    } 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_ELEMENT::GetRoundRect()
* @brief      Get round rect
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_ELEMENT::GetRoundRect()
{
  return roundrect;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetRoundRect(XDWORD roundrect)
* @brief      Set round rect
* @ingroup    USERINTERFACE
* 
* @param[in]  roundrect : Roundrect value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetRoundRect(XDWORD roundrect)
{
  this->roundrect = roundrect;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_ELEMENT::IsBlinking()
* @brief      Is blinking
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_ELEMENT::IsBlinking()
{
  return blink_time;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::SetBlink(XDWORD blink_time)
* @brief      Set blink
* @ingroup    USERINTERFACE
* 
* @param[in]  blink_time : Blink time value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::SetBlink(XDWORD blink_time)
{
  if(!blink_xtimer) return false;

  this->blink_time = blink_time; 

  blink_state = true;
  
  blink_xtimer->Reset();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::GetStateBlink()
* @brief      Get state blink
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::GetStateBlink()
{
  return blink_state;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::SwitchStateBlink()
* @brief      Swicth state blink
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::SwitchStateBlink()
{
  if(!blink_nchanges)
    {
      blink_state = true;
      blink_nchanges++;

      return true;
    }

  blink_state = !blink_state;  

  blink_nchanges++;

  blink_xtimer->Reset();

  return true;
}

	
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* UI_ELEMENT::GetTimerBlink()
* @brief      Get timer blink
* @ingroup    USERINTERFACE
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* UI_ELEMENT::GetTimerBlink()
{
  return blink_xtimer;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD UI_ELEMENT::GetTransitionDuration()
* @brief      Get the "transition" duration in milliseconds (Step 7). 0 means disabled: ReapplyStyleVisual()
*             jumps color/bckgrdcolor instantly on a state change, exactly as before this feature existed.
* @ingroup    USERINTERFACE
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_ELEMENT::GetTransitionDuration()
{
  return style_transition_duration;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT::SetTransitionDuration(XDWORD milliseconds)
* @brief      Set the "transition" duration in milliseconds (Step 7).
* @ingroup    USERINTERFACE
*
* @param[in]  milliseconds : Duration value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::SetTransitionDuration(XDWORD milliseconds)
{
  style_transition_duration = milliseconds;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT::IsTransitioning()
* @brief      True while a color/background-color tween started by ReapplyStyleVisual() is still in flight.
*             Polled by UI_SKIN::Draw() every frame, exactly like IsBlinking()/GetStateBlink() above.
* @ingroup    USERINTERFACE
*
* @return     bool : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::IsTransitioning()
{
  return style_transition_active;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::UpdateTransition()
* @brief      Advance the in-flight color/background-color tween by however much time has elapsed since
*             ReapplyStyleVisual() started it, and write the interpolated values into the live color/
*             backgroundcolor members that Draw_Form/Draw_Text already read every frame. Ends the transition
*             (snapping exactly to the target, so it never gets stuck short of it due to frame timing) once
*             the configured duration has elapsed.
* @note       Called only while IsTransitioning() is true; no-op guards are still kept defensively.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::UpdateTransition()
{
  if(!style_transition_active) return;
  if(!style_transition_xtimer) { style_transition_active = false; return; }

  double elapsedms = (double)style_transition_xtimer->GetMeasureMilliSeconds();
  double t         = style_transition_duration ? (elapsedms / (double)style_transition_duration) : 1.0;

  if(t >= 1.0)
    {
      t = 1.0;
      style_transition_active = false;
    }

  color.SetRed  (style_transition_color_from.GetRed() + (int)((style_transition_color_to.GetRed() - style_transition_color_from.GetRed()) * t));
  color.SetGreen(style_transition_color_from.GetGreen() + (int)((style_transition_color_to.GetGreen() - style_transition_color_from.GetGreen()) * t));
  color.SetBlue (style_transition_color_from.GetBlue() + (int)((style_transition_color_to.GetBlue() - style_transition_color_from.GetBlue()) * t));
  color.SetAlpha(style_transition_color_from.GetAlpha() + (int)((style_transition_color_to.GetAlpha() - style_transition_color_from.GetAlpha()) * t));

  backgroundcolor.SetRed  (style_transition_backgroundcolor_from.GetRed() + (int)((style_transition_backgroundcolor_to.GetRed() - style_transition_backgroundcolor_from.GetRed()) * t));
  backgroundcolor.SetGreen(style_transition_backgroundcolor_from.GetGreen() + (int)((style_transition_backgroundcolor_to.GetGreen() - style_transition_backgroundcolor_from.GetGreen()) * t));
  backgroundcolor.SetBlue (style_transition_backgroundcolor_from.GetBlue() + (int)((style_transition_backgroundcolor_to.GetBlue() - style_transition_backgroundcolor_from.GetBlue()) * t));
  backgroundcolor.SetAlpha(style_transition_backgroundcolor_from.GetAlpha() + (int)((style_transition_backgroundcolor_to.GetAlpha() - style_transition_backgroundcolor_from.GetAlpha()) * t));

  mustredraw = true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_ELEMENT_TRANSITION_STATE_SHOW UI_ELEMENT::GetTransitionStateShow()
* @brief      Get transition state show
* @ingroup    USERINTERFACE
* 
* @return     UI_ELEMENT_TRANSITION_STATE_SHOW : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_TRANSITION_STATE_SHOW UI_ELEMENT::GetTransitionStateShow()
{
  return transitionstateshow;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetTransitionStateShow(UI_ELEMENT_TRANSITION_STATE_SHOW transitionstateshow)
* @brief      Set transition state show
* @ingroup    USERINTERFACE
* 
* @param[in]  transitionstateshow : Transitionstateshow value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetTransitionStateShow(UI_ELEMENT_TRANSITION_STATE_SHOW transitionstateshow)
{
  this->transitionstateshow = transitionstateshow;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::HasScroll()
* @brief      Has scroll
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::HasScroll()
{
  return hasscroll;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetHasScroll(bool hasscroll)
* @brief      Set has scroll
* @ingroup    USERINTERFACE
* 
* @param[in]  hasscroll : Hasscroll value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetHasScroll(bool hasscroll)
{
  this->hasscroll = hasscroll;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* UI_ELEMENT::GetExtra()
* @brief      Get extra
* @ingroup    USERINTERFACE
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* UI_ELEMENT::GetExtra()
{
  return &extra;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::MustReDraw()
* @brief      Must re draw
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::MustReDraw()
{
  return mustredraw;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetMustReDraw(bool mustredraw)
* @brief      Set must re draw
* @ingroup    USERINTERFACE
* 
* @param[in]  mustredraw : Mustredraw value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetMustReDraw(bool mustredraw)
{
  this->mustredraw = mustredraw;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<UI_ELEMENT*>* UI_ELEMENT::GetComposeElements()
* @brief      Get compose elements
* @ingroup    USERINTERFACE
* 
* @return     XVECTOR<UI_ELEMENT*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<UI_ELEMENT*>* UI_ELEMENT::GetComposeElements()
{
  return &compose_elements;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::DeleteAllComposeElements()
* @brief      Delete all compose elements
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::DeleteAllComposeElements()
{
  if(compose_elements.IsEmpty()) return false;

  compose_elements.DeleteContents();
  compose_elements.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT::IsFlexContainer()
* @brief      Is flex container
* @ingroup    USERINTERFACE
*
* @return     bool : true if this element is a flex container (CSS "display: flex"); otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::IsFlexContainer()
{
  return css_flexcontainer;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetFlexContainer(bool isflexcontainer)
* @brief      Set flex container
* @ingroup    USERINTERFACE
*
* @param[in]  isflexcontainer : Flex container value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetFlexContainer(bool isflexcontainer)
{
  css_flexcontainer = isflexcontainer;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_FLEX_DIRECTION UI_ELEMENT::GetFlexDirection()
* @brief      Get flex direction
* @ingroup    USERINTERFACE
*
* @return     UI_FLEX_DIRECTION : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_FLEX_DIRECTION UI_ELEMENT::GetFlexDirection()
{
  return css_flexdirection;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetFlexDirection(UI_FLEX_DIRECTION flexdirection)
* @brief      Set flex direction
* @ingroup    USERINTERFACE
*
* @param[in]  flexdirection : Flex direction value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetFlexDirection(UI_FLEX_DIRECTION flexdirection)
{
  css_flexdirection = flexdirection;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_JUSTIFY_CONTENT UI_ELEMENT::GetJustifyContent()
* @brief      Get justify content
* @ingroup    USERINTERFACE
*
* @return     UI_JUSTIFY_CONTENT : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_JUSTIFY_CONTENT UI_ELEMENT::GetJustifyContent()
{
  return css_justifycontent;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetJustifyContent(UI_JUSTIFY_CONTENT justifycontent)
* @brief      Set justify content
* @ingroup    USERINTERFACE
*
* @param[in]  justifycontent : Justify content value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetJustifyContent(UI_JUSTIFY_CONTENT justifycontent)
{
  css_justifycontent = justifycontent;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT::GetRowGap()
* @brief      Get row gap
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetRowGap()
{
  return css_rowgap;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT::GetColumnGap()
* @brief      Get column gap
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetColumnGap()
{
  return css_columngap;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetGap(double rowgap, double columngap)
* @brief      Set row/column gap
* @ingroup    USERINTERFACE
*
* @param[in]  rowgap : CSS "row-gap" value.
* @param[in]  columngap : CSS "column-gap" value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetGap(double rowgap, double columngap)
{
  css_rowgap    = rowgap;
  css_columngap = columngap;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_FLEX_WRAP UI_ELEMENT::GetFlexWrap()
* @brief      Get flex wrap
* @ingroup    USERINTERFACE
*
* @return     UI_FLEX_WRAP : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_FLEX_WRAP UI_ELEMENT::GetFlexWrap()
{
  return css_flexwrap;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetFlexWrap(UI_FLEX_WRAP flexwrap)
* @brief      Set flex wrap
* @ingroup    USERINTERFACE
*
* @param[in]  flexwrap : Flex wrap value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetFlexWrap(UI_FLEX_WRAP flexwrap)
{
  css_flexwrap = flexwrap;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ALIGN_CONTENT UI_ELEMENT::GetAlignContent()
* @brief      Get align content
* @ingroup    USERINTERFACE
*
* @return     UI_ALIGN_CONTENT : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_ALIGN_CONTENT UI_ELEMENT::GetAlignContent()
{
  return css_aligncontent;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetAlignContent(UI_ALIGN_CONTENT aligncontent)
* @brief      Set align content
* @ingroup    USERINTERFACE
*
* @param[in]  aligncontent : Align content value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetAlignContent(UI_ALIGN_CONTENT aligncontent)
{
  css_aligncontent = aligncontent;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ALIGN_ITEMS UI_ELEMENT::GetAlignItems()
* @brief      Get align items
* @ingroup    USERINTERFACE
*
* @return     UI_ALIGN_ITEMS : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_ALIGN_ITEMS UI_ELEMENT::GetAlignItems()
{
  return css_alignitems;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetAlignItems(UI_ALIGN_ITEMS alignitems)
* @brief      Set align items
* @ingroup    USERINTERFACE
*
* @param[in]  alignitems : Align items value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetAlignItems(UI_ALIGN_ITEMS alignitems)
{
  css_alignitems = alignitems;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT::GetFlexGrow()
* @brief      Get flex grow
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetFlexGrow()
{
  return css_flexgrow;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetFlexGrow(double flexgrow)
* @brief      Set flex grow
* @ingroup    USERINTERFACE
*
* @param[in]  flexgrow : Flex grow value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetFlexGrow(double flexgrow)
{
  css_flexgrow = flexgrow;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT::GetFlexShrink()
* @brief      Get flex shrink
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetFlexShrink()
{
  return css_flexshrink;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetFlexShrink(double flexshrink)
* @brief      Set flex shrink
* @ingroup    USERINTERFACE
*
* @param[in]  flexshrink : Flex shrink value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetFlexShrink(double flexshrink)
{
  css_flexshrink = flexshrink;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LAYOUTBOX_INSET UI_ELEMENT::GetFlexBasis()
* @brief      Get flex basis
* @ingroup    USERINTERFACE
*
* @return     UI_LAYOUTBOX_INSET : Requested value ("specified" false means "auto").
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUTBOX_INSET UI_ELEMENT::GetFlexBasis()
{
  return css_flexbasis;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetFlexBasisAuto()
* @brief      Set flex basis to "auto"
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetFlexBasisAuto()
{
  css_flexbasis.specified = false;
  css_flexbasis.value     = 0.0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetFlexBasis(double value)
* @brief      Set flex basis to an explicit value
* @ingroup    USERINTERFACE
*
* @param[in]  value : Flex basis value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetFlexBasis(double value)
{
  css_flexbasis.specified = true;
  css_flexbasis.value     = value;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ALIGN_SELF UI_ELEMENT::GetAlignSelf()
* @brief      Get align self
* @ingroup    USERINTERFACE
*
* @return     UI_ALIGN_SELF : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_ALIGN_SELF UI_ELEMENT::GetAlignSelf()
{
  return css_alignself;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetAlignSelf(UI_ALIGN_SELF alignself)
* @brief      Set align self
* @ingroup    USERINTERFACE
*
* @param[in]  alignself : Align self value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetAlignSelf(UI_ALIGN_SELF alignself)
{
  css_alignself = alignself;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT::GetIntrinsicWidth()
* @brief      Get intrinsic width
* @ingroup    USERINTERFACE
*
* @return     double : The width UI_SKIN::CalculateBoundaryLine() last measured/resolved for this element (its
*                       authored size for a fixed-size widget, or its freshly-measured natural size for auto-sized
*                       content such as text) -- -1.0 if CalculateBoundaryLine() has never run for it, in which case
*                       the caller should fall back to GetBoundaryLine()->width. See UI_Element.h's own comment.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetIntrinsicWidth()
{
  return intrinsic_width;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetIntrinsicWidth(double intrinsicwidth)
* @brief      Set intrinsic width
* @ingroup    USERINTERFACE
*
* @param[in]  intrinsicwidth : Intrinsic width value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetIntrinsicWidth(double intrinsicwidth)
{
  intrinsic_width = intrinsicwidth;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT::GetIntrinsicHeight()
* @brief      Get intrinsic height
* @ingroup    USERINTERFACE
*
* @return     double : Same rule as GetIntrinsicWidth(), for the vertical axis.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT::GetIntrinsicHeight()
{
  return intrinsic_height;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetIntrinsicHeight(double intrinsicheight)
* @brief      Set intrinsic height
* @ingroup    USERINTERFACE
*
* @param[in]  intrinsicheight : Intrinsic height value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetIntrinsicHeight(double intrinsicheight)
{
  intrinsic_height = intrinsicheight;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::GetActivePseudos(XVECTOR<XSTRING*>& out)
* @brief      Populates `out` with the pseudo-class names implied by the element's current live state.
*             Strings are heap-allocated with GEN_NEW; the caller owns them and must delete them.
* @note       "hover" is emitted as a synonym of "preselect": GEN's preselect state IS mouse-over (it is set
*             from UI_MANAGER::PreSelectElement(), which tests the live cursor position against the element's
*             box), so ":hover" and ":preselect" match exactly the same live state and either spelling can be
*             used in a stylesheet -- authors coming from CSS can write the familiar ":hover", existing rules
*             written against ":preselect" keep working unchanged.
* @note       ":active" is a DELIBERATE divergence from CSS. Here it mirrors GEN's own `isactive`
*             ("this element is enabled / not disabled"), not CSS's `:active` ("the pointer is currently
*             pressed on this element"). Phase 4 exposes ":pressed" for pointer-down (SetPressed), keeping
*             ":active"/":disabled" as the enabled/disabled pair authors already use.
* @ingroup    USERINTERFACE
*
* @param[out] out : Vector to append pseudo names into.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::GetActivePseudos(XVECTOR<XSTRING*>& out)
{
  if(ispreselect)
    {
      XSTRING* p1 = GEN_NEW XSTRING(); if(p1) { p1->Set(__L("preselect")); out.Add(p1); }
      XSTRING* p2 = GEN_NEW XSTRING(); if(p2) { p2->Set(__L("hover"));     out.Add(p2); }
    }

  if(isselected)
    {
      XSTRING* p = GEN_NEW XSTRING(); if(p) { p->Set(__L("selected"));  out.Add(p); }
    }

  if(isactive)
    {
      XSTRING* p = GEN_NEW XSTRING(); if(p) { p->Set(__L("active"));    out.Add(p); }
    }
   else
    {
      XSTRING* p = GEN_NEW XSTRING(); if(p) { p->Set(__L("disabled"));  out.Add(p); }
    }

  // Phase 4: ":pressed" is the CSS-like pointer-down pseudo; ":active" stays GEN enabled (above).
  if(ispressed)
    {
      XSTRING* p = GEN_NEW XSTRING(); if(p) { p->Set(__L("pressed"));   out.Add(p); }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SnapshotStyleVisual()
* @brief      Captures color / bckgrdcolor / roundrect as the baseline restored by every ReapplyStyleVisual() call.
*             UI_MANAGER calls this once at end-of-load, after XML and stateless CSS have populated the element.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SnapshotStyleVisual()
{
  color          .CopyTo(&snapshot_color);
  backgroundcolor.CopyTo(&snapshot_backgroundcolor);
  snapshot_roundrect = roundrect;

  snapshot_border_width     = border_width;
  border_color.CopyTo(&snapshot_border_color);
  snapshot_border_color_set = border_color_set;

  for(int c=0; c<UI_ELEMENT_BORDER_CORNER_MAX; c++) snapshot_border_radius[c] = border_radius[c];

  snapshot_box_shadow_set  = box_shadow_set;
  snapshot_shadow_offset_x = shadow_offset_x;
  snapshot_shadow_offset_y = shadow_offset_y;
  snapshot_shadow_blur     = shadow_blur;
  shadow_color.CopyTo(&snapshot_shadow_color);

  snapshot_taken     = true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::ReapplyStyleVisual()
* @brief      Restore the visual baseline and layer the active stylesheet's rules on top, using the current
*             pseudo-class set derived from state. No-op when no stylesheet is loaded, no snapshot was taken,
*             or the loader marked this element as unaffected by pseudo rules.
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::ReapplyStyleVisual()
{
  if(!style_has_state_rules) return;
  if(!snapshot_taken)        return;

  // Phase 1 ownership step: the stylesheet belongs to THIS element's own UI_LAYOUT (set once at build time by
  // UI_MANAGER::GetLayoutElement_Base(), see SetLayout()), not to a single UI_MANAGER-wide pointer shared by
  // every currently-loaded layout -- otherwise loading a second, unrelated layout's XML would silently swap
  // (and free) the stylesheet every pre-existing element on screen resolves against.
  UI_STYLESHEET* sheet = element_layout ? element_layout->GetStyleSheet() : NULL;

  if(!sheet) return;

  // Resolve the TARGET values into local copies, starting from the stateless baseline so a key the current
  // cascade does not touch still falls back to the authored (XML + stateless CSS) value instead of whatever
  // the previous state left on screen. This used to be applied straight into color/backgroundcolor; now it is
  // kept separate so a "transition" duration (Step 7) can animate towards it instead of jumping to it.
  UI_COLOR targetcolor;           snapshot_color          .CopyTo(&targetcolor);
  UI_COLOR targetbackgroundcolor; snapshot_backgroundcolor.CopyTo(&targetbackgroundcolor);
  XDWORD   targetroundrect = snapshot_roundrect;

  double   targetborderwidth = snapshot_border_width;
  UI_COLOR targetbordercolor;    snapshot_border_color.CopyTo(&targetbordercolor);
  bool     targetbordercolorset = snapshot_border_color_set;

  double   targetborderradius[UI_ELEMENT_BORDER_CORNER_MAX];
  for(int c=0; c<UI_ELEMENT_BORDER_CORNER_MAX; c++) targetborderradius[c] = snapshot_border_radius[c];

  bool     targetboxshadowset = snapshot_box_shadow_set;
  double   targetshadowoffsetx = snapshot_shadow_offset_x;
  double   targetshadowoffsety = snapshot_shadow_offset_y;
  double   targetshadowblur    = snapshot_shadow_blur;
  UI_COLOR targetshadowcolor;    snapshot_shadow_color.CopyTo(&targetshadowcolor);

  // Re-resolve. FillFromCSSDeclarations internally builds the active-pseudo list from the element's live state.
  UI_STYLE bag;
  bag.FillFromCSSDeclarations(sheet, this);

  // Apply the base-level visual keys that a pseudo-class rule can legitimately restyle without triggering a
  // re-layout (color/background-color/roundrect, already handled since Step 6/"transiciones"; border-width/
  // -color/-radius and box-shadow, added here). Per-type keys (linecolor, gradientcolor, thickness, ...) stay
  // out of scope -- they live on subclasses this base-class method has no knowledge of, and would need virtual
  // dispatch to reach; box-model/geometry keys (xpos/ypos/width/height/margin/padding/direction) stay out of
  // scope too -- swapping those per pseudo-class state would require a full re-layout pass, not just a redraw,
  // which is a materially bigger change than this step.
  XSTRING v;
  double  d;

  if(bag.Get(__L("color")      , v))  targetcolor          .SetFromString(v);

  // "bckgrdcolor" or "background-color": same shared first-hit alias lookup as GetLayoutElement_Base() at load
  // time (UI_Manager.cpp) -- both now go through UI_PROPERTYREGISTRY::GetAliased(), so a rule written with the
  // CSS-natural name (e.g. ":hover { background-color: ... }") is honored here too instead of freezing at the
  // stateless baseline, and the two call sites cannot silently drift onto different precedence.
  XSTRING bckgrdcolor;
  UI_PROPERTYREGISTRY::GetAliased(bag, __L("bckgrdcolor"), __L("background-color"), bckgrdcolor);
  if(!bckgrdcolor.IsEmpty()) targetbackgroundcolor.SetFromString(bckgrdcolor);

  if(bag.Get(__L("roundrect")  , d))  targetroundrect = (XDWORD)d;

  if(bag.Get(__L("border-width"), d)) targetborderwidth = d;

  XSTRING bordercolorstr;
  if(bag.Get(__L("border-color"), bordercolorstr) && !bordercolorstr.IsEmpty())
    {
      targetbordercolor.SetFromString(bordercolorstr);
      targetbordercolorset = true;
    }

  // border-radius shorthand (1-4 values, same CSS positional rule as load time) then per-corner longhands,
  // which override the shorthand -- identical precedence to GetLayoutElement_Base().
  XSTRING borderradiusstr;
  if(bag.Get(__L("border-radius"), borderradiusstr))
    {
      double out[4] = { 0.0, 0.0, 0.0, 0.0 };
      UI_PROPERTYREGISTRY::ExpandCSSShorthand4(borderradiusstr, out);            // out = TL, TR, BR, BL

      targetborderradius[UI_ELEMENT_BORDER_CORNER_TL] = out[0];
      targetborderradius[UI_ELEMENT_BORDER_CORNER_TR] = out[1];
      targetborderradius[UI_ELEMENT_BORDER_CORNER_BR] = out[2];
      targetborderradius[UI_ELEMENT_BORDER_CORNER_BL] = out[3];
    }

  if(bag.Get(__L("border-top-left-radius")     , d)) targetborderradius[UI_ELEMENT_BORDER_CORNER_TL] = d;
  if(bag.Get(__L("border-top-right-radius")    , d)) targetborderradius[UI_ELEMENT_BORDER_CORNER_TR] = d;
  if(bag.Get(__L("border-bottom-right-radius") , d)) targetborderradius[UI_ELEMENT_BORDER_CORNER_BR] = d;
  if(bag.Get(__L("border-bottom-left-radius")  , d)) targetborderradius[UI_ELEMENT_BORDER_CORNER_BL] = d;

  XSTRING boxshadowstr;
  if(bag.Get(__L("box-shadow"), boxshadowstr) && !boxshadowstr.IsEmpty())
    {
      double  sh_x    = 0.0;
      double  sh_y    = 0.0;
      double  sh_blur = 0.0;
      XSTRING sh_color;

      if(UI_PROPERTYREGISTRY::ParseBoxShadow(boxshadowstr, sh_x, sh_y, sh_blur, sh_color))
        {
          targetshadowoffsetx = sh_x;
          targetshadowoffsety = sh_y;
          targetshadowblur    = sh_blur;
          targetshadowcolor.SetFromString(sh_color);
          targetboxshadowset  = true;
        }
    }

  // roundrect/border-width/border-color/border-radius/box-shadow are discrete/structural values, not ones that
  // can be usefully interpolated the way a colour can, so they always jump immediately regardless of
  // "transition". Only color/background-color -- the two keys this method has ever tweened -- are eligible.
  roundrect = targetroundrect;

  border_width     = targetborderwidth;
  targetbordercolor.CopyTo(&border_color);
  border_color_set = targetbordercolorset;

  for(int c=0; c<UI_ELEMENT_BORDER_CORNER_MAX; c++) border_radius[c] = targetborderradius[c];

  // Uniform CSS border-radius keeps roundrect in sync so Draw_Form can use AGG rounded_rect (no Path miters).
  if((border_radius[0] >= 0.0) &&
     (border_radius[0] == border_radius[1]) && (border_radius[1] == border_radius[2]) &&
     (border_radius[2] == border_radius[3]) && (border_radius[0] > 0.0))
    {
      roundrect = (XDWORD)border_radius[0];
    }

  box_shadow_set  = targetboxshadowset;
  shadow_offset_x = targetshadowoffsetx;
  shadow_offset_y = targetshadowoffsety;
  shadow_blur     = targetshadowblur;
  targetshadowcolor.CopyTo(&shadow_color);

  if(style_transition_duration)
    {
      // Animate FROM whatever is currently on screen (which may itself still be mid-transition, e.g. a fast
      // hover-in immediately followed by a hover-out) TO the newly resolved target, over style_transition_
      // duration ms. UpdateTransition(), polled every frame from UI_SKIN::Draw() exactly like the blink timer
      // above, does the actual interpolation.
      style_transition_color_from = color;
      style_transition_color_to   = targetcolor;

      style_transition_backgroundcolor_from = backgroundcolor;
      style_transition_backgroundcolor_to   = targetbackgroundcolor;

      style_transition_xtimer->Reset();
      style_transition_active = true;
    }
   else
    {
      color           = targetcolor;
      backgroundcolor = targetbackgroundcolor;
    }

  mustredraw = true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::ReapplyStyleVisualRecursive()
* @brief      ReapplyStyleVisual on this element then every compose descendant (ancestor :selected combinators).
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::ReapplyStyleVisualRecursive()
{
  ReapplyStyleVisual();

  XVECTOR<UI_ELEMENT*>* children = GetComposeElements();
  if(!children) return;

  for(XDWORD c=0; c<children->GetSize(); c++)
    {
      UI_ELEMENT* child = children->Get(c);
      if(child) child->ReapplyStyleVisualRecursive();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT::GetStyleHasStateRules()
* @brief      True iff at least one pseudo-carrying rule in the active stylesheet targets this element.
* @ingroup    USERINTERFACE
*
* @return     bool : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::GetStyleHasStateRules()
{
  return style_has_state_rules;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::SetStyleHasStateRules(bool has)
* @brief      Marker set by UI_MANAGER at load time; enables the ReapplyStyleVisual() fast path.
* @ingroup    USERINTERFACE
*
* @param[in]  has : Value to set.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetStyleHasStateRules(bool has)
{
  style_has_state_rules = has;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UI_STYLE* UI_ELEMENT::GetComputedStyle()
* @brief      Get Computed Style
* @ingroup    USERINTERFACE
* 
* @return     UI_STYLE* : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
UI_STYLE* UI_ELEMENT::GetComputedStyle()
{
  return computed_style;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::StoreComputedStyle(UI_STYLE& style)
* @brief      Store Computed Style
* @ingroup    USERINTERFACE
* 
* @param[in]  style : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::StoreComputedStyle(UI_STYLE& style)
{
  if(computed_style)
    {
      GEN_DELETE computed_style;
      computed_style = NULL;
    }

  computed_style = GEN_NEW UI_STYLE();
  if(!computed_style) return;

  XVECTOR<UI_STYLEPROPERTY*>* props = style.GetProperties();
  if(!props) return;

  for(XDWORD c=0; c<props->GetSize(); c++)
    {
      UI_STYLEPROPERTY* prop = props->Get(c);
      if(prop) computed_style->Set(prop->GetKey().Get(), prop->GetValue());
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::IsGridContainer()
* @brief      Is Grid Container
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::IsGridContainer()
{
  return css_gridcontainer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetGridContainer(bool isgridcontainer)
* @brief      Set Grid Container
* @ingroup    USERINTERFACE
* 
* @param[in]  isgridcontainer : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetGridContainer(bool isgridcontainer)
{
  css_gridcontainer = isgridcontainer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::ClearGridColumnTracks()
* @brief      Clear Grid Column Tracks
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::ClearGridColumnTracks()
{
  css_gridcolumntracks.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::AddGridColumnTrack(UI_GRIDTRACK track)
* @brief      Add Grid Column Track
* @ingroup    USERINTERFACE
* 
* @param[in]  track : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::AddGridColumnTrack(UI_GRIDTRACK track)
{
  css_gridcolumntracks.Add(track);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<UI_GRIDTRACK>& UI_ELEMENT::GetGridColumnTracks()
* @brief      Get Grid Column Tracks
* @ingroup    USERINTERFACE
* 
* @return     XVECTOR<UI_GRIDTRACK>& : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<UI_GRIDTRACK>& UI_ELEMENT::GetGridColumnTracks()
{
  return css_gridcolumntracks;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::ClearGridRowTracks()
* @brief      Clear Grid Row Tracks
* @ingroup    USERINTERFACE
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::ClearGridRowTracks()
{
  css_gridrowtracks.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::AddGridRowTrack(UI_GRIDTRACK track)
* @brief      Add Grid Row Track
* @ingroup    USERINTERFACE
* 
* @param[in]  track : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::AddGridRowTrack(UI_GRIDTRACK track)
{
  css_gridrowtracks.Add(track);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<UI_GRIDTRACK>& UI_ELEMENT::GetGridRowTracks()
* @brief      Get Grid Row Tracks
* @ingroup    USERINTERFACE
* 
* @return     XVECTOR<UI_GRIDTRACK>& : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<UI_GRIDTRACK>& UI_ELEMENT::GetGridRowTracks()
{
  return css_gridrowtracks;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_ELEMENT::GetGridColumnSpan()
* @brief      Get Grid Column Span
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_ELEMENT::GetGridColumnSpan()
{
  return css_gridcolumnspan;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetGridColumnSpan(XDWORD span)
* @brief      Set Grid Column Span
* @ingroup    USERINTERFACE
* 
* @param[in]  span : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetGridColumnSpan(XDWORD span)
{
  css_gridcolumnspan = (span < 1) ? 1 : span;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD UI_ELEMENT::GetGridRowSpan()
* @brief      Get Grid Row Span
* @ingroup    USERINTERFACE
* 
* @return     XDWORD : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_ELEMENT::GetGridRowSpan()
{
  return css_gridrowspan;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetGridRowSpan(XDWORD span)
* @brief      Set Grid Row Span
* @ingroup    USERINTERFACE
* 
* @param[in]  span : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetGridRowSpan(XDWORD span)
{
  css_gridrowspan = (span < 1) ? 1 : span;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_ELEMENT::IsPressed()
* @brief      Is Pressed
* @ingroup    USERINTERFACE
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT::IsPressed()
{
  return ispressed;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_ELEMENT::SetPressed(bool ispressed)
* @brief      Set Pressed
* @ingroup    USERINTERFACE
* 
* @param[in]  ispressed : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::SetPressed(bool ispressed)
{
  bool changed = (this->ispressed != ispressed);
  this->ispressed = ispressed;
  if(changed) ReapplyStyleVisual();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT::Clean()
{
  type                    = UI_ELEMENT_TYPE_UNKNOWN;
  chromerole              = UI_ELEMENT_CHROMEROLE_NONE;

  father                  = NULL;
  isdetached              = false;
  element_layout          = NULL;

  x_position              = 0.0f;
	y_position              = 0.0f;

  z_level                 = 0;

  direction               = UI_ELEMENT_TYPE_DIRECTION_UNKWOWN;
  textalign               = UI_ELEMENT_TYPE_ALIGN_LEFT;

	x_positionwithscroll    = 0.0f;
  y_positionwithscroll    = 0.0f;
	
  for(XDWORD c=0; c<UI_ELEMENT_MARGIN_MAX; c++)
    {
	    margin[c] = 0.0f;
    }

  for(XDWORD c=0; c<UI_ELEMENT_MARGIN_MAX; c++)
    {
      padding[c] = 0.0f;
    }

  border_width = -1.0;    // -1 signals "unset": skin uses its historical default (1.0 for containers).

  border_color_set = false;
  for(int c=0; c<UI_ELEMENT_BORDER_CORNER_MAX; c++)
    {
      border_radius[c] = -1.0;   // -1 signals "unset": fall back to the element's uniform roundrect.
    }

  color_set            = false;
  background_color_set = false;

  box_shadow_set   = false;
  shadow_offset_x  = 0.0;
  shadow_offset_y  = 0.0;
  shadow_blur      = 0.0;

  isactive                = false;
  ispreselect             = false;
  isselected              = false;
  isvisible               = false;

  roundrect               = 0;

  blink_time              = 0;
  blink_state             = false;
	blink_xtimer            = NULL;
  blink_nchanges          = 0;

  style_transition_duration = 0;
  style_transition_active   = false;
  style_transition_xtimer   = NULL;

  transitionstateshow     = UI_ELEMENT_TRANSITION_STATE_SHOW_NONE;

  hasscroll               = false;

  mustredraw              = false;

  snapshot_roundrect      = 0;
  snapshot_taken          = false;
  style_has_state_rules   = false;

  // computed_style ownership: deleted in destructor / StoreComputedStyle replace; Clean only nulls.
  computed_style          = NULL;

  // --- Flexbox: CSS Lite wiring -- same defaults as the mirrored UI_LAYOUTBOX properties (UI_LayoutBox.cpp's own
  //     constructor), so an element that never sets any of these behaves exactly like a freshly-built UI_LAYOUTBOX.
  css_flexcontainer       = false;
  css_flexdirection       = UI_FLEX_DIRECTION_ROW;
  css_justifycontent      = UI_JUSTIFY_CONTENT_FLEX_START;
  css_rowgap              = 0.0;
  css_columngap           = 0.0;
  css_flexwrap            = UI_FLEX_WRAP_NOWRAP;
  css_aligncontent        = UI_ALIGN_CONTENT_FLEX_START;
  css_alignitems          = UI_ALIGN_ITEMS_FLEX_START;

  css_flexgrow            = 0.0;
  css_flexshrink          = 1.0;
  css_flexbasis.specified = false;
  css_flexbasis.value     = 0.0;
  css_alignself           = UI_ALIGN_SELF_AUTO;

  css_gridcontainer       = false;
  css_gridcolumntracks.DeleteAll();
  css_gridrowtracks.DeleteAll();
  css_gridcolumnspan      = 1;
  css_gridrowspan         = 1;

  ispressed               = false;

  intrinsic_width         = -1.0;    // unset -- see GetIntrinsicWidth()'s own comment
  intrinsic_height        = -1.0;
}



