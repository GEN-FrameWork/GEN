/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_Element_Tree.cpp
*
* @class      UI_ELEMENT_TREE
* @brief      Generic hierarchical tree (unlimited nesting, [+]/[-] boxes, dotted guides, scrollable, selection)
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

#include "UI_Element_Tree.h"
#include "UI_Layout.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


UI_ELEMENT_TREE_NODE::UI_ELEMENT_TREE_NODE()
{
  Clean();
}


UI_ELEMENT_TREE_NODE::~UI_ELEMENT_TREE_NODE()
{
  children.DeleteContents();                                                   // recursive: each child deletes its own children
  children.DeleteAll();

  Clean();
}


void UI_ELEMENT_TREE_NODE::Clean()
{
  text.Empty();
  icon.Empty();
  expanded = false;
  father   = NULL;
}


bool UI_ELEMENT_TREE_NODE::SetIcon(XCHAR* resource)
{
  if(resource && resource[0]) icon.Set(resource);
   else                       icon.Empty();

  return true;
}


XSTRING* UI_ELEMENT_TREE_NODE::GetIcon()
{
  return &icon;
}


UI_ELEMENT_TREE::UI_ELEMENT_TREE()
{
  Clean();

  SetType(UI_ELEMENT_TYPE_TREE);
  GetTypeString()->Set(NL("tree"));
}


UI_ELEMENT_TREE::~UI_ELEMENT_TREE()
{
  Clear();
  Clean();
}


XDWORD UI_ELEMENT_TREE::GetSizeFont()
{
  return sizefont;
}


void UI_ELEMENT_TREE::SetSizeFont(XDWORD sizefont)
{
  this->sizefont = sizefont;
}


double UI_ELEMENT_TREE::GetRowHeight()
{
  return rowheight;
}


void UI_ELEMENT_TREE::SetRowHeight(double rowheight)
{
  if(rowheight < 1.0f) rowheight = 1.0f;
  this->rowheight = rowheight;

  visible_dirty = true;
  SetMustReDraw(true);
}


double UI_ELEMENT_TREE::GetIndentWidth()
{
  return indentwidth;
}


void UI_ELEMENT_TREE::SetIndentWidth(double indentwidth)
{
  if(indentwidth < UI_ELEMENT_TREE_BOXSIZE + 2.0f) indentwidth = UI_ELEMENT_TREE_BOXSIZE + 2.0f;
  this->indentwidth = indentwidth;

  SetMustReDraw(true);
}


double UI_ELEMENT_TREE::GetContentHeight()
{
  return contentheight;
}


void UI_ELEMENT_TREE::SetContentHeight(double contentheight)
{
  this->contentheight = contentheight;
}


double UI_ELEMENT_TREE::GetContentWidth()
{
  return contentwidth;
}


void UI_ELEMENT_TREE::SetContentWidth(double contentwidth)
{
  this->contentwidth = contentwidth;
}


UI_ELEMENT_TREE_NODE* UI_ELEMENT_TREE::AddNode(UI_ELEMENT_TREE_NODE* father, XCHAR* text)
{
  return AddNode(father, text, NULL);
}


UI_ELEMENT_TREE_NODE* UI_ELEMENT_TREE::AddNode(UI_ELEMENT_TREE_NODE* father, XCHAR* text, XCHAR* icon)
{
  if(father && !IsInTree(father)) return NULL;

  UI_ELEMENT_TREE_NODE* node = GEN_NEW UI_ELEMENT_TREE_NODE();
  if(!node) return NULL;

  if(text) node->text.Set(text);
  if(icon) node->SetIcon(icon);

  node->father = father;

  if(father) father->children.Add(node);
   else      roots.Add(node);

  visible_dirty = true;
  SetMustReDraw(true);

  return node;
}


bool UI_ELEMENT_TREE::RemoveNode(UI_ELEMENT_TREE_NODE* node)
{
  if(!node) return false;
  if(!IsInTree(node)) return false;

  // Drop the selection if it lives inside the removed subtree.
  for(UI_ELEMENT_TREE_NODE* walk = selected; walk; walk = walk->father)
    {
      if(walk == node)
        {
          selected = NULL;
          break;
        }
    }

  if(node->father) node->father->children.Delete(node);
   else            roots.Delete(node);

  GEN_DELETE node;

  visible_dirty = true;
  SetMustReDraw(true);

  return true;
}


