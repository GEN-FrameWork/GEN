/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_LayoutEngine.cpp
*
* @class      UI_LAYOUTENGINE
* @brief      User Interface : builds a UI_LAYOUTBOX tree from a UI_ELEMENT tree.
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

#include "UI_LayoutEngine.h"
#include "UI_CSSAdapter.h"
#include "UI_Element_Form.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_LAYOUTBOX* UI_LAYOUTENGINE::BuildTree(UI_ELEMENT* root)
* @brief      Build tree
* @note       STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  root : Element to build a UI_LAYOUTBOX (sub)tree for; NULL returns NULL.
*
* @return     UI_LAYOUTBOX* : Owned tree root, or NULL if "root" is NULL.
*
* --------------------------------------------------------------------------------------------------------------------*/
UI_LAYOUTBOX* UI_LAYOUTENGINE::BuildTree(UI_ELEMENT* root)
{
  if(!root) return NULL;

  UI_LAYOUTBOX* box = GEN_NEW UI_LAYOUTBOX();

  UI_CSSBOX cssbox = UI_CSSBox_Get(root);

  // Phase 4 ("migración del ejemplo" -- footer icon/text gap regression fix, see UI_SKIN::CalculateBoundaryLine()'s
  // own SCOPE ADDENDUM for the full root-cause story): "root"'s CONTENT SIZE -- what ResolveFlexItemSizes() below
  // reads as a flex-basis:auto item's "hypothetical main size" -- must come from its INTRINSIC size (its authored
  // width/height for a fixed-size widget, or its freshly-measured natural size for auto-sized content), NOT from
  // GetBoundaryLine()/UI_CSSBox_Get(), which WriteBackTree() overwrites with whatever this same engine last
  // RESOLVED it to. Falling back to cssbox.width/height (the pre-fix behaviour) only when GetIntrinsicWidth()/
  // Height() is still unset (-1) keeps this a no-op for any element CalculateBoundaryLine() has never touched.
  double contentwidth  = cssbox.width;
  double contentheight = cssbox.height;

  double intrinsicwidth = root->GetIntrinsicWidth();
  if(intrinsicwidth >= 0.0) contentwidth = intrinsicwidth;

  double intrinsicheight = root->GetIntrinsicHeight();
  if(intrinsicheight >= 0.0) contentheight = intrinsicheight;

  box->SetContentBox(cssbox.left, cssbox.top, contentwidth, contentheight);

  box->SetPadding(root->GetPadding(UI_ELEMENT_TYPE_ALIGN_UP), root->GetPadding(UI_ELEMENT_TYPE_ALIGN_RIGHT), root->GetPadding(UI_ELEMENT_TYPE_ALIGN_DOWN), root->GetPadding(UI_ELEMENT_TYPE_ALIGN_LEFT));

  double borderwidth = root->GetBorderWidth();
  if(borderwidth < 0.0) borderwidth = 0.0;    // -1 means "skin default": a painting concern, not a layout one

  box->SetBorder(borderwidth, borderwidth, borderwidth, borderwidth);

  box->SetMargin(root->GetMargin(UI_ELEMENT_TYPE_ALIGN_UP), root->GetMargin(UI_ELEMENT_TYPE_ALIGN_RIGHT), root->GetMargin(UI_ELEMENT_TYPE_ALIGN_DOWN), root->GetMargin(UI_ELEMENT_TYPE_ALIGN_LEFT));

  // Flexbox: CSS Lite wiring (see this file's SCOPE ADDENDUM on RunLayout()) -- "root"'s own Flexbox properties
  // are plain storage on UI_ELEMENT, one-for-one mirrors of UI_LAYOUTBOX's own (same enums/defaults), so this is
  // a straight copy, both the container side (how "root" itself arranges ITS children) and the item side (how
  // "root" is itself sized/aligned by ITS OWN father's flex layout, resolved one level up, exactly like every
  // other UI_LAYOUTBOX field this function already copies from the element that owns it).
  box->SetFlexContainer(root->IsFlexContainer());
  box->SetFlexDirection(root->GetFlexDirection());
  box->SetJustifyContent(root->GetJustifyContent());
  box->SetGap(root->GetRowGap(), root->GetColumnGap());
  box->SetFlexWrap(root->GetFlexWrap());
  box->SetAlignContent(root->GetAlignContent());
  box->SetAlignItems(root->GetAlignItems());

  box->SetFlexGrow(root->GetFlexGrow());
  box->SetFlexShrink(root->GetFlexShrink());

  UI_LAYOUTBOX_INSET flexbasis = root->GetFlexBasis();
  if(flexbasis.specified) box->SetFlexBasis(flexbasis.value);
    else box->SetFlexBasisAuto();

  box->SetAlignSelf(root->GetAlignSelf());

  // CSS Grid: mirror tracks/spans onto the layout box (Phase 3 wiring).
  box->SetGridContainer(root->IsGridContainer());
  {
    XVECTOR<UI_GRIDTRACK>& cols = root->GetGridColumnTracks();
    for(XDWORD t=0; t<cols.GetSize(); t++) box->AddGridColumnTrack(cols.Get(t));
    XVECTOR<UI_GRIDTRACK>& rows = root->GetGridRowTracks();
    for(XDWORD t=0; t<rows.GetSize(); t++) box->AddGridRowTrack(rows.Get(t));
    box->SetGridColumnSpan(root->GetGridColumnSpan());
    box->SetGridRowSpan(root->GetGridRowSpan());
  }

  XVECTOR<UI_ELEMENT*>* childelements = root->GetComposeElements();
  if(childelements)
    {
      for(XDWORD c=0; c<childelements->GetSize(); c++)
        {
          UI_ELEMENT* childelement = childelements->Get(c);
          if(!childelement) continue;
          // Phase 3: invisible children must not consume flex/grid slots (badge_ok/ko, etc.).
          if(!childelement->IsVisible()) continue;

          UI_LAYOUTBOX* childbox = BuildTree(childelement);
          if(childbox) box->AddChild(childbox);
        }
    }

  return box;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::ApplyPositioning(UI_LAYOUTBOX* root)
* @brief      Apply positioning
* @note       STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  root : Tree to reposition in place (root and every descendant). Ignored if NULL.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::ApplyPositioning(UI_LAYOUTBOX* root)
{
  if(!root) return;

  ApplyPositioningRecursive(root, root, NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::ApplyPositioningRecursive(UI_LAYOUTBOX* box, UI_LAYOUTBOX* rootbox, UI_LAYOUTBOX* nearestpositionedancestor)
* @brief      Apply positioning recursive
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  box : Node being visited.
* @param[in]  rootbox : Tree root, used as the containing block fallback when no ancestor is positioned.
* @param[in]  nearestpositionedancestor : Nearest ancestor (of "box") whose position is not STATIC, or NULL.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::ApplyPositioningRecursive(UI_LAYOUTBOX* box, UI_LAYOUTBOX* rootbox, UI_LAYOUTBOX* nearestpositionedancestor)
{
  if(!box) return;

  switch(box->GetPosition())
    {
      case UI_POSITION_RELATIVE:
        ApplyRelativeOffset(box);
        break;

      case UI_POSITION_ABSOLUTE:
        ApplyAbsolutePosition(box, nearestpositionedancestor ? nearestpositionedancestor : rootbox);
        break;

      case UI_POSITION_STATIC:
      default:
        break;
    }

  UI_LAYOUTBOX* childpositionedancestor = (box->GetPosition() != UI_POSITION_STATIC) ? box : nearestpositionedancestor;

  XVECTOR<UI_LAYOUTBOX*>& children = box->GetChildren();
  for(XDWORD c=0; c<children.GetSize(); c++)
    {
      UI_LAYOUTBOX* child = children.Get(c);
      if(child) ApplyPositioningRecursive(child, rootbox, childpositionedancestor);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::ApplyRelativeOffset(UI_LAYOUTBOX* box)
* @brief      Apply relative offset
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  box : Node to offset in place from its own current (static-flow) content box position.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::ApplyRelativeOffset(UI_LAYOUTBOX* box)
{
  if(!box) return;

  UI_LAYOUTBOX_INSETS& insets = box->GetInsets();

  double horizontaloffset = 0.0;
  if(insets.left.specified)            horizontaloffset =  insets.left.value;
  else if(insets.right.specified)      horizontaloffset = -insets.right.value;

  double verticaloffset = 0.0;
  if(insets.top.specified)             verticaloffset =  insets.top.value;
  else if(insets.bottom.specified)     verticaloffset = -insets.bottom.value;

  box->SetContentBox(box->GetContentLeft() + horizontaloffset, box->GetContentTop() + verticaloffset, box->GetContentWidth(), box->GetContentHeight());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::ApplyAbsolutePosition(UI_LAYOUTBOX* box, UI_LAYOUTBOX* containingblock)
* @brief      Apply absolute position
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  box : Node to reposition in place.
* @param[in]  containingblock : Box whose PADDING box "box" is positioned against (nearest positioned ancestor,
*                                or the tree root).
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::ApplyAbsolutePosition(UI_LAYOUTBOX* box, UI_LAYOUTBOX* containingblock)
{
  if(!box || !containingblock) return;

  double cbleft, cbtop, cbwidth, cbheight;
  containingblock->GetPaddingBox(cbleft, cbtop, cbwidth, cbheight);

  UI_LAYOUTBOX_INSETS& insets = box->GetInsets();

  double newleft = box->GetContentLeft();
  if(insets.left.specified)            newleft = cbleft + insets.left.value;
  else if(insets.right.specified)      newleft = cbleft + cbwidth - insets.right.value - box->GetContentWidth();

  double newtop = box->GetContentTop();
  if(insets.top.specified)             newtop = cbtop + insets.top.value;
  else if(insets.bottom.specified)     newtop = cbtop + cbheight - insets.bottom.value - box->GetContentHeight();

  box->SetContentBox(newleft, newtop, box->GetContentWidth(), box->GetContentHeight());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::WriteBackTree(UI_ELEMENT* root, UI_LAYOUTBOX* tree)
* @brief      Write back tree
* @note       STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  root : Element tree to write geometry into (root and every descendant). Ignored if NULL.
* @param[in]  tree : UI_LAYOUTBOX tree "root" was built from (see BuildTree()). Ignored if NULL.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::WriteBackTree(UI_ELEMENT* root, UI_LAYOUTBOX* tree)
{
  if(!root || !tree) return;

  WriteBackRecursive(root, tree);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::WriteBackRecursive(UI_ELEMENT* element, UI_LAYOUTBOX* box)
* @brief      Write back recursive
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  element : Element to write "box"'s content box into, then recurse into its children.
* @param[in]  box : Box paired with "element" (see the SCOPE ADDENDUM in UI_LayoutEngine.h for the pairing rule).
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::WriteBackRecursive(UI_ELEMENT* element, UI_LAYOUTBOX* box)
{
  if(!element || !box) return;

  UI_CSSBOX cssbox;
  cssbox.left   = box->GetContentLeft();
  cssbox.top    = box->GetContentTop();
  cssbox.width  = box->GetContentWidth();
  cssbox.height = box->GetContentHeight();

  UI_CSSBox_Set(element, cssbox);

  // Forms/menus paint fills from VisibleRect, which is NOT updated by UI_CSSBox_Set (BoundaryLine/X/Y only).
  // After RunLayout moves a CSS-flow form (flex/grid card, etc.), sync VisibleRect to the new content box
  // or Draw_Form keeps filling the load-time rectangle while children sit at the new one.
  //
  // REGRESSION GUARD (UI_Options menu_horz): absolute XML forms often author visiblerect larger than their
  // auto-fit content (e.g. "0,0,550,70"). Unconditionally rewriting VisibleRect from the content box undoes
  // that authored window and changes the painted menu size. Only sync when THIS form is itself a flex/grid
  // container, or its father is (CSS flow owns the geometry). Pure absolute forms keep load-time VisibleRect.
  bool css_flow_owns_geometry = element->IsFlexContainer() || element->IsGridContainer();
  if(!css_flow_owns_geometry && element->GetFather())
    {
      css_flow_owns_geometry = element->GetFather()->IsFlexContainer() || element->GetFather()->IsGridContainer();
    }

  if(css_flow_owns_geometry &&
     (element->GetType() == UI_ELEMENT_TYPE_FORM || element->GetType() == UI_ELEMENT_TYPE_MENU))
    {
      UI_ELEMENT_FORM* form = (UI_ELEMENT_FORM*)element;
      UI_BOUNDARYLINE* vr   = form->GetVisibleRect();
      if(vr)
        {
          vr->x      = cssbox.left;
          vr->y      = cssbox.top + cssbox.height;   // GEN bottom-edge storage
          vr->width  = cssbox.width;
          vr->height = cssbox.height;
        }
    }

  XVECTOR<UI_ELEMENT*>* elementchildren = element->GetComposeElements();
  if(!elementchildren) return;

  XVECTOR<UI_LAYOUTBOX*>& boxchildren = box->GetChildren();

  // Pairs each element child with its box child by walking both in lockstep, skipping NULL element entries --
  // the SAME rule BuildTree() used to decide which elements got a box in the first place (see BuildTree()
  // above), so the two child lists line up index-for-index despite any NULL holes in "elementchildren".
  XDWORD boxindex = 0;
  for(XDWORD c=0; c<elementchildren->GetSize() && boxindex<boxchildren.GetSize(); c++)
    {
      UI_ELEMENT* childelement = elementchildren->Get(c);
      if(!childelement) continue;
      // Must match BuildTree()'s visibility skip so element/box child lists stay paired.
      if(!childelement->IsVisible()) continue;

      WriteBackRecursive(childelement, boxchildren.Get(boxindex));
      boxindex++;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool UI_LAYOUTENGINE::SubtreeUsesCSSFlowLayout(UI_ELEMENT* element)
* @brief      True if element or any compose-descendant is a flex or grid container.
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
* 
* @param[in]  element : 
* 
* @return     bool : true if is succesful.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LAYOUTENGINE::SubtreeUsesCSSFlowLayout(UI_ELEMENT* element)
{
  if(!element) return false;

  if(element->IsFlexContainer() || element->IsGridContainer()) return true;

  XVECTOR<UI_ELEMENT*>* children = element->GetComposeElements();
  if(!children) return false;

  for(XDWORD c=0; c<children->GetSize(); c++)
    {
      UI_ELEMENT* child = children->Get(c);
      if(child && SubtreeUsesCSSFlowLayout(child)) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::RunLayout(UI_ELEMENT* root, UI_LAYOUTSTRATEGY strategy)
* @brief      Run layout
* @note       STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  root : Element tree to lay out. Ignored if NULL.
* @param[in]  strategy : UI_LAYOUTSTRATEGY_LEGACY (no-op) or UI_LAYOUTSTRATEGY_CSS (runs the new engine).
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::RunLayout(UI_ELEMENT* root, UI_LAYOUTSTRATEGY strategy)
{
  if(!root) return;
  if(strategy != UI_LAYOUTSTRATEGY_CSS) return;   // LEGACY: existing geometry stands untouched, by design

  // Compat descendente: CreateLayouts() calls RunLayout on every top-level element. Trees with no
  // display:flex/grid (UI_Options absolute XML, virtual-keyboard keys, etc.) must not enter BuildTree/
  // WriteBack at all -- WriteBack was rewriting Form VisibleRect and BoundaryLine and visibly changed
  // menu_horz / chrome sizes even though ArrangeFlex never ran.
  if(!SubtreeUsesCSSFlowLayout(root)) return;

  UI_LAYOUTBOX* tree = BuildTree(root);
  if(!tree) return;

  // Normal flow: ONE arrange-then-recurse walk for flex and grid together. A separate full-tree
  // ApplyFlexLayout() followed by ApplyGridLayout() placed grid items AFTER flex had already finished
  // arranging their descendants against pre-grid geometry -- nested texts inside grid cells (e.g. uptime
  // tiles) stayed at CalculePosition coords and painted as clipped green fragments. Same risk for
  // flex-inside-grid the other way. ApplyFlowLayoutRecursive arranges THIS node (flex and/or grid), then
  // recurses so children see the parent's final content box.
  ApplyFlowLayoutRecursive(tree);

  ApplyPositioning(tree);
  WriteBackTree(root, tree);

  GEN_DELETE tree;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::ApplyFlexLayout(UI_LAYOUTBOX* root)
* @brief      Apply flex layout
* @note       STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  root : Tree to arrange in place (root and every descendant). Ignored if NULL.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::ApplyFlexLayout(UI_LAYOUTBOX* root)
{
  if(!root) return;

  ApplyFlexLayoutRecursive(root);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::ApplyFlowLayoutRecursive(UI_LAYOUTBOX* box)
* @brief      Apply flex and/or grid at this node, then recurse (single normal-flow pass for RunLayout).
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  box : Node being visited.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::ApplyFlowLayoutRecursive(UI_LAYOUTBOX* box)
{
  if(!box) return;

  // A node is never both flex and grid in authored CSS Lite today; if both flags were set, flex then grid
  // would still be wrong -- grid wins as the last arrange on this node before children run.
  if(box->IsFlexContainer()) ArrangeFlexChildren(box);
  if(box->IsGridContainer()) ArrangeGridChildren(box);

  XVECTOR<UI_LAYOUTBOX*>& children = box->GetChildren();
  for(XDWORD c=0; c<children.GetSize(); c++)
    {
      UI_LAYOUTBOX* child = children.Get(c);
      if(child) ApplyFlowLayoutRecursive(child);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::ApplyFlexLayoutRecursive(UI_LAYOUTBOX* box)
* @brief      Apply flex layout recursive
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  box : Node being visited.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::ApplyFlexLayoutRecursive(UI_LAYOUTBOX* box)
{
  if(!box) return;

  if(box->IsFlexContainer()) ArrangeFlexChildren(box);

  XVECTOR<UI_LAYOUTBOX*>& children = box->GetChildren();
  for(XDWORD c=0; c<children.GetSize(); c++)
    {
      UI_LAYOUTBOX* child = children.Get(c);
      if(child) ApplyFlexLayoutRecursive(child);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::ResolveFlexItemSizes(UI_LAYOUTBOX* container)
* @brief      Resolve flex item sizes
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  container : Flex container whose DIRECT children get their CONTENT main size grown/shrunk in place
*                          (cross size and position are untouched here -- see the SCOPE ADDENDUM in the header).
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::ResolveFlexItemSizes(UI_LAYOUTBOX* container)
{
  if(!container) return;

  XVECTOR<UI_LAYOUTBOX*>& children = container->GetChildren();
  XDWORD childcount = children.GetSize();
  if(childcount == 0) return;

  UI_FLEX_DIRECTION direction = container->GetFlexDirection();
  bool mainishorizontal = (direction == UI_FLEX_DIRECTION_ROW) || (direction == UI_FLEX_DIRECTION_ROW_REVERSE);

  double containermainsize = mainishorizontal ? container->GetContentWidth() : container->GetContentHeight();
  double maingap            = mainishorizontal ? container->GetColumnGap()    : container->GetRowGap();

  // Pass 1: each child's base ("hypothetical") main size, and the totals needed to decide grow vs shrink.
  XVECTOR<double> basesizes;
  double totalhypotheticalmain = 0.0;
  double sumgrow                = 0.0;
  double sumscaledshrink        = 0.0;

  for(XDWORD c=0; c<childcount; c++)
    {
      UI_LAYOUTBOX* child = children.Get(c);
      if(!child) { basesizes.Add(0.0); continue; }

      UI_LAYOUTBOX_INSET& basis = child->GetFlexBasis();
      double basesize = basis.specified ? basis.value : (mainishorizontal ? child->GetContentWidth() : child->GetContentHeight());
      basesizes.Add(basesize);

      UI_LAYOUTBOX_EDGES& padding = child->GetPadding();
      UI_LAYOUTBOX_EDGES& border  = child->GetBorder();
      UI_LAYOUTBOX_EDGES& margin  = child->GetMargin();

      double outerextra = mainishorizontal
        ? (padding.left + padding.right + border.left + border.right + margin.left + margin.right)
        : (padding.top  + padding.bottom + border.top  + border.bottom + margin.top  + margin.bottom);

      totalhypotheticalmain += basesize + outerextra;
      sumgrow                += child->GetFlexGrow();
      sumscaledshrink         += child->GetFlexShrink() * basesize;
    }
  totalhypotheticalmain += maingap * (double)(childcount - 1);

  double freespace = containermainsize - totalhypotheticalmain;

  // Pass 2: apply grow (freespace > 0) or shrink (freespace < 0), then write each child's final main size back.
  for(XDWORD c=0; c<childcount; c++)
    {
      UI_LAYOUTBOX* child = children.Get(c);
      if(!child) continue;

      double basesize  = basesizes.Get(c);
      double finalsize = basesize;

      if(freespace > 0.0 && sumgrow > 0.0)
        {
          finalsize = basesize + freespace * (child->GetFlexGrow() / sumgrow);
        }
      else if(freespace < 0.0 && sumscaledshrink > 0.0)
        {
          double scaledshrink = child->GetFlexShrink() * basesize;
          finalsize = basesize - (-freespace) * (scaledshrink / sumscaledshrink);
          if(finalsize < 0.0) finalsize = 0.0;
        }

      if(mainishorizontal) child->SetContentBox(child->GetContentLeft(), child->GetContentTop(), finalsize, child->GetContentHeight());
      else                 child->SetContentBox(child->GetContentLeft(), child->GetContentTop(), child->GetContentWidth(), finalsize);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_LAYOUTENGINE::FlexItemOuterMainSize(UI_LAYOUTBOX* child, bool mainishorizontal)
* @brief      Flex item outer main size
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  child : Item to measure.
* @param[in]  mainishorizontal : true if the main axis is horizontal (ROW/ROW_REVERSE).
*
* @return     double : "child"'s margin-box size along the MAIN axis.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUTENGINE::FlexItemOuterMainSize(UI_LAYOUTBOX* child, bool mainishorizontal)
{
  double left, top, width, height;
  child->GetMarginBox(left, top, width, height);

  return mainishorizontal ? width : height;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_LAYOUTENGINE::FlexItemOuterCrossSize(UI_LAYOUTBOX* child, bool mainishorizontal)
* @brief      Flex item outer cross size
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  child : Item to measure.
* @param[in]  mainishorizontal : true if the main axis is horizontal (ROW/ROW_REVERSE).
*
* @return     double : "child"'s margin-box size along the CROSS axis.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUTENGINE::FlexItemOuterCrossSize(UI_LAYOUTBOX* child, bool mainishorizontal)
{
  double left, top, width, height;
  child->GetMarginBox(left, top, width, height);

  return mainishorizontal ? height : width;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::PackFlexLine(XVECTOR<UI_LAYOUTBOX*>& children, XDWORD start, XDWORD count, bool mainishorizontal, bool reversed, UI_JUSTIFY_CONTENT justifycontent, double maingap, double linemainsize, double linemainstart, double linecrossstart, double linecrosssize, UI_ALIGN_ITEMS containeralignitems)
* @brief      Pack flex line
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  children : The flex container's full child list.
* @param[in]  start : Index of this line's first item within "children".
* @param[in]  count : Number of items in this line.
* @param[in]  mainishorizontal : true if the main axis is horizontal (ROW/ROW_REVERSE).
* @param[in]  reversed : true for ROW_REVERSE/COLUMN_REVERSE -- items within THIS line pack in reverse order.
* @param[in]  justifycontent : How free main-axis space within this one line is distributed.
* @param[in]  maingap : Main-axis gap between items.
* @param[in]  linemainsize : Main-axis size available to this line (always the container's full main size).
* @param[in]  linemainstart : Main-axis coordinate this line packs from.
* @param[in]  linecrossstart : Cross-axis coordinate this line starts at.
* @param[in]  linecrosssize : Cross-axis size available to this line -- align-items/align-self position (and, for
*             STRETCH, resize) each item within this span.
* @param[in]  containeralignitems : The container's align-items, used for every child whose own align-self is AUTO.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::PackFlexLine(XVECTOR<UI_LAYOUTBOX*>& children, XDWORD start, XDWORD count, bool mainishorizontal, bool reversed, UI_JUSTIFY_CONTENT justifycontent, double maingap, double linemainsize, double linemainstart, double linecrossstart, double linecrosssize, UI_ALIGN_ITEMS containeralignitems)
{
  if(count == 0) return;

  double totalitemsmain = 0.0;
  for(XDWORD i=0; i<count; i++)
    {
      UI_LAYOUTBOX* child = children.Get(start + i);
      if(!child) continue;

      totalitemsmain += FlexItemOuterMainSize(child, mainishorizontal);
    }
  totalitemsmain += maingap * (double)(count - 1);

  double freespace = linemainsize - totalitemsmain;

  double cursor  = linemainstart;
  double spacing = maingap;

  switch(justifycontent)
    {
      case UI_JUSTIFY_CONTENT_FLEX_END:
        cursor += freespace;
        break;

      case UI_JUSTIFY_CONTENT_CENTER:
        cursor += freespace / 2.0;
        break;

      case UI_JUSTIFY_CONTENT_SPACE_BETWEEN:
        if(count > 1) spacing = maingap + freespace / (double)(count - 1);
        break;

      case UI_JUSTIFY_CONTENT_SPACE_AROUND:
        spacing = maingap + freespace / (double)count;
        cursor += spacing / 2.0;
        break;

      case UI_JUSTIFY_CONTENT_SPACE_EVENLY:
        spacing = maingap + freespace / (double)(count + 1);
        cursor += spacing;
        break;

      case UI_JUSTIFY_CONTENT_FLEX_START:
      default:
        break;
    }

  for(XDWORD i=0; i<count; i++)
    {
      XDWORD index = reversed ? (count - 1 - i) : i;

      UI_LAYOUTBOX* child = children.Get(start + index);
      if(!child) continue;

      UI_LAYOUTBOX_EDGES& padding = child->GetPadding();
      UI_LAYOUTBOX_EDGES& border  = child->GetBorder();
      UI_LAYOUTBOX_EDGES& margin  = child->GetMargin();

      double mainleadingoffset   = mainishorizontal ? (margin.left + border.left + padding.left)
                                                     : (margin.top  + border.top  + padding.top);
      double crossleadingoffset  = mainishorizontal ? (margin.top  + border.top  + padding.top)
                                                     : (margin.left + border.left + padding.left);
      double crosstrailingoffset = mainishorizontal ? (margin.bottom + border.bottom + padding.bottom)
                                                     : (margin.right  + border.right  + padding.right);

      double outermain  = FlexItemOuterMainSize(child, mainishorizontal);
      double outercross = FlexItemOuterCrossSize(child, mainishorizontal);
      double contentcrosssize = mainishorizontal ? child->GetContentHeight() : child->GetContentWidth();

      // Resolve THIS item's effective cross-axis alignment: align-self overrides align-items when it isn't
      // AUTO; AUTO (the default) defers to the container's align-items. 0=start, 1=end, 2=center, 3=stretch.
      UI_ALIGN_SELF selfalign = child->GetAlignSelf();
      int alignmode;

      if(selfalign == UI_ALIGN_SELF_AUTO)
        {
          switch(containeralignitems)
            {
              case UI_ALIGN_ITEMS_FLEX_END: alignmode = 1; break;
              case UI_ALIGN_ITEMS_CENTER:   alignmode = 2; break;
              case UI_ALIGN_ITEMS_STRETCH:  alignmode = 3; break;
              case UI_ALIGN_ITEMS_FLEX_START:
              default: alignmode = 0; break;
            }
        }
      else
        {
          switch(selfalign)
            {
              case UI_ALIGN_SELF_FLEX_END: alignmode = 1; break;
              case UI_ALIGN_SELF_CENTER:   alignmode = 2; break;
              case UI_ALIGN_SELF_STRETCH:  alignmode = 3; break;
              case UI_ALIGN_SELF_FLEX_START:
              case UI_ALIGN_SELF_AUTO:
              default: alignmode = 0; break;
            }
        }

      if(alignmode == 3)   // stretch: grow the item's content cross size to fill the line
        {
          double stretchedcontentcross = linecrosssize - crossleadingoffset - crosstrailingoffset;
          if(stretchedcontentcross < 0.0) stretchedcontentcross = 0.0;

          contentcrosssize = stretchedcontentcross;
          outercross       = linecrosssize;
        }

      double crossfreespace = linecrosssize - outercross;
      double crossoffset    = 0.0;

      if(alignmode == 1)      crossoffset = crossfreespace;         // flex-end
      else if(alignmode == 2) crossoffset = crossfreespace / 2.0;   // center
      // start and stretch both keep crossoffset = 0

      double newmainpos  = cursor + mainleadingoffset;
      double newcrosspos = linecrossstart + crossoffset + crossleadingoffset;

      if(mainishorizontal) child->SetContentBox(newmainpos, newcrosspos, child->GetContentWidth(), contentcrosssize);
      else                 child->SetContentBox(newcrosspos, newmainpos, contentcrosssize, child->GetContentHeight());

      cursor += outermain + spacing;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::ArrangeFlexChildren(UI_LAYOUTBOX* container)
* @brief      Arrange flex children
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  container : Flex container whose DIRECT children get arranged, in place.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::ArrangeFlexChildren(UI_LAYOUTBOX* container)
{
  if(!container) return;

  XVECTOR<UI_LAYOUTBOX*>& children = container->GetChildren();
  XDWORD childcount = children.GetSize();
  if(childcount == 0) return;

  ResolveFlexItemSizes(container);   // grow/shrink each child's main size BEFORE packing (see the SCOPE ADDENDUM)

  UI_FLEX_DIRECTION direction = container->GetFlexDirection();
  bool mainishorizontal = (direction == UI_FLEX_DIRECTION_ROW) || (direction == UI_FLEX_DIRECTION_ROW_REVERSE);
  bool reversed          = (direction == UI_FLEX_DIRECTION_ROW_REVERSE) || (direction == UI_FLEX_DIRECTION_COLUMN_REVERSE);

  double containermainsize   = mainishorizontal ? container->GetContentWidth()  : container->GetContentHeight();
  double containermainstart  = mainishorizontal ? container->GetContentLeft()   : container->GetContentTop();
  double containercrosssize  = mainishorizontal ? container->GetContentHeight() : container->GetContentWidth();
  double containercrossstart = mainishorizontal ? container->GetContentTop()    : container->GetContentLeft();

  // CSS: "column-gap" separates children along the HORIZONTAL axis, "row-gap" along the VERTICAL axis -- which
  // one is the MAIN-axis gap (and which is the CROSS-axis gap, between wrapped lines) depends on flex-direction.
  double maingap  = mainishorizontal ? container->GetColumnGap() : container->GetRowGap();
  double crossgap = mainishorizontal ? container->GetRowGap()    : container->GetColumnGap();

  UI_FLEX_WRAP wrap = container->GetFlexWrap();

  if(wrap == UI_FLEX_WRAP_NOWRAP)
    {
      // single line spanning the container's whole cross size -- identical to the pre-wrap sub-step's behaviour
      // (see PackFlexLine's SCOPE note: for a single-line container, per CSS the line's cross size IS the
      // container's full cross size, regardless of how "tall" its items actually are)
      PackFlexLine(children, 0, childcount, mainishorizontal, reversed, container->GetJustifyContent(), maingap, containermainsize, containermainstart, containercrossstart, containercrosssize, container->GetAlignItems());
      return;
    }

  // --- flex-wrap: WRAP or WRAP_REVERSE -----------------------------------------------------------------------
  // Step 1: split "children" into lines, walking in COMPOSE order and starting a new line whenever the next
  // item's outer main size would overflow the container (an item that overflows alone still gets its own line
  // -- CSS never wraps a single item away from itself). Uses each item's CURRENT (already grow/shrink-resolved)
  // main size -- see the SCOPE ADDENDUM for why this does not re-run grow/shrink PER line.
  XVECTOR<XDWORD> linestart;
  XVECTOR<XDWORD> linecount;
  {
    XDWORD currentstart = 0;
    XDWORD currentcount = 0;
    double currentmain  = 0.0;

    for(XDWORD c=0; c<childcount; c++)
      {
        UI_LAYOUTBOX* child = children.Get(c);
        if(!child) continue;

        double outermain = FlexItemOuterMainSize(child, mainishorizontal);
        double addition  = (currentcount == 0) ? outermain : (maingap + outermain);

        if(currentcount > 0 && (currentmain + addition) > containermainsize)
          {
            linestart.Add(currentstart);
            linecount.Add(currentcount);

            currentstart = c;
            currentcount = 0;
            currentmain  = 0.0;
            addition     = outermain;
          }

        currentmain += addition;
        currentcount++;
      }

    if(currentcount > 0)
      {
        linestart.Add(currentstart);
        linecount.Add(currentcount);
      }
  }

  XDWORD numlines = linestart.GetSize();
  if(numlines == 0) return;

  // Step 2: each line's own cross size is the LARGEST outer cross size among its items (CSS: a line is only as
  // "tall" -- in a ROW container -- as its tallest item; no per-item cross growth/stretch yet, see SCOPE).
  XVECTOR<double> linecrosssizes;
  for(XDWORD l=0; l<numlines; l++)
    {
      double maxcross = 0.0;
      XDWORD start = linestart.Get(l);
      XDWORD count = linecount.Get(l);

      for(XDWORD i=0; i<count; i++)
        {
          UI_LAYOUTBOX* child = children.Get(start + i);
          if(!child) continue;

          double outercross = FlexItemOuterCrossSize(child, mainishorizontal);
          if(outercross > maxcross) maxcross = outercross;
        }

      linecrosssizes.Add(maxcross);
    }

  // Step 3: align-content distributes the lines themselves across the container's cross axis -- the same six
  // distributions justify-content uses for items, now applied to whole lines.
  double totallinescross = 0.0;
  for(XDWORD l=0; l<numlines; l++) totallinescross += linecrosssizes.Get(l);
  totallinescross += crossgap * (double)(numlines - 1);

  double crossfreespace = containercrosssize - totallinescross;

  double linecursor  = containercrossstart;
  double linespacing = crossgap;

  switch(container->GetAlignContent())
    {
      case UI_ALIGN_CONTENT_FLEX_END:
        linecursor += crossfreespace;
        break;

      case UI_ALIGN_CONTENT_CENTER:
        linecursor += crossfreespace / 2.0;
        break;

      case UI_ALIGN_CONTENT_SPACE_BETWEEN:
        if(numlines > 1) linespacing = crossgap + crossfreespace / (double)(numlines - 1);
        break;

      case UI_ALIGN_CONTENT_SPACE_AROUND:
        linespacing = crossgap + crossfreespace / (double)numlines;
        linecursor += linespacing / 2.0;
        break;

      case UI_ALIGN_CONTENT_SPACE_EVENLY:
        linespacing = crossgap + crossfreespace / (double)(numlines + 1);
        linecursor += linespacing;
        break;

      case UI_ALIGN_CONTENT_FLEX_START:
      default:
        break;
    }

  // Step 4: pack each line along the main axis (unchanged justify-content logic, via PackFlexLine) and place it
  // at its cross coordinate. WRAP_REVERSE stacks the lines in REVERSE cross order (the LAST line ends up first).
  bool wrapreverse = (wrap == UI_FLEX_WRAP_WRAP_REVERSE);

  for(XDWORD li=0; li<numlines; li++)
    {
      XDWORD lineindex = wrapreverse ? (numlines - 1 - li) : li;

      PackFlexLine(children, linestart.Get(lineindex), linecount.Get(lineindex), mainishorizontal, reversed, container->GetJustifyContent(), maingap, containermainsize, containermainstart, linecursor, linecrosssizes.Get(lineindex), container->GetAlignItems());

      linecursor += linecrosssizes.Get(lineindex) + linespacing;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::ApplyGridLayout(UI_LAYOUTBOX* root)
* @brief      Apply grid layout
* @note       STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  root : Tree to arrange in place (root and every descendant). Ignored if NULL.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::ApplyGridLayout(UI_LAYOUTBOX* root)
{
  if(!root) return;

  ApplyGridLayoutRecursive(root);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::ApplyGridLayoutRecursive(UI_LAYOUTBOX* box)
* @brief      Apply grid layout recursive
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  box : Node being visited.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::ApplyGridLayoutRecursive(UI_LAYOUTBOX* box)
{
  if(!box) return;

  if(box->IsGridContainer()) ArrangeGridChildren(box);

  XVECTOR<UI_LAYOUTBOX*>& children = box->GetChildren();
  for(XDWORD c=0; c<children.GetSize(); c++)
    {
      UI_LAYOUTBOX* child = children.Get(c);
      if(child) ApplyGridLayoutRecursive(child);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_LAYOUTENGINE::ResolveGridTrackSize(UI_GRIDTRACK track, double containersize)
* @brief      Resolve grid track size
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  track : Track to resolve. Must NOT be UI_GRID_TRACK_UNIT_FR -- an `fr` track has no size in
*             isolation (it needs the OTHER tracks' sizes to know how much free space is left to share), so it
*             is resolved only by ResolveTrackSizesWithFr(), never through this single-track helper.
* @param[in]  containersize : Grid container's content size on the axis this track belongs to.
*
* @return     double : Resolved track size, in pixels.
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUTENGINE::ResolveGridTrackSize(UI_GRIDTRACK track, double containersize)
{
  switch(track.unit)
    {
      case UI_GRID_TRACK_UNIT_PERCENT: return (track.value / 100.0) * containersize;
      case UI_GRID_TRACK_UNIT_FR:      return 0.0;   // never meaningful in isolation -- see the @param note above
      case UI_GRID_TRACK_UNIT_PX:
      default:                          return track.value;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::ResolveTrackSizesWithFr(XVECTOR<UI_GRIDTRACK>& tracks, XDWORD count, double fullcontainersize, double reservedspace, XVECTOR<double>& outsizes)
* @brief      Resolve track sizes with fr
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  tracks : Full track list (only the first "count" entries are resolved -- lets a caller cap how
*             many EXPLICIT row tracks apply when there are fewer used rows than declared tracks).
* @param[in]  count : Number of tracks, from the start of "tracks", to resolve.
* @param[in]  fullcontainersize : Grid container's FULL content size on this axis -- what UI_GRID_TRACK_UNIT_PERCENT
*             tracks resolve against (CSS: percentages are relative to the grid container, never to the
*             post-gap/post-fr "free space").
* @param[in]  reservedspace : Space on this axis already spoken for by things OTHER than these "count" tracks
*             (the axis's total gap, plus -- for rows -- whatever the container's IMPLICIT rows already consume).
*             Subtracted from "fullcontainersize" before what remains is shared among `fr` tracks (and among any
*             minmax() track whose max bound is `fr` -- see the SCOPE ADDENDUM).
* @param[out] outsizes : Receives exactly "count" resolved sizes, one per track, in the same order as "tracks".
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::ResolveTrackSizesWithFr(XVECTOR<UI_GRIDTRACK>& tracks, XDWORD count, double fullcontainersize, double reservedspace, XVECTOR<double>& outsizes)
{
  double fixedsum = 0.0;
  double frtotal  = 0.0;

  // Pass 1: every track contributes a FLOOR to "fixedsum" (what it will occupy no matter what) and, if it can
  // still grow, a weight to "frtotal". A simple FR track's floor is 0 (it grows purely from free space); a
  // minmax() track's floor is always its resolved MIN, and it additionally grows (weight = its max `fr` value)
  // only when its max bound IS `fr` -- otherwise its floor is immediately raised to its resolved MAX instead
  // (a fully fixed size, no growth) -- see the SCOPE ADDENDUM for why.
  for(XDWORD i=0; i<count; i++)
    {
      UI_GRIDTRACK track = tracks.Get(i);

      if(!track.isminmax)
        {
          if(track.unit == UI_GRID_TRACK_UNIT_FR) frtotal += track.value;
          else                                     fixedsum += ResolveGridTrackSize(track, fullcontainersize);
          continue;
        }

      UI_GRIDTRACK minpart; minpart.unit = track.minunit; minpart.value = track.minvalue;
      double minresolved = ResolveGridTrackSize(minpart, fullcontainersize);

      if(track.maxunit == UI_GRID_TRACK_UNIT_FR)
        {
          fixedsum += minresolved;      // reserve just the floor -- the rest comes from its fr share, in Pass 2
          frtotal  += track.maxvalue;
        }
      else
        {
          UI_GRIDTRACK maxpart; maxpart.unit = track.maxunit; maxpart.value = track.maxvalue;
          double maxresolved = ResolveGridTrackSize(maxpart, fullcontainersize);

          fixedsum += (maxresolved > minresolved) ? maxresolved : minresolved;   // fully fixed -- no fr weight
        }
    }

  double freespace = fullcontainersize - reservedspace - fixedsum;
  if(freespace < 0.0) freespace = 0.0;   // never a negative share -- an overflowing fr track simply gets 0

  // Pass 2: same per-track logic, now producing each track's FINAL resolved size.
  for(XDWORD i=0; i<count; i++)
    {
      UI_GRIDTRACK track = tracks.Get(i);

      if(!track.isminmax)
        {
          if(track.unit == UI_GRID_TRACK_UNIT_FR) outsizes.Add(frtotal > 0.0 ? freespace * (track.value / frtotal) : 0.0);
          else                                     outsizes.Add(ResolveGridTrackSize(track, fullcontainersize));
          continue;
        }

      UI_GRIDTRACK minpart; minpart.unit = track.minunit; minpart.value = track.minvalue;
      double minresolved = ResolveGridTrackSize(minpart, fullcontainersize);

      if(track.maxunit == UI_GRID_TRACK_UNIT_FR)
        {
          double frshare = (frtotal > 0.0) ? freespace * (track.maxvalue / frtotal) : 0.0;
          outsizes.Add(minresolved + frshare);   // floor, plus its share of what's left
        }
      else
        {
          UI_GRIDTRACK maxpart; maxpart.unit = track.maxunit; maxpart.value = track.maxvalue;
          double maxresolved = ResolveGridTrackSize(maxpart, fullcontainersize);

          outsizes.Add((maxresolved > minresolved) ? maxresolved : minresolved);
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_LAYOUTENGINE::GridItemOuterHeight(UI_LAYOUTBOX* child)
* @brief      Grid item outer height
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  child : Grid item to measure.
*
* @return     double : Its margin-box height (content height + its own top/bottom margin+border+padding).
*
* --------------------------------------------------------------------------------------------------------------------*/
double UI_LAYOUTENGINE::GridItemOuterHeight(UI_LAYOUTBOX* child)
{
  double left, top, width, height;

  child->GetMarginBox(left, top, width, height);

  return height;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::ArrangeGridChildren(UI_LAYOUTBOX* container)
* @brief      Arrange grid children
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in]  container : Grid container whose DIRECT children get arranged, in place.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::ArrangeGridChildren(UI_LAYOUTBOX* container)
{
  if(!container) return;

  XVECTOR<UI_LAYOUTBOX*>& children = container->GetChildren();
  XDWORD childcount = children.GetSize();
  if(childcount == 0) return;

  XVECTOR<UI_GRIDTRACK>& columntracks = container->GetGridColumnTracks();
  XDWORD numcolumns = columntracks.GetSize();
  if(numcolumns == 0) return;   // no columns defined -- nothing to place (see SCOPE)

  double containerleft   = container->GetContentLeft();
  double containertop    = container->GetContentTop();
  double containerwidth  = container->GetContentWidth();
  double containerheight = container->GetContentHeight();

  double columngap = container->GetColumnGap();
  double rowgap     = container->GetRowGap();

  // --- column widths + origins -----------------------------------------------------------------------------
  double columngaptotal = (numcolumns > 1) ? columngap * (double)(numcolumns - 1) : 0.0;

  XVECTOR<double> columnwidths;
  ResolveTrackSizesWithFr(columntracks, numcolumns, containerwidth, columngaptotal, columnwidths);

  XVECTOR<double> columnorigins;
  {
    double cursor = 0.0;
    for(XDWORD c=0; c<numcolumns; c++)
      {
        columnorigins.Add(cursor);
        cursor += columnwidths.Get(c) + columngap;
      }
  }

  // --- auto-placement (row-major, "sparse" packing -- see SCOPE): walk children in compose order, giving each
  //     one the first free (row, column) able to fit its own column/row span without overlapping an EARLIER
  //     item's span. "occupied" is a flat row-major bool grid, grown one row at a time as spans reach into rows
  //     not seen yet; it ends up exactly "numrows" rows tall once every child is placed.
  XVECTOR<bool> occupied;
  XDWORD occupiedrows = 0;

  XVECTOR<XDWORD> itemcolumn;
  XVECTOR<XDWORD> itemrow;
  XVECTOR<XDWORD> itemcolspan;
  XVECTOR<XDWORD> itemrowspan;

  XDWORD cursorrow = 0;
  XDWORD cursorcol = 0;

  for(XDWORD i=0; i<childcount; i++)
    {
      UI_LAYOUTBOX* child = children.Get(i);

      XDWORD colspan = child ? child->GetGridColumnSpan() : 1;
      if(colspan < 1) colspan = 1;
      if(colspan > numcolumns) colspan = numcolumns;   // a span wider than the grid simply fills the row (SCOPE)

      XDWORD rowspan = child ? child->GetGridRowSpan() : 1;
      if(rowspan < 1) rowspan = 1;

      for(;;)
        {
          if(cursorcol + colspan > numcolumns) { cursorcol = 0; cursorrow++; continue; }

          EnsureGridRows(occupied, occupiedrows, cursorrow + rowspan, numcolumns);

          if(GridCellsOccupied(occupied, numcolumns, cursorrow, cursorcol, rowspan, colspan))
            {
              cursorcol++;
              if(cursorcol >= numcolumns) { cursorcol = 0; cursorrow++; }
              continue;
            }

          break;   // (cursorrow, cursorcol) can fit this item's whole span, unobstructed
        }

      MarkGridCellsOccupied(occupied, numcolumns, cursorrow, cursorcol, rowspan, colspan);

      itemcolumn.Add(cursorcol);
      itemrow.Add(cursorrow);
      itemcolspan.Add(colspan);
      itemrowspan.Add(rowspan);

      cursorcol += colspan;
      if(cursorcol >= numcolumns) { cursorcol = 0; cursorrow++; }
    }

  XDWORD numrows = occupiedrows;

  // --- row heights: EXPLICIT tracks (grid-template-rows) are resolved together, `fr` included, exactly like
  //     columns above; any row beyond them is IMPLICIT, sized to the tallest item placed into it (see SCOPE).
  //     Free space for the explicit tracks' `fr` distribution excludes whatever the implicit rows already
  //     consume, computed FIRST for that reason. Only SINGLE-ROW items (rowspan == 1) contribute to an
  //     implicit row's size -- a spanning item's own height is not distributed across the rows it spans (see
  //     this sub-step's SCOPE ADDENDUM in UI_LayoutBox.h).
  XVECTOR<UI_GRIDTRACK>& rowtracks = container->GetGridRowTracks();
  XDWORD numexplicitrows = rowtracks.GetSize();
  if(numexplicitrows > numrows) numexplicitrows = numrows;   // never resolve more explicit tracks than used rows

  XVECTOR<double> implicitrowheights;   // indexed from row "numexplicitrows" onward (rowheights.Get(r) below)
  double implicitrowstotal = 0.0;

  for(XDWORD r=numexplicitrows; r<numrows; r++)
    {
      double tallest = 0.0;

      for(XDWORD i=0; i<childcount; i++)
        {
          if((itemrow.Get(i) != r) || (itemrowspan.Get(i) != 1)) continue;

          UI_LAYOUTBOX* child = children.Get(i);
          if(!child) continue;

          double outerheight = GridItemOuterHeight(child);
          if(outerheight > tallest) tallest = outerheight;
        }

      implicitrowheights.Add(tallest);
      implicitrowstotal += tallest;
    }

  double rowgaptotal = (numrows > 1) ? rowgap * (double)(numrows - 1) : 0.0;

  XVECTOR<double> explicitrowheights;
  ResolveTrackSizesWithFr(rowtracks, numexplicitrows, containerheight, rowgaptotal + implicitrowstotal, explicitrowheights);

  XVECTOR<double> rowheights;
  for(XDWORD r=0; r<numrows; r++)
    {
      if(r < numexplicitrows) rowheights.Add(explicitrowheights.Get(r));
      else                    rowheights.Add(implicitrowheights.Get(r - numexplicitrows));
    }

  XVECTOR<double> roworigins;
  {
    double cursor = 0.0;
    for(XDWORD r=0; r<numrows; r++)
      {
        roworigins.Add(cursor);
        cursor += rowheights.Get(r) + rowgap;
      }
  }

  // --- place every child, stretched to fill its (possibly spanned) cell minus its own margin/border/padding
  //     (see SCOPE) -- a spanning item's cell simply sums the widths/heights of every track it spans, plus the
  //     gaps BETWEEN them (span count - 1 gaps).
  for(XDWORD i=0; i<childcount; i++)
    {
      UI_LAYOUTBOX* child = children.Get(i);
      if(!child) continue;

      XDWORD column  = itemcolumn.Get(i);
      XDWORD row      = itemrow.Get(i);
      XDWORD colspan = itemcolspan.Get(i);
      XDWORD rowspan  = itemrowspan.Get(i);

      double cellwidth = 0.0;
      for(XDWORD c=column; c<column+colspan; c++) cellwidth += columnwidths.Get(c);
      cellwidth += columngap * (double)(colspan - 1);

      double cellheight = 0.0;
      for(XDWORD r=row; r<row+rowspan; r++) cellheight += rowheights.Get(r);
      cellheight += rowgap * (double)(rowspan - 1);

      double cellleft = containerleft + columnorigins.Get(column);
      double celltop   = containertop  + roworigins.Get(row);

      UI_LAYOUTBOX_EDGES& padding = child->GetPadding();
      UI_LAYOUTBOX_EDGES& border  = child->GetBorder();
      UI_LAYOUTBOX_EDGES& margin  = child->GetMargin();

      double leftinset   = margin.left + border.left + padding.left;
      double rightinset  = margin.right + border.right + padding.right;
      double topinset    = margin.top + border.top + padding.top;
      double bottominset = margin.bottom + border.bottom + padding.bottom;

      double contentwidth  = cellwidth  - leftinset - rightinset;
      double contentheight = cellheight - topinset  - bottominset;
      if(contentwidth  < 0.0) contentwidth  = 0.0;
      if(contentheight < 0.0) contentheight = 0.0;

      child->SetContentBox(cellleft + leftinset, celltop + topinset, contentwidth, contentheight);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::EnsureGridRows(XVECTOR<bool>& occupied, XDWORD& occupiedrows, XDWORD targetrows, XDWORD numcolumns)
* @brief      Ensure grid rows
* @note       INTERNAL / STATIC
* @ingroup    USERINTERFACE
*
* @param[in,out] occupied : Flat row-major occupancy grid ("numcolumns" bools per row), grown in place.
* @param[in,out] occupiedrows : Number of rows currently represented in "occupied"; updated to "targetrows" if
*                smaller.
* @param[in]  targetrows : Minimum number of rows "occupied" must cover after this call.
* @param[in]  numcolumns : Columns per row -- how many "false" entries one new row adds.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::EnsureGridRows(XVECTOR<bool>& occupied, XDWORD& occupiedrows, XDWORD targetrows, XDWORD numcolumns)
{
  while(occupiedrows < targetrows)
    {
      for(XDWORD c=0; c<numcolumns; c++) occupied.Add(false);
      occupiedrows++;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_LAYOUTENGINE::GridCellsOccupied(XVECTOR<bool>& occupied, XDWORD numcolumns, XDWORD row, XDWORD col, XDWORD rowspan, XDWORD colspan)
* @brief      Grid cells occupied
* @note       INTERNAL / STATIC -- the caller must have already grown "occupied" (EnsureGridRows()) to cover
*             every row this call inspects.
* @ingroup    USERINTERFACE
*
* @param[in]  occupied : Flat row-major occupancy grid.
* @param[in]  numcolumns : Columns per row.
* @param[in]  row : First row of the span to check.
* @param[in]  col : First column of the span to check.
* @param[in]  rowspan : Number of rows the span covers.
* @param[in]  colspan : Number of columns the span covers.
*
* @return     bool : true if ANY cell in the span is already occupied.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool UI_LAYOUTENGINE::GridCellsOccupied(XVECTOR<bool>& occupied, XDWORD numcolumns, XDWORD row, XDWORD col, XDWORD rowspan, XDWORD colspan)
{
  for(XDWORD r=row; r<row+rowspan; r++)
    {
      for(XDWORD c=col; c<col+colspan; c++)
        {
          if(occupied.Get(r * numcolumns + c)) return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_LAYOUTENGINE::MarkGridCellsOccupied(XVECTOR<bool>& occupied, XDWORD numcolumns, XDWORD row, XDWORD col, XDWORD rowspan, XDWORD colspan)
* @brief      Mark grid cells occupied
* @note       INTERNAL / STATIC -- the caller must have already grown "occupied" (EnsureGridRows()) to cover
*             every row this call marks.
* @ingroup    USERINTERFACE
*
* @param[in,out] occupied : Flat row-major occupancy grid, marked in place.
* @param[in]  numcolumns : Columns per row.
* @param[in]  row : First row of the span to mark.
* @param[in]  col : First column of the span to mark.
* @param[in]  rowspan : Number of rows the span covers.
* @param[in]  colspan : Number of columns the span covers.
*
* --------------------------------------------------------------------------------------------------------------------*/
void UI_LAYOUTENGINE::MarkGridCellsOccupied(XVECTOR<bool>& occupied, XDWORD numcolumns, XDWORD row, XDWORD col, XDWORD rowspan, XDWORD colspan)
{
  for(XDWORD r=row; r<row+rowspan; r++)
    {
      for(XDWORD c=col; c<col+colspan; c++)
        {
          occupied.Set(r * numcolumns + c, true);
        }
    }
}
