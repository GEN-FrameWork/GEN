/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_LayoutBox.cpp
*
* @class      UI_LAYOUTBOX
* @brief      User Interface : one node of the CSS top-left layout box tree -- pure geometry, no painting.
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

#include "UI_LayoutBox.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LAYOUTBOX::UI_LAYOUTBOX()
* @brief      Constructor of class
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUTBOX::UI_LAYOUTBOX()
{
  contentleft   = 0.0;
  contenttop    = 0.0;
  contentwidth  = 0.0;
  contentheight = 0.0;

  padding.top = padding.right = padding.bottom = padding.left = 0.0;
  border.top  = border.right  = border.bottom  = border.left  = 0.0;
  margin.top  = margin.right  = margin.bottom  = margin.left  = 0.0;

  position = UI_POSITION_STATIC;

  insets.top.specified = insets.right.specified = insets.bottom.specified = insets.left.specified = false;
  insets.top.value     = insets.right.value     = insets.bottom.value     = insets.left.value     = 0.0;

  flexcontainer   = false;
  flexdirection   = UI_FLEX_DIRECTION_ROW;
  justifycontent  = UI_JUSTIFY_CONTENT_FLEX_START;
  rowgap          = 0.0;
  columngap       = 0.0;

  flexgrow           = 0.0;    // CSS initial value
  flexshrink          = 1.0;    // CSS initial value
  flexbasis.specified = false;  // "auto"
  flexbasis.value     = 0.0;

  flexwrap     = UI_FLEX_WRAP_NOWRAP;
  aligncontent = UI_ALIGN_CONTENT_FLEX_START;

  alignitems = UI_ALIGN_ITEMS_FLEX_START;   // deliberately not CSS's real default (stretch) -- see SCOPE ADDENDUM
  alignself  = UI_ALIGN_SELF_AUTO;          // CSS initial value: defer to the container's align-items

  gridcontainer = false;

  gridcolumnspan = 1;   // CSS initial value: occupy exactly one column
  gridrowspan    = 1;   // CSS initial value: occupy exactly one row

  parent = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LAYOUTBOX::~UI_LAYOUTBOX()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUTBOX::~UI_LAYOUTBOX()
{
  for(XDWORD c=0; c<children.GetSize(); c++)
    {
      UI_LAYOUTBOX* child = children.Get(c);
      if(child) GEN_DELETE child;
    }

  children.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::SetContentBox(double left, double top, double width, double height)
* @brief      Set content box
* @ingroup    USERINTERFACE
*
* @param[in]  left : Content box left edge (CSS top-left origin).
* @param[in]  top : Content box top edge.
* @param[in]  width : Content box width.
* @param[in]  height : Content box height.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetContentBox(double left, double top, double width, double height)
{
  contentleft   = left;
  contenttop    = top;
  contentwidth  = width;
  contentheight = height;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_LAYOUTBOX::GetContentLeft()
* @brief      Get content left
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUTBOX::GetContentLeft()
{
  return contentleft;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_LAYOUTBOX::GetContentTop()
* @brief      Get content top
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUTBOX::GetContentTop()
{
  return contenttop;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_LAYOUTBOX::GetContentWidth()
* @brief      Get content width
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUTBOX::GetContentWidth()
{
  return contentwidth;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_LAYOUTBOX::GetContentHeight()
* @brief      Get content height
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUTBOX::GetContentHeight()
{
  return contentheight;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::SetPadding(double top, double right, double bottom, double left)
* @brief      Set padding
* @ingroup    USERINTERFACE
*
* @param[in]  top : Top edge width.
* @param[in]  right : Right edge width.
* @param[in]  bottom : Bottom edge width.
* @param[in]  left : Left edge width.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetPadding(double top, double right, double bottom, double left)
{
  padding.top    = top;
  padding.right  = right;
  padding.bottom = bottom;
  padding.left   = left;

  ClampEdgesToZero(padding);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::SetBorder(double top, double right, double bottom, double left)
* @brief      Set border
* @ingroup    USERINTERFACE
*
* @param[in]  top : Top edge width.
* @param[in]  right : Right edge width.
* @param[in]  bottom : Bottom edge width.
* @param[in]  left : Left edge width.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetBorder(double top, double right, double bottom, double left)
{
  border.top    = top;
  border.right  = right;
  border.bottom = bottom;
  border.left   = left;

  ClampEdgesToZero(border);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::SetMargin(double top, double right, double bottom, double left)
* @brief      Set margin
* @ingroup    USERINTERFACE
*
* @param[in]  top : Top edge width.
* @param[in]  right : Right edge width.
* @param[in]  bottom : Bottom edge width.
* @param[in]  left : Left edge width.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetMargin(double top, double right, double bottom, double left)
{
  margin.top    = top;
  margin.right  = right;
  margin.bottom = bottom;
  margin.left   = left;

  ClampEdgesToZero(margin);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LAYOUTBOX_EDGES& UI_LAYOUTBOX::GetPadding()
* @brief      Get padding
* @ingroup    USERINTERFACE
*
* @return     UI_LAYOUTBOX_EDGES& : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUTBOX_EDGES& UI_LAYOUTBOX::GetPadding()
{
  return padding;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LAYOUTBOX_EDGES& UI_LAYOUTBOX::GetBorder()
* @brief      Get border
* @ingroup    USERINTERFACE
*
* @return     UI_LAYOUTBOX_EDGES& : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUTBOX_EDGES& UI_LAYOUTBOX::GetBorder()
{
  return border;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LAYOUTBOX_EDGES& UI_LAYOUTBOX::GetMargin()
* @brief      Get margin
* @ingroup    USERINTERFACE
*
* @return     UI_LAYOUTBOX_EDGES& : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUTBOX_EDGES& UI_LAYOUTBOX::GetMargin()
{
  return margin;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::GetPaddingBox(double& left, double& top, double& width, double& height)
* @brief      Get padding box
* @ingroup    USERINTERFACE
*
* @param[out] left : Receives the padding box left edge.
* @param[out] top : Receives the padding box top edge.
* @param[out] width : Receives the padding box width.
* @param[out] height : Receives the padding box height.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::GetPaddingBox(double& left, double& top, double& width, double& height)
{
  left   = contentleft - padding.left;
  top    = contenttop  - padding.top;
  width  = contentwidth  + padding.left + padding.right;
  height = contentheight + padding.top  + padding.bottom;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::GetBorderBox(double& left, double& top, double& width, double& height)
* @brief      Get border box
* @ingroup    USERINTERFACE
*
* @param[out] left : Receives the border box left edge.
* @param[out] top : Receives the border box top edge.
* @param[out] width : Receives the border box width.
* @param[out] height : Receives the border box height.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::GetBorderBox(double& left, double& top, double& width, double& height)
{
  left   = contentleft - padding.left - border.left;
  top    = contenttop  - padding.top  - border.top;
  width  = contentwidth  + padding.left + padding.right  + border.left + border.right;
  height = contentheight + padding.top  + padding.bottom + border.top  + border.bottom;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::GetMarginBox(double& left, double& top, double& width, double& height)
* @brief      Get margin box
* @ingroup    USERINTERFACE
*
* @param[out] left : Receives the margin box left edge.
* @param[out] top : Receives the margin box top edge.
* @param[out] width : Receives the margin box width.
* @param[out] height : Receives the margin box height.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::GetMarginBox(double& left, double& top, double& width, double& height)
{
  left   = contentleft - padding.left - border.left - margin.left;
  top    = contenttop  - padding.top  - border.top  - margin.top;
  width  = contentwidth  + padding.left + padding.right  + border.left + border.right  + margin.left + margin.right;
  height = contentheight + padding.top  + padding.bottom + border.top  + border.bottom + margin.top  + margin.bottom;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::AddChild(UI_LAYOUTBOX* child)
* @brief      Add child
* @ingroup    USERINTERFACE
*
* @param[in]  child : Node to adopt; owned from this call on (this node's destructor deletes it). Ignored if NULL.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::AddChild(UI_LAYOUTBOX* child)
{
  if(!child) return;

  child->parent = this;

  children.Add(child);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<UI_LAYOUTBOX*>& UI_LAYOUTBOX::GetChildren()
* @brief      Get children
* @ingroup    USERINTERFACE
*
* @return     XVECTOR<UI_LAYOUTBOX*>& : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<UI_LAYOUTBOX*>& UI_LAYOUTBOX::GetChildren()
{
  return children;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LAYOUTBOX* UI_LAYOUTBOX::GetParent()
* @brief      Get parent
* @ingroup    USERINTERFACE
*
* @return     UI_LAYOUTBOX* : Requested value (borrowed; NULL for a root node).
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUTBOX* UI_LAYOUTBOX::GetParent()
{
  return parent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_LAYOUTBOX::SetPosition(UI_POSITION _position)
* @brief      Set position
* @ingroup    USERINTERFACE
* 
* @param[in]  _position : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetPosition(UI_POSITION _position)
{
  position = _position;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_POSITION UI_LAYOUTBOX::GetPosition()
* @brief      Get position
* @ingroup    USERINTERFACE
*
* @return     UI_POSITION : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_POSITION UI_LAYOUTBOX::GetPosition()
{
  return position;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_LAYOUTBOX::SetInsets(UI_LAYOUTBOX_INSETS& _insets)
* @brief      Set insets
* @ingroup    USERINTERFACE
* 
* @param[in]  _insets : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetInsets(UI_LAYOUTBOX_INSETS& _insets)
{
  insets = _insets;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LAYOUTBOX_INSETS& UI_LAYOUTBOX::GetInsets()
* @brief      Get insets
* @ingroup    USERINTERFACE
*
* @return     UI_LAYOUTBOX_INSETS& : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUTBOX_INSETS& UI_LAYOUTBOX::GetInsets()
{
  return insets;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::SetFlexContainer(bool isflexcontainer)
* @brief      Set flex container
* @ingroup    USERINTERFACE
*
* @param[in]  isflexcontainer : Whether this box's DIRECT children should be arranged as flex items.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetFlexContainer(bool isflexcontainer)
{
  flexcontainer = isflexcontainer;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_LAYOUTBOX::IsFlexContainer()
* @brief      Is flex container
* @ingroup    USERINTERFACE
*
* @return     bool : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LAYOUTBOX::IsFlexContainer()
{
  return flexcontainer;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::SetFlexDirection(UI_FLEX_DIRECTION direction)
* @brief      Set flex direction
* @ingroup    USERINTERFACE
*
* @param[in]  direction : Main axis and packing direction for this container's children.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetFlexDirection(UI_FLEX_DIRECTION direction)
{
  flexdirection = direction;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_FLEX_DIRECTION UI_LAYOUTBOX::GetFlexDirection()
* @brief      Get flex direction
* @ingroup    USERINTERFACE
*
* @return     UI_FLEX_DIRECTION : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_FLEX_DIRECTION UI_LAYOUTBOX::GetFlexDirection()
{
  return flexdirection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_LAYOUTBOX::SetJustifyContent(UI_JUSTIFY_CONTENT _justifycontent)
* @brief      Set justify content
* @ingroup    USERINTERFACE
* 
* @param[in]  _justifycontent : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetJustifyContent(UI_JUSTIFY_CONTENT _justifycontent)
{
  justifycontent = _justifycontent;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_JUSTIFY_CONTENT UI_LAYOUTBOX::GetJustifyContent()
* @brief      Get justify content
* @ingroup    USERINTERFACE
*
* @return     UI_JUSTIFY_CONTENT : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_JUSTIFY_CONTENT UI_LAYOUTBOX::GetJustifyContent()
{
  return justifycontent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_LAYOUTBOX::SetGap(double _rowgap, double _columngap)
* @brief      Set gap
* @ingroup    USERINTERFACE
* 
* @param[in]  _rowgap : 
* @param[in]  _columngap : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetGap(double _rowgap, double _columngap)
{
  rowgap    = _rowgap;
  columngap = _columngap;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_LAYOUTBOX::GetRowGap()
* @brief      Get row gap
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUTBOX::GetRowGap()
{
  return rowgap;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_LAYOUTBOX::GetColumnGap()
* @brief      Get column gap
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUTBOX::GetColumnGap()
{
  return columngap;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::SetFlexGrow(double grow)
* @brief      Set flex grow
* @ingroup    USERINTERFACE
*
* @param[in]  grow : CSS "flex-grow" factor for this box as a flex item.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetFlexGrow(double grow)
{
  flexgrow = grow;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_LAYOUTBOX::GetFlexGrow()
* @brief      Get flex grow
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUTBOX::GetFlexGrow()
{
  return flexgrow;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::SetFlexShrink(double shrink)
* @brief      Set flex shrink
* @ingroup    USERINTERFACE
*
* @param[in]  shrink : CSS "flex-shrink" factor for this box as a flex item.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetFlexShrink(double shrink)
{
  flexshrink = shrink;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_LAYOUTBOX::GetFlexShrink()
* @brief      Get flex shrink
* @ingroup    USERINTERFACE
*
* @return     double : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUTBOX::GetFlexShrink()
{
  return flexshrink;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::SetFlexBasisAuto()
* @brief      Set flex basis auto
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetFlexBasisAuto()
{
  flexbasis.specified = false;
  flexbasis.value     = 0.0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::SetFlexBasis(double value)
* @brief      Set flex basis
* @ingroup    USERINTERFACE
*
* @param[in]  value : Explicit CSS "flex-basis" (a resolved content-box main-axis size, replacing "auto").
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetFlexBasis(double value)
{
  flexbasis.specified = true;
  flexbasis.value     = value;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LAYOUTBOX_INSET& UI_LAYOUTBOX::GetFlexBasis()
* @brief      Get flex basis
* @ingroup    USERINTERFACE
*
* @return     UI_LAYOUTBOX_INSET& : Requested value ("specified" false means "auto").
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUTBOX_INSET& UI_LAYOUTBOX::GetFlexBasis()
{
  return flexbasis;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::SetFlexWrap(UI_FLEX_WRAP wrap)
* @brief      Set flex wrap
* @ingroup    USERINTERFACE
*
* @param[in]  wrap : CSS "flex-wrap" for this container's children.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetFlexWrap(UI_FLEX_WRAP wrap)
{
  flexwrap = wrap;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_FLEX_WRAP UI_LAYOUTBOX::GetFlexWrap()
* @brief      Get flex wrap
* @ingroup    USERINTERFACE
*
* @return     UI_FLEX_WRAP : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_FLEX_WRAP UI_LAYOUTBOX::GetFlexWrap()
{
  return flexwrap;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_LAYOUTBOX::SetAlignContent(UI_ALIGN_CONTENT _aligncontent)
* @brief      Set align content
* @ingroup    USERINTERFACE
* 
* @param[in]  _aligncontent : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetAlignContent(UI_ALIGN_CONTENT _aligncontent)
{
  aligncontent = _aligncontent;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ALIGN_CONTENT UI_LAYOUTBOX::GetAlignContent()
* @brief      Get align content
* @ingroup    USERINTERFACE
*
* @return     UI_ALIGN_CONTENT : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_ALIGN_CONTENT UI_LAYOUTBOX::GetAlignContent()
{
  return aligncontent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_LAYOUTBOX::SetAlignItems(UI_ALIGN_ITEMS _alignitems)
* @brief      Set align items
* @ingroup    USERINTERFACE
* 
* @param[in]  _alignitems : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetAlignItems(UI_ALIGN_ITEMS _alignitems)
{
  alignitems = _alignitems;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ALIGN_ITEMS UI_LAYOUTBOX::GetAlignItems()
* @brief      Get align items
* @ingroup    USERINTERFACE
*
* @return     UI_ALIGN_ITEMS : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_ALIGN_ITEMS UI_LAYOUTBOX::GetAlignItems()
{
  return alignitems;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void UI_LAYOUTBOX::SetAlignSelf(UI_ALIGN_SELF _alignself)
* @brief      Set align self
* @ingroup    USERINTERFACE
* 
* @param[in]  _alignself : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetAlignSelf(UI_ALIGN_SELF _alignself)
{
  alignself = _alignself;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ALIGN_SELF UI_LAYOUTBOX::GetAlignSelf()
* @brief      Get align self
* @ingroup    USERINTERFACE
*
* @return     UI_ALIGN_SELF : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_ALIGN_SELF UI_LAYOUTBOX::GetAlignSelf()
{
  return alignself;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::SetGridContainer(bool isgridcontainer)
* @brief      Set grid container
* @ingroup    USERINTERFACE
*
* @param[in]  isgridcontainer : Whether this box's DIRECT children are laid out as a CSS grid.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetGridContainer(bool isgridcontainer)
{
  gridcontainer = isgridcontainer;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_LAYOUTBOX::IsGridContainer()
* @brief      Is grid container
* @ingroup    USERINTERFACE
*
* @return     bool : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LAYOUTBOX::IsGridContainer()
{
  return gridcontainer;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::AddGridColumnTrack(UI_GRIDTRACK track)
* @brief      Add grid column track
* @ingroup    USERINTERFACE
*
* @param[in]  track : Track appended to the end of grid-template-columns.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::AddGridColumnTrack(UI_GRIDTRACK track)
{
  gridcolumntracks.Add(track);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::ClearGridColumnTracks()
* @brief      Clear grid column tracks
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::ClearGridColumnTracks()
{
  gridcolumntracks.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<UI_GRIDTRACK>& UI_LAYOUTBOX::GetGridColumnTracks()
* @brief      Get grid column tracks
* @ingroup    USERINTERFACE
*
* @return     XVECTOR<UI_GRIDTRACK>& : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<UI_GRIDTRACK>& UI_LAYOUTBOX::GetGridColumnTracks()
{
  return gridcolumntracks;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::AddGridRowTrack(UI_GRIDTRACK track)
* @brief      Add grid row track
* @ingroup    USERINTERFACE
*
* @param[in]  track : Track appended to the end of grid-template-rows.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::AddGridRowTrack(UI_GRIDTRACK track)
{
  gridrowtracks.Add(track);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::ClearGridRowTracks()
* @brief      Clear grid row tracks
* @ingroup    USERINTERFACE
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::ClearGridRowTracks()
{
  gridrowtracks.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<UI_GRIDTRACK>& UI_LAYOUTBOX::GetGridRowTracks()
* @brief      Get grid row tracks
* @ingroup    USERINTERFACE
*
* @return     XVECTOR<UI_GRIDTRACK>& : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<UI_GRIDTRACK>& UI_LAYOUTBOX::GetGridRowTracks()
{
  return gridrowtracks;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::SetGridColumnSpan(XDWORD span)
* @brief      Set grid column span
* @ingroup    USERINTERFACE
*
* @param[in]  span : Number of columns this box occupies, starting from wherever auto-placement puts it.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetGridColumnSpan(XDWORD span)
{
  gridcolumnspan = span;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD UI_LAYOUTBOX::GetGridColumnSpan()
* @brief      Get grid column span
* @ingroup    USERINTERFACE
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_LAYOUTBOX::GetGridColumnSpan()
{
  return gridcolumnspan;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::SetGridRowSpan(XDWORD span)
* @brief      Set grid row span
* @ingroup    USERINTERFACE
*
* @param[in]  span : Number of rows this box occupies, starting from wherever auto-placement puts it.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::SetGridRowSpan(XDWORD span)
{
  gridrowspan = span;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD UI_LAYOUTBOX::GetGridRowSpan()
* @brief      Get grid row span
* @ingroup    USERINTERFACE
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD UI_LAYOUTBOX::GetGridRowSpan()
{
  return gridrowspan;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTBOX::ClampEdgesToZero(UI_LAYOUTBOX_EDGES& edges)
* @brief      Clamp edges to zero
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in,out] edges : Edge set to clamp in place; any negative component becomes 0.0.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTBOX::ClampEdgesToZero(UI_LAYOUTBOX_EDGES& edges)
{
  if(edges.top    < 0.0) edges.top    = 0.0;
  if(edges.right  < 0.0) edges.right  = 0.0;
  if(edges.bottom < 0.0) edges.bottom = 0.0;
  if(edges.left   < 0.0) edges.left   = 0.0;
}
