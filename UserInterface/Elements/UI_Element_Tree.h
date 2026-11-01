/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_Element_Tree.h
*
* @class      UI_ELEMENT_TREE
* @brief      Generic hierarchical tree (unlimited nesting, [+]/[-] boxes, dotted guides, H/V scroll, selection)
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"
#include "XString.h"

#include "UI_Property_Scrolleable.h"
#include "UI_Element.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define UI_ELEMENT_TREE_DEFAULT_ROWHEIGHT       20.0f
#define UI_ELEMENT_TREE_DEFAULT_INDENTWIDTH     18.0f
#define UI_ELEMENT_TREE_BOXSIZE                 9.0f
#define UI_ELEMENT_TREE_PADDING_X               8.0f
#define UI_ELEMENT_TREE_PADDING_Y               4.0f

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

struct UI_ELEMENT_TREE_NODE
{
                                      UI_ELEMENT_TREE_NODE          ();
                                     ~UI_ELEMENT_TREE_NODE          ();

  XSTRING                             text;
  bool                                expanded;
  XVECTOR<UI_ELEMENT_TREE_NODE*>      children;
  UI_ELEMENT_TREE_NODE*               father;

  bool                                HasChildren                   ()    { return children.GetSize() ? true : false; }

  private:

  void                                Clean                         ();
};


class UI_ELEMENT_TREE : public UI_ELEMENT, public UI_PROPERTY_SCROLLEABLE
{
  public:
                                      UI_ELEMENT_TREE               ();
    virtual                          ~UI_ELEMENT_TREE               ();

    XDWORD                            GetSizeFont                   ();
    void                              SetSizeFont                   (XDWORD sizefont);

    double                            GetRowHeight                  ();
    void                              SetRowHeight                  (double rowheight);

    double                            GetIndentWidth                ();
    void                              SetIndentWidth                (double indentwidth);

    double                            GetContentHeight              ();
    void                              SetContentHeight              (double contentheight);

    double                            GetContentWidth               ();
    void                              SetContentWidth               (double contentwidth);

    // Structure. father == NULL adds a top-level node.
    UI_ELEMENT_TREE_NODE*             AddNode                       (UI_ELEMENT_TREE_NODE* father, XCHAR* text);
    bool                              RemoveNode                    (UI_ELEMENT_TREE_NODE* node);
    bool                              Clear                         ();

    XDWORD                            GetRootCount                  ();
    UI_ELEMENT_TREE_NODE*             GetRoot                       (XDWORD index = 0);
    XVECTOR<UI_ELEMENT_TREE_NODE*>*   GetRoots                      ();
    XDWORD                            GetNodeDepth                  (UI_ELEMENT_TREE_NODE* node);

    // Selection.
    UI_ELEMENT_TREE_NODE*             GetSelected                   ();
    bool                              SetSelected                   (UI_ELEMENT_TREE_NODE* node);

    // Expansion.
    bool                              Expand                        (UI_ELEMENT_TREE_NODE* node);
    bool                              Collapse                      (UI_ELEMENT_TREE_NODE* node);
    bool                              Toggle                        (UI_ELEMENT_TREE_NODE* node);

    // Flattened visible rows (only nodes whose ancestors are all expanded).
    XDWORD                            GetVisibleRowCount            ();
    UI_ELEMENT_TREE_NODE*             GetVisibleNode                (XDWORD row);
    int                               GetVisibleRowOfNode           (UI_ELEMENT_TREE_NODE* node);

    // Hit-test (local coordinates: x from the left edge, y from the top edge of the element box, in design px).
    // Current vertical and horizontal scroll displacements are taken into account.
    UI_ELEMENT_TREE_NODE*             HitTestExpandBox              (double local_x, double local_y);
    UI_ELEMENT_TREE_NODE*             HitTestNode                   (double local_x, double local_y);
    UI_ELEMENT_TREE_NODE*             HitTestRow                    (double local_y);
    bool                              ScreenToLocal                 (int screen_x, int screen_y, double& local_x, double& local_y);

    // Layout metrics (shared by draw and hit-test; local to the box, without scroll).
    double                            GetBoxCenterX                 (XDWORD depth);
    double                            GetTextX                      (XDWORD depth);
    double                            GetRowTop                     (XDWORD row);                    // vertical scroll applied

    // Applies a click at local coordinates: expand box toggles; label selects. Returns true if something changed.
    bool                              ClickAt                       (double local_x, double local_y);

  private:

    void                              Clean                         ();

    void                              Flatten                       ();
    void                              Flatten_Node                  (UI_ELEMENT_TREE_NODE* node);
    bool                              IsOwned                       (XVECTOR<UI_ELEMENT_TREE_NODE*>* list, UI_ELEMENT_TREE_NODE* node);
    bool                              IsInTree                      (UI_ELEMENT_TREE_NODE* node);

    XDWORD                            sizefont;
    double                            rowheight;
    double                            indentwidth;
    double                            contentheight;
    double                            contentwidth;

    XVECTOR<UI_ELEMENT_TREE_NODE*>    roots;
    UI_ELEMENT_TREE_NODE*             selected;

    XVECTOR<UI_ELEMENT_TREE_NODE*>    visible_nodes;                                                 // cache (rebuilt on demand)
    bool                              visible_dirty;
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
