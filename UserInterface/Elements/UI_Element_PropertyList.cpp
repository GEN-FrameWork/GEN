/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_Element_PropertyList.cpp
*
* @class      UI_ELEMENT_PROPERTYLIST
* @brief      Name / Value property list (two columns, header, scrollable body, draggable column splitter)
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

#include "UI_Element_PropertyList.h"
#include "UI_Layout.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


UI_ELEMENT_PROPERTYLIST_ROW::UI_ELEMENT_PROPERTYLIST_ROW()
{
  Clean();
}


UI_ELEMENT_PROPERTYLIST_ROW::~UI_ELEMENT_PROPERTYLIST_ROW()
{
  Clean();
}


void UI_ELEMENT_PROPERTYLIST_ROW::Clean()
{
  name.Empty();
  value.Empty();
}


UI_ELEMENT_PROPERTYLIST::UI_ELEMENT_PROPERTYLIST()
{
  Clean();

  SetType(UI_ELEMENT_TYPE_PROPERTYLIST);
  GetTypeString()->Set(NL("propertylist"));

  headername.Set(NL("Name"));
  headervalue.Set(NL("Value"));
}


UI_ELEMENT_PROPERTYLIST::~UI_ELEMENT_PROPERTYLIST()
{
  Clear();
  Clean();
}


XDWORD UI_ELEMENT_PROPERTYLIST::GetSizeFont()
{
  return sizefont;
}


void UI_ELEMENT_PROPERTYLIST::SetSizeFont(XDWORD sizefont)
{
  this->sizefont = sizefont;
}


double UI_ELEMENT_PROPERTYLIST::GetRowHeight()
{
  return rowheight;
}


void UI_ELEMENT_PROPERTYLIST::SetRowHeight(double rowheight)
{
  if(rowheight < 1.0f) rowheight = 1.0f;
  this->rowheight = rowheight;
}


double UI_ELEMENT_PROPERTYLIST::GetHeaderHeight()
{
  return headerheight;
}


void UI_ELEMENT_PROPERTYLIST::SetHeaderHeight(double headerheight)
{
  if(headerheight < 1.0f) headerheight = 1.0f;
  this->headerheight = headerheight;
}


double UI_ELEMENT_PROPERTYLIST::GetContentHeight()
{
  return contentheight;
}


void UI_ELEMENT_PROPERTYLIST::SetContentHeight(double contentheight)
{
  this->contentheight = contentheight;
}


XSTRING* UI_ELEMENT_PROPERTYLIST::GetHeaderName()
{
  return &headername;
}


XSTRING* UI_ELEMENT_PROPERTYLIST::GetHeaderValue()
{
  return &headervalue;
}


void UI_ELEMENT_PROPERTYLIST::SetHeaderCaptions(XCHAR* headername, XCHAR* headervalue)
{
  if(headername)  this->headername.Set(headername);
  if(headervalue) this->headervalue.Set(headervalue);

  SetMustReDraw(true);
}


bool UI_ELEMENT_PROPERTYLIST::Clear()
{
  rows.DeleteContents();
  rows.DeleteAll();

  contentheight = 0.0f;

  Scroll_SetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, 0.0f);
  Scroll_SetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, 0.0f);

  SetMustReDraw(true);

  return true;
}


bool UI_ELEMENT_PROPERTYLIST::Add(XCHAR* name, XCHAR* value)
{
  UI_ELEMENT_PROPERTYLIST_ROW* row = GEN_NEW UI_ELEMENT_PROPERTYLIST_ROW();
  if(!row) return false;

  if(name)  row->name.Set(name);
  if(value) row->value.Set(value);

  rows.Add(row);

  SetMustReDraw(true);

  return true;
}


bool UI_ELEMENT_PROPERTYLIST::SetValue(XCHAR* name, XCHAR* value)
{
  if(!name) return false;

  for(XDWORD c=0; c<rows.GetSize(); c++)
    {
      UI_ELEMENT_PROPERTYLIST_ROW* row = rows.Get(c);
      if(!row) continue;

      if(!row->name.Compare(name, true))
        {
          if(value) row->value.Set(value);
           else     row->value.Empty();

          SetMustReDraw(true);
          return true;
        }
    }

  return Add(name, value);
}


