/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_Element_Combo.h
*
* @class      UI_ELEMENT_COMBO
* @brief      Edit field with a trailing drop button that opens a selectable item menu.
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
*---------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"
#include "XString.h"

#include "UI_Element_EditText.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define UI_ELEMENT_COMBO_DROP_WIDTH             26.0
#define UI_ELEMENT_COMBO_OPTION_HEIGHT          24.0
#define UI_ELEMENT_COMBO_OPTION_MARGIN_W        6.0
#define UI_ELEMENT_COMBO_OPTION_MARGIN_H        1.0   // 1px gap so row separators stay visible
#define UI_ELEMENT_COMBO_OPTION_SEP_H           1.0

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class UI_ELEMENT_MENU;
class UI_ELEMENT_OPTION;
class UI_LAYOUT;


class UI_ELEMENT_COMBO_ITEM
{
  public:

                                    UI_ELEMENT_COMBO_ITEM           ();
    virtual                        ~UI_ELEMENT_COMBO_ITEM           ();

    XSTRING                         display;                        // single-column fallback (also columns[0] mirror)
    XSTRING                         value;
    XVECTOR<XSTRING*>               columns;                        // owned column strings (tabulated row)

    void                            ClearColumns                    ();
    bool                            AddColumn                       (XCHAR* text);
    int                             GetColumnCount                  ();

  private:

    void                            Clean                           ();
};


class UI_ELEMENT_COMBO : public UI_ELEMENT_EDITTEXT
{
  public:

                                    UI_ELEMENT_COMBO                ();
    virtual                        ~UI_ELEMENT_COMBO                ();

    virtual double                  GetSpinWidth                    ();
    virtual int                     HitTestSpin                     (int design_x, int design_y);

    bool                            HitTestDropButton               (int design_x, int design_y);

    void                            ClearItems                      ();
    bool                            AddItem                         (XCHAR* display, XCHAR* value);
    bool                            AddItem                         (XSTRING& display, XSTRING& value);
    // Multi-column row: drawn at tab stops within the edit-field width (default menu width).
    bool                            AddItemColumns                  (XCHAR* value, XCHAR* col0, XCHAR* col1 = NULL, XCHAR* col2 = NULL);
    int                             GetItemCount                    ();
    UI_ELEMENT_COMBO_ITEM*          GetItem                         (int index);
    double                          GetDropWidth                    ();

    UI_ELEMENT_MENU*                Get_UIMenu                      ();
    bool                            EnsureMenu                      (UI_LAYOUT* layout);
    bool                            RebuildMenu                     ();
    bool                            OpenMenu                        ();
    bool                            CloseMenu                       ();
    bool                            ToggleMenu                      ();
    bool                            IsMenuVisible                   ();
    bool                            IsPointInMenu                   (int design_x, int design_y);

    // Apply menu option selection: writes item value into the edit text.
    bool                            ApplyMenuOption                 (UI_ELEMENT* option_element);

    // Mini-modal helpers: close every open combo popup under root (except optional keep).
    static bool                     CloseAllVisibleMenus            (UI_ELEMENT* root, UI_ELEMENT_COMBO* keep_open = NULL);
    static UI_ELEMENT_COMBO*        FindOwnerOfMenu                 (UI_ELEMENT* menu_or_child);
    static bool                     ElementIsInsideOpenComboMenu    (UI_ELEMENT* element);
    // Open dropdown under root whose AABB contains (design_x,design_y), or NULL.
    // Used when the pointer is outside the host form AABB (modal overhang).
    static UI_ELEMENT_MENU*         FindOpenMenuUnderPoint          (UI_ELEMENT* root, int design_x, int design_y);

  private:

    void                            Clean                           ();
    bool                            LayoutMenuUnderCombo            ();
    bool                            SnapshotRestoreHostFormSize     (bool restore);
    // Re-apply tabulated column boxes after CalculeBoundaryLine (which recenters texts).
    bool                            ApplyOptionColumns              ();
    // Popup plate/ink = edit idle (never focus / error wash on the live field).
    bool                            SyncMenuChrome                  ();
    bool                            ResolveMenuIdleColors           (XBYTE& fill_r, XBYTE& fill_g, XBYTE& fill_b,
                                                                     XBYTE& ink_r, XBYTE& ink_g, XBYTE& ink_b);

    UI_ELEMENT_MENU*                UImenu;
    bool                            menu_owned;
    XVECTOR<UI_ELEMENT_COMBO_ITEM*> items;
    // Idle option ink cached by SyncMenuChrome for RebuildMenu.
    XBYTE                           menu_idle_ink_r;
    XBYTE                           menu_idle_ink_g;
    XBYTE                           menu_idle_ink_b;

    // Host form size lock while the popup is open (form must not resize for the menu).
    double                          host_lock_w;
    double                          host_lock_h;
    double                          host_lock_vis_w;
    double                          host_lock_vis_h;
    bool                            host_size_locked;
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