bool UI_ELEMENT_TREE::Clear()
{
  roots.DeleteContents();
  roots.DeleteAll();

  visible_nodes.DeleteAll();
  visible_dirty = true;

  selected      = NULL;
  contentheight = 0.0f;
  contentwidth  = 0.0f;

  Scroll_SetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, 0.0f);
  Scroll_SetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, 0.0f);

  SetMustReDraw(true);

  return true;
}


XDWORD UI_ELEMENT_TREE::GetRootCount()
{
  return roots.GetSize();
}


UI_ELEMENT_TREE_NODE* UI_ELEMENT_TREE::GetRoot(XDWORD index)
{
  return roots.Get(index);
}


XVECTOR<UI_ELEMENT_TREE_NODE*>* UI_ELEMENT_TREE::GetRoots()
{
  return &roots;
}


XDWORD UI_ELEMENT_TREE::GetNodeDepth(UI_ELEMENT_TREE_NODE* node)
{
  XDWORD depth = 0;

  while(node && node->father)
    {
      depth++;
      node = node->father;
    }

  return depth;
}


UI_ELEMENT_TREE_NODE* UI_ELEMENT_TREE::GetSelected()
{
  return selected;
}


bool UI_ELEMENT_TREE::SetSelected(UI_ELEMENT_TREE_NODE* node)
{
  if(node && !IsInTree(node)) return false;

  if(selected != node)
    {
      selected = node;
      SetMustReDraw(true);
    }

  return true;
}


bool UI_ELEMENT_TREE::Expand(UI_ELEMENT_TREE_NODE* node)
{
  if(!node || !node->HasChildren() || node->expanded) return false;
  if(!IsInTree(node)) return false;

  node->expanded = true;
  visible_dirty  = true;
  SetMustReDraw(true);

  return true;
}


bool UI_ELEMENT_TREE::Collapse(UI_ELEMENT_TREE_NODE* node)
{
  if(!node || !node->expanded) return false;
  if(!IsInTree(node)) return false;

  node->expanded = false;
  visible_dirty  = true;

  // A selection hidden by the collapse moves to the collapsed node.
  for(UI_ELEMENT_TREE_NODE* walk = selected; walk; walk = walk->father)
    {
      if(walk == node)
        {
          selected = node;
          break;
        }
    }

  SetMustReDraw(true);

  return true;
}


bool UI_ELEMENT_TREE::Toggle(UI_ELEMENT_TREE_NODE* node)
{
  if(!node || !node->HasChildren()) return false;

  return node->expanded ? Collapse(node) : Expand(node);
}


XDWORD UI_ELEMENT_TREE::GetVisibleRowCount()
{
  if(visible_dirty) Flatten();

  return visible_nodes.GetSize();
}


UI_ELEMENT_TREE_NODE* UI_ELEMENT_TREE::GetVisibleNode(XDWORD row)
{
  if(visible_dirty) Flatten();

  return visible_nodes.Get(row);
}


int UI_ELEMENT_TREE::GetVisibleRowOfNode(UI_ELEMENT_TREE_NODE* node)
{
  if(!node) return -1;

  if(visible_dirty) Flatten();

  for(XDWORD c=0; c<visible_nodes.GetSize(); c++)
    {
      if(visible_nodes.Get(c) == node) return (int)c;
    }

  return -1;
}


double UI_ELEMENT_TREE::GetBoxCenterX(XDWORD depth)
{
  return UI_ELEMENT_TREE_PADDING_X + ((double)depth * indentwidth) + (indentwidth / 2.0f);
}


double UI_ELEMENT_TREE::GetLabelX(XDWORD depth)
{
  return UI_ELEMENT_TREE_PADDING_X + ((double)(depth + 1) * indentwidth) + 4.0f;
}


double UI_ELEMENT_TREE::GetTextX(XDWORD depth)
{
  return GetLabelX(depth);
}


double UI_ELEMENT_TREE::GetIconSize()
{
  double sz = (double)sizefont;
  if(sz < 1.0f) sz = 1.0f;
  return sz;
}