XDWORD UI_ELEMENT_PROPERTYLIST::GetRowCount()
{
  return rows.GetSize();
}


UI_ELEMENT_PROPERTYLIST_ROW* UI_ELEMENT_PROPERTYLIST::GetRow(XDWORD index)
{
  return rows.Get(index);
}


double UI_ELEMENT_PROPERTYLIST::GetNameColumnWidth()
{
  return name_column_width;
}


void UI_ELEMENT_PROPERTYLIST::SetNameColumnWidth(double namecolumnwidth)
{
  double minwidth = UI_ELEMENT_PROPERTYLIST_MIN_COLUMNWIDTH;
  double boxwidth = GetBoundaryLine() ? GetBoundaryLine()->width : 0.0f;

  if(namecolumnwidth < minwidth) namecolumnwidth = minwidth;

  // Max keeps at least a minimum-width Value column. Unknown box width (not laid out yet) = no upper clamp.
  if(boxwidth > 0.0f)
    {
      double maxwidth = boxwidth - minwidth;
      if(maxwidth < minwidth) maxwidth = minwidth;
      if(namecolumnwidth > maxwidth) namecolumnwidth = maxwidth;
    }

  if(name_column_width != namecolumnwidth)
    {
      name_column_width = namecolumnwidth;
      SetMustReDraw(true);
    }
}


bool UI_ELEMENT_PROPERTYLIST::HitTestSplitter(double local_x, double local_y, double box_w, double box_h)
{
  if(local_x < 0.0f || local_x >= box_w) return false;
  if(local_y < 0.0f || local_y >= box_h) return false;

  double half = UI_ELEMENT_PROPERTYLIST_SPLITTER_HITWIDTH / 2.0f;

  return ((local_x >= (name_column_width - half)) && (local_x <= (name_column_width + half)));
}


bool UI_ELEMENT_PROPERTYLIST::ScreenToLocal(int screen_x, int screen_y, double& local_x, double& local_y)
{
  if(!GetBoundaryLine()) return false;

  double rect_x = GetXPosition();
  double rect_y = GetYPosition() - GetBoundaryLine()->height;
  double scale  = 1.0;

  UI_LAYOUT* layout = GetLayout();
  if(layout && layout->IsUIScaleActive())
    {
      scale = layout->GetUIScale();
      if(scale < UI_LAYOUT_UISCALE_MIN) scale = UI_LAYOUT_UISCALE_MIN;
      rect_x = rect_x * scale + layout->GetUIScaleOffsetX();
      rect_y = rect_y * scale + layout->GetUIScaleOffsetY();
    }

  local_x = ((double)screen_x - rect_x) / scale;
  local_y = ((double)screen_y - rect_y) / scale;

  return true;
}


bool UI_ELEMENT_PROPERTYLIST::IsSplitDragging()
{
  return split_dragging;
}


bool UI_ELEMENT_PROPERTYLIST::Split_BeginDrag()
{
  split_dragging = true;
  return true;
}


bool UI_ELEMENT_PROPERTYLIST::Split_DragTo(double abs_x)
{
  if(!split_dragging) return false;

  SetNameColumnWidth(abs_x);

  return true;
}


bool UI_ELEMENT_PROPERTYLIST::Split_EndDrag()
{
  bool wasdragging = split_dragging;
  split_dragging   = false;

  return wasdragging;
}


void UI_ELEMENT_PROPERTYLIST::Clean()
{
  sizefont          = 0;
  rowheight         = UI_ELEMENT_PROPERTYLIST_DEFAULT_ROWHEIGHT;
  headerheight      = UI_ELEMENT_PROPERTYLIST_DEFAULT_HEADERHEIGHT;
  contentheight     = 0.0f;
  name_column_width = UI_ELEMENT_PROPERTYLIST_DEFAULT_NAMECOLUMNWIDTH;
  split_dragging    = false;
}