double UI_ELEMENT_TREE::GetTextX(UI_ELEMENT_TREE_NODE* node)
{
  double x = GetLabelX(node ? GetNodeDepth(node) : 0);

  if(node && node->HasIcon()) x += GetIconSize() + UI_ELEMENT_TREE_ICON_GAP;

  return x;
}


double UI_ELEMENT_TREE::GetRowTop(XDWORD row)
{
  double vdisp = Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL);

  return UI_ELEMENT_TREE_PADDING_Y + ((double)row * rowheight) + vdisp;
}


UI_ELEMENT_TREE_NODE* UI_ELEMENT_TREE::HitTestRow(double local_y)
{
  if(local_y < 0.0f) return NULL;                                              // above the element box

  double rel = local_y - GetRowTop(0);
  if(rel < 0.0f) return NULL;

  XDWORD row = (XDWORD)(rel / rowheight);

  return GetVisibleNode(row);
}


UI_ELEMENT_TREE_NODE* UI_ELEMENT_TREE::HitTestExpandBox(double local_x, double local_y)
{
  UI_ELEMENT_TREE_NODE* node = HitTestRow(local_y);
  if(!node || !node->HasChildren()) return NULL;

  double hdisp = Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL);
  double cx    = GetBoxCenterX(GetNodeDepth(node)) + hdisp;
  double half  = (UI_ELEMENT_TREE_BOXSIZE / 2.0f) + 2.0f;

  if(local_x < (cx - half) || local_x > (cx + half)) return NULL;

  return node;
}


UI_ELEMENT_TREE_NODE* UI_ELEMENT_TREE::HitTestNode(double local_x, double local_y)
{
  UI_ELEMENT_TREE_NODE* node = HitTestRow(local_y);
  if(!node) return NULL;

  double hdisp = Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL);

  // Label hit includes the optional icon band in front of the text.
  if(local_x < (GetLabelX(GetNodeDepth(node)) + hdisp - 2.0f)) return NULL;

  return node;
}


bool UI_ELEMENT_TREE::ScreenToLocal(int screen_x, int screen_y, double& local_x, double& local_y)
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


bool UI_ELEMENT_TREE::ClickAt(double local_x, double local_y)
{
  UI_ELEMENT_TREE_NODE* node = HitTestExpandBox(local_x, local_y);
  if(node) return Toggle(node);

  node = HitTestNode(local_x, local_y);
  if(node && (node != selected)) return SetSelected(node);

  return false;
}


void UI_ELEMENT_TREE::Flatten()
{
  visible_nodes.DeleteAll();

  for(XDWORD c=0; c<roots.GetSize(); c++)
    {
      Flatten_Node(roots.Get(c));
    }

  visible_dirty = false;

  contentheight = visible_nodes.GetSize() ? ((UI_ELEMENT_TREE_PADDING_Y * 2.0f) + ((double)visible_nodes.GetSize() * rowheight)) : 0.0f;
}


void UI_ELEMENT_TREE::Flatten_Node(UI_ELEMENT_TREE_NODE* node)
{
  if(!node) return;

  visible_nodes.Add(node);

  if(!node->expanded) return;

  for(XDWORD c=0; c<node->children.GetSize(); c++)
    {
      Flatten_Node(node->children.Get(c));
    }
}


bool UI_ELEMENT_TREE::IsOwned(XVECTOR<UI_ELEMENT_TREE_NODE*>* list, UI_ELEMENT_TREE_NODE* node)
{
  return (list->Find(node) != NOTFOUND);
}


bool UI_ELEMENT_TREE::IsInTree(UI_ELEMENT_TREE_NODE* node)
{
  if(!node) return false;

  UI_ELEMENT_TREE_NODE* top = node;
  while(top->father) top = top->father;

  return IsOwned(&roots, top);
}


void UI_ELEMENT_TREE::Clean()
{
  sizefont      = 0;
  rowheight     = UI_ELEMENT_TREE_DEFAULT_ROWHEIGHT;
  indentwidth   = UI_ELEMENT_TREE_DEFAULT_INDENTWIDTH;
  contentheight = 0.0f;
  contentwidth  = 0.0f;
  selected      = NULL;
  visible_dirty = true;
}
