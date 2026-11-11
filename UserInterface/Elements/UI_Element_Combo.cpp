/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_Element_Combo.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "UI_Element_Combo.h"

#include "UI_Color.h"
#include "UI_Element_Form.h"
#include "UI_Element_Menu.h"
#include "UI_Element_Option.h"
#include "UI_Element_Text.h"
#include "UI_Layout.h"
#include "UI_Manager.h"
#include "UI_Property_Selectable.h"
#include "UI_Property_Scrolleable.h"
#include "UI_StyleSheet.h"

#include "XMemory_Control.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_COMBO_ITEM::UI_ELEMENT_COMBO_ITEM()
* @brief      Constructor
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_COMBO_ITEM::UI_ELEMENT_COMBO_ITEM()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_COMBO_ITEM::~UI_ELEMENT_COMBO_ITEM()
* @brief      Destructor
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_COMBO_ITEM::~UI_ELEMENT_COMBO_ITEM()
{
  ClearColumns();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_COMBO_ITEM::ClearColumns()
* @brief      Frees owned column strings.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_COMBO_ITEM::ClearColumns()
{
  columns.DeleteContents();
  columns.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO_ITEM::AddColumn(XCHAR* text)
* @brief      Appends one column cell.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO_ITEM::AddColumn(XCHAR* text)
{
  XSTRING* cell = GEN_NEW XSTRING();
  if(!cell) return false;
  if(text) cell->Set(text);
  columns.Add(cell);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int UI_ELEMENT_COMBO_ITEM::GetColumnCount()
* @brief      Number of tabulated columns.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
int UI_ELEMENT_COMBO_ITEM::GetColumnCount()
{
  return (int)columns.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_COMBO_ITEM::Clean()
* @brief      Clears item fields.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_COMBO_ITEM::Clean()
{
  display.Empty();
  value.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_COMBO::UI_ELEMENT_COMBO()
* @brief      Constructor
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_COMBO::UI_ELEMENT_COMBO()
{
  Clean();

  SetType(UI_ELEMENT_TYPE_COMBO);
  GetTypeString()->Set(NL("combo"));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_COMBO::~UI_ELEMENT_COMBO()
* @brief      Destructor
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_COMBO::~UI_ELEMENT_COMBO()
{
  // Only free the value/display list here. The dropdown menu is owned by the host
  // form compose (or layout) and is deleted once with that tree — never GEN_DELETE here.
  items.DeleteContents();
  items.DeleteAll();
  UImenu = NULL;
  menu_owned = false;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT_COMBO::GetDropWidth()
* @brief      Trailing combo chevron strip width.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT_COMBO::GetDropWidth()
{
  return UI_ELEMENT_COMBO_DROP_WIDTH;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         double UI_ELEMENT_COMBO::GetSpinWidth()
* @brief      Trailing controls: numeric +/- (when numeric) then combo drop.
* @ingroup    USERINTERFACE
*
* @return     double : Combined trailing strip width.
*
*---------------------------------------------------------------------------------------------------------------------*/
double UI_ELEMENT_COMBO::GetSpinWidth()
{
  double w = GetDropWidth();
  if(GetInputMode() == UI_ELEMENT_EDITTEXT_INPUTMODE_NUMERIC)
    {
      w += UI_ELEMENT_EDITTEXT_SPIN_WIDTH;
    }
  return w;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int UI_ELEMENT_COMBO::HitTestSpin(int design_x, int design_y)
* @brief      Hit-tests the numeric +/- strip (left of the combo drop when both are present).
* @ingroup    USERINTERFACE
*
* @return     int : 1 up, -1 down, 0 outside spin.
*
*---------------------------------------------------------------------------------------------------------------------*/
int UI_ELEMENT_COMBO::HitTestSpin(int design_x, int design_y)
{
  if(GetInputMode() != UI_ELEMENT_EDITTEXT_INPUTMODE_NUMERIC) return 0;
  if(!GetBoundaryLine()) return 0;

  double spin_w = UI_ELEMENT_EDITTEXT_SPIN_WIDTH;
  double drop_w = GetDropWidth();
  if(spin_w < 1.0) return 0;

  double width  = GetBoundaryLine()->width;
  double left   = UI_BOUNDARYLINE_EdgeLeft(GetXPosition(), width);
  double right  = left + width;
  double bottom = GetYPosition();
  double top    = GetTopY();
  double spin_r = right - drop_w;
  double spin_l = spin_r - spin_w;

  if((design_x < (int)spin_l) || (design_x >= (int)spin_r)) return 0;
  if((design_y < (int)top) || (design_y > (int)bottom)) return 0;

  double mid = top + ((bottom - top) * 0.5);
  if(design_y <= (int)mid) return 1;
  return -1;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::HitTestDropButton(int design_x, int design_y)
* @brief      True when the point is inside the trailing drop button (rightmost strip).
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::HitTestDropButton(int design_x, int design_y)
{
  if(!GetBoundaryLine()) return false;

  double drop_w = GetDropWidth();
  if(drop_w < 1.0) return false;

  // Same left edge convention as Draw_EditText (EdgeLeft), so the chevron strip
  // hit matches the painted drop button — especially when +/- is also present.
  double width  = GetBoundaryLine()->width;
  double left   = UI_BOUNDARYLINE_EdgeLeft(GetXPosition(), width);
  double right  = left + width;
  double bottom = GetYPosition();
  double top    = GetTopY();
  double drop_l = right - drop_w;

  if((design_x < (int)drop_l) || (design_x > (int)right)) return false;
  if((design_y < (int)top) || (design_y > (int)bottom)) return false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_COMBO::ClearItems()
* @brief      Removes all combo items and empties the linked menu.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_COMBO::ClearItems()
{
  items.DeleteContents();
  items.DeleteAll();

  if(UImenu)
    {
      UImenu->GetComposeElements()->DeleteContents();
      UImenu->GetComposeElements()->DeleteAll();
      UImenu->SetVisible(false);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::AddItem(XCHAR* display, XCHAR* value)
* @brief      Appends a display/value pair (menu rebuilt on Open/RebuildMenu).
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::AddItem(XCHAR* display, XCHAR* value)
{
  UI_ELEMENT_COMBO_ITEM* item = GEN_NEW UI_ELEMENT_COMBO_ITEM();
  if(!item) return false;

  if(display) item->display = display;
  if(value)   item->value   = value;
  if(display) item->AddColumn(display);

  items.Add(item);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::AddItem(XSTRING& display, XSTRING& value)
* @brief      Appends a display/value pair.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::AddItem(XSTRING& display, XSTRING& value)
{
  return AddItem(display.Get(), value.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::AddItemColumns(XCHAR* value, XCHAR* col0, XCHAR* col1, XCHAR* col2)
* @brief      Appends a multi-column row (tabulated inside the edit-field width).
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::AddItemColumns(XCHAR* value, XCHAR* col0, XCHAR* col1, XCHAR* col2)
{
  UI_ELEMENT_COMBO_ITEM* item = GEN_NEW UI_ELEMENT_COMBO_ITEM();
  if(!item) return false;

  if(value) item->value = value;
  if(col0)
    {
      item->display = col0;
      item->AddColumn(col0);
    }
  if(col1) item->AddColumn(col1);
  if(col2) item->AddColumn(col2);

  // Rebuild a single-line display for any consumer that still reads item->display.
  if(item->GetColumnCount() > 1)
    {
      item->display.Empty();
      for(int c=0; c<item->GetColumnCount(); c++)
        {
          XSTRING* cell = item->columns.Get(c);
          if(!cell) continue;
          if(c > 0) item->display.Add((XCHAR)' ');
          item->display += (*cell);
        }
    }

  items.Add(item);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int UI_ELEMENT_COMBO::GetItemCount()
* @brief      Number of items.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
int UI_ELEMENT_COMBO::GetItemCount()
{
  return (int)items.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_COMBO_ITEM* UI_ELEMENT_COMBO::GetItem(int index)
* @brief      Item at index, or NULL.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_COMBO_ITEM* UI_ELEMENT_COMBO::GetItem(int index)
{
  if(index < 0) return NULL;
  if(index >= (int)items.GetSize()) return NULL;
  return items.Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_MENU* UI_ELEMENT_COMBO::Get_UIMenu()
* @brief      Linked dropdown menu.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_MENU* UI_ELEMENT_COMBO::Get_UIMenu()
{
  return UImenu;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::EnsureMenu(UI_LAYOUT* layout)
* @brief      Creates an owned menu (once) and registers it on the layout.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::EnsureMenu(UI_LAYOUT* layout)
{
  if(UImenu) return true;

  UImenu = GEN_NEW UI_ELEMENT_MENU();
  if(!UImenu) return false;

  // Parent under the combo's father (config modal form), NOT under the combo:
  // Draw_EditText does not paint compose children, while Draw_Form does — and the menu
  // may extend past the form AABB (dropdown overflow). Lifetime: form compose owns it;
  // combo destructor must never GEN_DELETE the menu.
  menu_owned = false;

  XSTRING menuname;
  menuname.Format(NL("%s_menu"), GetName()->Get());
  UImenu->GetName()->Set(menuname);
  UImenu->SetVisible(false);

  UI_ELEMENT* host = GetFather();
  if(host)
    {
      UImenu->SetFather(host);
      host->GetComposeElements()->Add(UImenu);
    }
   else if(layout)
    {
      UImenu->SetFather(NULL);
      layout->Elements_Add(UImenu);
    }
   else
    {
      GEN_DELETE UImenu;
      UImenu = NULL;
      return false;
    }

  if(layout) UImenu->SetLayout(layout);

  SyncMenuChrome();

  // Popup AABB is sized to its options (RebuildMenu). Default FORM overflow AUTO
  // was enabling a semi-transparent overlay scrollbar even when everything fits —
  // that alpha track looked like a missing/transparent right edge of the menu.
  UImenu->Scroll_SetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, UI_OVERFLOW_HIDDEN);
  UImenu->Scroll_SetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL  , UI_OVERFLOW_HIDDEN);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::ResolveMenuIdleColors(...)
* @brief      Idle edit fill/ink for the dropdown (never focus or validation-error wash).
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::ResolveMenuIdleColors(XBYTE& fill_r, XBYTE& fill_g, XBYTE& fill_b,
                                             XBYTE& ink_r, XBYTE& ink_g, XBYTE& ink_b)
{
  // Dark-theme defaults (match common --edit-idle-bg / --label-ink tokens).
  fill_r = 64;  fill_g = 72;  fill_b = 84;
  ink_r  = 200; ink_g  = 208; ink_b  = 220;

  auto parse_rgb = [](XSTRING& csv, XBYTE& r, XBYTE& g, XBYTE& b) -> bool
    {
      if(csv.IsEmpty()) return false;
      UI_COLOR tmp;
      if(!tmp.SetFromString(csv.Get())) return false;
      r = tmp.GetRed();
      g = tmp.GetGreen();
      b = tmp.GetBlue();
      return true;
    };

  // Prefer live CSS tokens (ApplyColorTheme keeps --edit-idle-bg / --label-ink current).
  UI_LAYOUT* layout = GetLayout();
  if(layout && layout->GetStyleSheet())
    {
      XSTRING idle;
      XSTRING ink;
      const bool got_idle = layout->GetStyleSheet()->Variables_Get(NL("--edit-idle-bg"), idle) && parse_rgb(idle, fill_r, fill_g, fill_b);
      const bool got_ink  = layout->GetStyleSheet()->Variables_Get(NL("--label-ink"), ink) && parse_rgb(ink, ink_r, ink_g, ink_b);
      if(got_idle)
        {
          if(!got_ink)
            {
              const int lum = ((int)fill_r * 3 + (int)fill_g * 6 + (int)fill_b) / 10;
              if(lum >= 160) { ink_r = 40; ink_g = 44; ink_b = 52; }
            }
          return true;
        }
    }

  // No CSS: reuse the field fill only when it already looks like idle (not error / focus).
  if(IsBackgroundColorSet() && GetBackgroundColor() && (GetBackgroundColor()->GetAlpha() > 0))
    {
      const XBYTE cr = GetBackgroundColor()->GetRed();
      const XBYTE cg = GetBackgroundColor()->GetGreen();
      const XBYTE cb = GetBackgroundColor()->GetBlue();
      // Same error-red heuristic as Draw_EditText; focus tokens are near-black / near-white.
      const bool error_red   = ((cr > 140) && (cr < 185) && (cg < 70) && (cb < 70));
      const bool focus_dark  = ((cr <= 20) && (cg <= 20) && (cb <= 20));
      const bool focus_light = ((cr >= 240) && (cg >= 240) && (cb >= 240));
      if(!error_red && !focus_dark && !focus_light)
        {
          fill_r = cr; fill_g = cg; fill_b = cb;
          const int lum = ((int)fill_r * 3 + (int)fill_g * 6 + (int)fill_b) / 10;
          if(lum >= 160) { ink_r = 40; ink_g = 44; ink_b = 52; }
          else           { ink_r = 200; ink_g = 208; ink_b = 220; }
          return true;
        }

      // Error/focus: pick Dark vs Light idle from the frame token when present.
      if(IsBorderColorSet() && GetBorderColor())
        {
          const int flum = ((int)GetBorderColor()->GetRed() * 3 +
                            (int)GetBorderColor()->GetGreen() * 6 +
                            (int)GetBorderColor()->GetBlue()) / 10;
          if(flum >= 120)
            {
              fill_r = 210; fill_g = 214; fill_b = 220;
              ink_r  = 40;  ink_g  = 44;  ink_b  = 52;
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::SyncMenuChrome()
* @brief      Applies idle fill/border into the dropdown (never live error/focus wash).
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::SyncMenuChrome()
{
  if(!UImenu) return false;

  // Match idle combo chrome so the popup reads as part of the field — not the
  // validation-error red or :selected focus fill that may be on the edit right now.
  XBYTE br = 64;
  XBYTE bg = 72;
  XBYTE bb = 84;
  ResolveMenuIdleColors(br, bg, bb, menu_idle_ink_r, menu_idle_ink_g, menu_idle_ink_b);

  XSTRING fill;
  fill.Format(NL("%d,%d,%d,255"), br, bg, bb);
  if(UImenu->GetBackgroundColor())
    {
      UImenu->GetBackgroundColor()->SetFromString(fill.Get());
      UImenu->SetBackgroundColorSet(true);
    }
  // Keep legacy Color fill path in sync (pre-bckgrdcolor Draw_Form consumers).
  if(UImenu->GetColor())
    {
      UImenu->GetColor()->SetFromString(fill.Get());
    }

  if(IsBorderColorSet() && GetBorderColor())
    {
      UImenu->SetBorderWidth(GetBorderWidth() > 0.0 ? GetBorderWidth() : 1.0);
      XSTRING bcol;
      bcol.Format(NL("%d,%d,%d,%d"),
                  GetBorderColor()->GetRed(), GetBorderColor()->GetGreen(),
                  GetBorderColor()->GetBlue(), GetBorderColor()->GetAlpha());
      UImenu->SetBorderColorFromString(bcol);
    }
   else
    {
      // Match Dark/Light frame tokens (same as --frame-line) from the popup fill luminance.
      UImenu->SetBorderWidth(1.0);
      const int lum = ((int)br * 3 + (int)bg * 6 + (int)bb) / 10;
      UImenu->SetBorderColorFromString((lum >= 160) ? NL("140,150,165,255") : NL("70,80,95,255"));
    }
  UImenu->SetRoundRect(GetRoundRect() > 0 ? GetRoundRect() : 4);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::RebuildMenu()
* @brief      Rebuilds menu options from the current item list.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::RebuildMenu()
{
  if(!UImenu) return false;

  UImenu->GetComposeElements()->DeleteContents();
  UImenu->GetComposeElements()->DeleteAll();

  // Default popup width = edit field width (never grow to text extent).
  const double menu_w = GetBoundaryLine() ? GetBoundaryLine()->width : 160.0;

  const int n = (int)items.GetSize();
  if(n <= 0)
    {
      UImenu->GetBoundaryLine()->width  = menu_w;
      UImenu->GetBoundaryLine()->height = UI_ELEMENT_COMBO_OPTION_HEIGHT;
      UImenu->SetIntrinsicWidth(menu_w);
      UImenu->SetIntrinsicHeight(UI_ELEMENT_COMBO_OPTION_HEIGHT);
      return true;
    }

  int max_cols = 1;
  for(int i=0; i<n; i++)
    {
      UI_ELEMENT_COMBO_ITEM* item = items.Get(i);
      if(!item) continue;
      int nc = item->GetColumnCount();
      if(nc < 1) nc = 1;
      if(nc > max_cols) max_cols = nc;
    }

  double row_h = UI_ELEMENT_COMBO_OPTION_HEIGHT;
  double total_h = (row_h + UI_ELEMENT_COMBO_OPTION_MARGIN_H) * (double)n + UI_ELEMENT_COMBO_OPTION_MARGIN_H;
  if(total_h < row_h) total_h = row_h;
  UImenu->GetBoundaryLine()->width  = menu_w;
  UImenu->GetBoundaryLine()->height = total_h;
  UImenu->SetIntrinsicWidth(menu_w);
  UImenu->SetIntrinsicHeight(total_h);

  XDWORD sizefont = GetSizeFont() ? GetSizeFont() : 12;
  const double inner_w = menu_w - (UI_ELEMENT_COMBO_OPTION_MARGIN_W * 2.0);
  const double col_gap = 8.0;
  // Conservative advance so truncation keeps glyphs inside the column (overflow looked like
  // "WB.7" jammed against the next column).
  const double approx_advance = __MAX(7.0, (double)sizefont * 0.70);

  // Column fractions: 1 col=100%; 2 cols=42/58; 3 cols=30/48/22 (name / host / port).
  double frac[3] = { 1.0, 0.0, 0.0 };
  if(max_cols == 2) { frac[0] = 0.42; frac[1] = 0.58; }
  else if(max_cols >= 3) { frac[0] = 0.30; frac[1] = 0.48; frac[2] = 0.22; max_cols = 3; }

  double col_w[3] = { inner_w, 0.0, 0.0 };
  double col_x[3] = { 0.0, 0.0, 0.0 };
  {
    double gaps = col_gap * (double)(max_cols - 1);
    if(gaps < 0.0) gaps = 0.0;
    double usable = inner_w - gaps;
    if(usable < 8.0) usable = 8.0;
    double cursor = 0.0;
    for(int c=0; c<max_cols; c++)
      {
        col_w[c] = usable * frac[c];
        if(col_w[c] < 8.0) col_w[c] = 8.0;
        col_x[c] = cursor;
        cursor += col_w[c] + col_gap;
      }
  }

  auto truncate_to_width = [approx_advance](XSTRING& text, double width_px)
    {
      // Keep 2px slack so the last glyph cannot bleed into the next column.
      double usable = width_px - 2.0;
      if(usable < 6.0) { text.Empty(); return; }
      int max_chars = (int)(usable / approx_advance);
      if(max_chars < 1) max_chars = 1;
      if((int)text.GetSize() <= max_chars) return;
      if(max_chars <= 1) { text.DeleteCharacters(1, text.GetSize() - 1); return; }
      text.DeleteCharacters(max_chars - 1, text.GetSize() - (max_chars - 1));
      text.Add((XCHAR)'.');
      text.Add((XCHAR)'.');
    };

  for(int i=0; i<n; i++)
    {
      UI_ELEMENT_COMBO_ITEM* item = items.Get(i);
      if(!item) continue;

      XSTRING leyend;
      if(item->GetColumnCount() > 0 && item->columns.Get(0)) leyend = (*item->columns.Get(0));
       else leyend = item->display;
      truncate_to_width(leyend, col_w[0]);

      UI_ELEMENT_MENU_OPTION_CFG cfg;
      cfg.index = (XDWORD)(i + 1);
      cfg.nameoption.Format(NL("combo_item_%02d"), i);
      cfg.leyend = leyend;
      cfg.sizefont = sizefont;
      cfg.marginwidth  = UI_ELEMENT_COMBO_OPTION_MARGIN_W;
      cfg.marginheight = UI_ELEMENT_COMBO_OPTION_MARGIN_H;
      cfg.width  = menu_w;
      cfg.height = row_h;
      // Idle label ink (not white-on-error / focus overrides living on the edit field).
      cfg.colorstr.Format(NL("%d,%d,%d,255"), menu_idle_ink_r, menu_idle_ink_g, menu_idle_ink_b);

      UI_ELEMENT_OPTION* opt = UImenu->Option_AddText(cfg);
      if(!opt) continue;

      opt->SetSelectableState(UI_PROPERTY_SELECTABLE_STATE_ACTIVE);
      if(UImenu->GetBackgroundColor())
        {
          (*opt->GetBackgroundColor()) = (*UImenu->GetBackgroundColor());
          opt->SetBackgroundColorSet(true);
        }

      // Keep option box fixed to menu inner width (never grow to text extent).
      if(opt->GetBoundaryLine())
        {
          opt->GetBoundaryLine()->width = inner_w;
          opt->SetIntrinsicWidth(inner_w);
        }

      UI_ELEMENT_TEXT* first = opt->Get_UIText();
      if(first && first->GetBoundaryLine())
        {
          first->GetBoundaryLine()->x      = col_x[0];
          first->GetBoundaryLine()->width  = col_w[0];
          first->GetBoundaryLine()->y      = UI_ELEMENT_TYPE_ALIGN_CENTER;
          first->GetBoundaryLine()->height = UI_ELEMENT_TYPE_ALIGN_AUTO;
          first->SetIntrinsicWidth(col_w[0]);
        }

      for(int c=1; c<max_cols; c++)
        {
          XSTRING* cell = (c < item->GetColumnCount()) ? item->columns.Get(c) : NULL;
          if(!cell || cell->IsEmpty()) continue;

          XSTRING clipped = (*cell);
          truncate_to_width(clipped, col_w[c]);

          UI_ELEMENT_TEXT* t = GEN_NEW UI_ELEMENT_TEXT();
          if(!t) continue;

          t->SetFather(opt);
          t->GetName()->Format(NL("combo_item_%02d_c%d"), i, c);
          t->GetText()->Set(clipped.Get());
          t->SetSizeFont(sizefont);
          t->GetColor()->SetRed(menu_idle_ink_r);
          t->GetColor()->SetGreen(menu_idle_ink_g);
          t->GetColor()->SetBlue(menu_idle_ink_b);
          t->GetColor()->SetAlpha(255);
          t->GetBoundaryLine()->x      = col_x[c];
          t->GetBoundaryLine()->y      = UI_ELEMENT_TYPE_ALIGN_CENTER;
          t->GetBoundaryLine()->width  = col_w[c];
          t->GetBoundaryLine()->height = UI_ELEMENT_TYPE_ALIGN_AUTO;
          t->SetIntrinsicWidth(col_w[c]);
          t->SetMustReDraw(true);
          opt->GetComposeElements()->Add(t);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::ApplyOptionColumns()
* @brief      Re-applies tabulated column boxes/text after layout so columns stay aligned.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::ApplyOptionColumns()
{
  if(!UImenu || !UImenu->GetComposeElements() || !GetBoundaryLine()) return false;

  const double menu_w = GetBoundaryLine()->width;
  const double inner_w = menu_w - (UI_ELEMENT_COMBO_OPTION_MARGIN_W * 2.0);
  if(inner_w < 8.0) return false;

  const int n = (int)items.GetSize();
  if(n <= 0) return true;

  int max_cols = 1;
  for(int i=0; i<n; i++)
    {
      UI_ELEMENT_COMBO_ITEM* item = items.Get(i);
      if(!item) continue;
      int nc = item->GetColumnCount();
      if(nc < 1) nc = 1;
      if(nc > max_cols) max_cols = nc;
    }
  if(max_cols > 3) max_cols = 3;

  XDWORD sizefont = GetSizeFont() ? GetSizeFont() : 12;
  const double col_gap = 8.0;
  const double approx_advance = __MAX(7.0, (double)sizefont * 0.70);

  double frac[3] = { 1.0, 0.0, 0.0 };
  if(max_cols == 2) { frac[0] = 0.42; frac[1] = 0.58; }
  else if(max_cols >= 3) { frac[0] = 0.30; frac[1] = 0.48; frac[2] = 0.22; }

  double col_w[3] = { inner_w, 0.0, 0.0 };
  double col_x[3] = { 0.0, 0.0, 0.0 };
  {
    double gaps = col_gap * (double)(max_cols - 1);
    if(gaps < 0.0) gaps = 0.0;
    double usable = inner_w - gaps;
    if(usable < 8.0) usable = 8.0;
    double cursor = 0.0;
    for(int c=0; c<max_cols; c++)
      {
        col_w[c] = usable * frac[c];
        if(col_w[c] < 8.0) col_w[c] = 8.0;
        col_x[c] = cursor;
        cursor += col_w[c] + col_gap;
      }
  }

  auto truncate_to_width = [approx_advance](XSTRING& text, double width_px)
    {
      double usable = width_px - 2.0;
      if(usable < 6.0) { text.Empty(); return; }
      int max_chars = (int)(usable / approx_advance);
      if(max_chars < 1) max_chars = 1;
      if((int)text.GetSize() <= max_chars) return;
      if(max_chars <= 1) { text.DeleteCharacters(1, text.GetSize() - 1); return; }
      text.DeleteCharacters(max_chars - 1, text.GetSize() - (max_chars - 1));
      text.Add((XCHAR)'.');
      text.Add((XCHAR)'.');
    };

  int opt_index = 0;
  for(XDWORD i=0; i<UImenu->GetComposeElements()->GetSize(); i++)
    {
      UI_ELEMENT* child = UImenu->GetComposeElements()->Get(i);
      if(!child || child->GetType() != UI_ELEMENT_TYPE_OPTION || !child->GetBoundaryLine()) continue;

      UI_ELEMENT_OPTION* opt = (UI_ELEMENT_OPTION*)child;
      UI_ELEMENT_COMBO_ITEM* item = (opt_index < n) ? items.Get(opt_index) : NULL;
      opt_index++;
      if(!item) continue;

      // Collect primary label then extra column TEXTs (creation order).
      UI_ELEMENT_TEXT* texts[3] = { NULL, NULL, NULL };
      int ntexts = 0;
      UI_ELEMENT_TEXT* primary = opt->Get_UIText();
      if(primary && ntexts < 3) texts[ntexts++] = primary;
      if(opt->GetComposeElements())
        {
          for(XDWORD c=0; c<opt->GetComposeElements()->GetSize() && ntexts < 3; c++)
            {
              UI_ELEMENT* sub = opt->GetComposeElements()->Get(c);
              if(!sub || sub->GetType() != UI_ELEMENT_TYPE_TEXT) continue;
              if(sub == (UI_ELEMENT*)primary) continue;
              texts[ntexts++] = (UI_ELEMENT_TEXT*)sub;
            }
        }

      const double opt_x = opt->GetXPosition();
      for(int c=0; c<ntexts && c<max_cols; c++)
        {
          UI_ELEMENT_TEXT* t = texts[c];
          if(!t || !t->GetBoundaryLine()) continue;

          XSTRING cell;
          if(c < item->GetColumnCount() && item->columns.Get(c)) cell = (*item->columns.Get(c));
           else if(c == 0) cell = item->display;
          truncate_to_width(cell, col_w[c]);
          if(t->GetText()) t->GetText()->Set(cell.Get());

          t->GetBoundaryLine()->x      = col_x[c];
          t->GetBoundaryLine()->width  = col_w[c];
          t->GetBoundaryLine()->y      = UI_ELEMENT_TYPE_ALIGN_CENTER;
          t->GetBoundaryLine()->height = UI_ELEMENT_TYPE_ALIGN_AUTO;
          t->SetIntrinsicWidth(col_w[c]);
          // Absolute: same as CalculePosition default (father_x + local x).
          t->SetXPosition(opt_x + col_x[c]);
          t->SetMustReDraw(true);
        }
    }

  return true;
}


bool UI_ELEMENT_COMBO::LayoutMenuUnderCombo()
{
  if(!UImenu || !GetBoundaryLine()) return false;

  double menu_h = UImenu->GetBoundaryLine()->height;
  double menu_w = UImenu->GetBoundaryLine()->width;

  // Absolute: dropdown sits just under the combo (may extend past the host form AABB).
  double abs_x = GetXPosition();
  double abs_y = GetYPosition() + menu_h;

  // Local to host father (form): abs = father_x + local_x ; abs_y = father_y - local_y.
  // local_y can be negative when the menu hangs below the form's EdgeBottom.
  double local_x = abs_x;
  double local_y = abs_y;
  UI_ELEMENT* host = UImenu->GetFather();
  if(host)
    {
      local_x = abs_x - host->GetXPosition();
      local_y = host->GetYPosition() - abs_y;
    }

  UImenu->GetBoundaryLine()->x      = local_x;
  UImenu->GetBoundaryLine()->y      = local_y;
  UImenu->GetBoundaryLine()->width  = menu_w;
  UImenu->GetBoundaryLine()->height = menu_h;
  UImenu->SetIntrinsicWidth(menu_w);
  UImenu->SetIntrinsicHeight(menu_h);
  UImenu->SetXPosition(abs_x);
  UImenu->SetYPosition(abs_y);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::OpenMenu()
* @brief      Rebuilds, positions, and shows the dropdown as modal.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::OpenMenu()
{
  if(!UImenu) return false;
  if(items.GetSize() == 0) return false;

  // One mini-modal popup at a time under the host (or active modal).
  UI_ELEMENT* dismiss_root = GetFather();
  if(GEN_USERINTERFACE.Element_GetModal()) dismiss_root = GEN_USERINTERFACE.Element_GetModal();
  CloseAllVisibleMenus(dismiss_root, this);

  SnapshotRestoreHostFormSize(false);   // lock host form size before layout

  // Refresh plate/border from the live combo (Dark↔Light may have remapped after EnsureMenu).
  SyncMenuChrome();
  RebuildMenu();
  LayoutMenuUnderCombo();

  // Keep parent modal (config form) — popup is a mini-modal overlay inside its compose.
  // Move menu to end of host compose so Draw_Form paints it on top of siblings.
  UI_ELEMENT* host = UImenu->GetFather();
  if(host && host->GetComposeElements())
    {
      host->GetComposeElements()->Delete(UImenu);
      host->GetComposeElements()->Add(UImenu);
    }

  // Keep invisible until the under-menu backdrop is snapped — SetComboMenuRect GetBitmap
  // must see pre-menu pixels or dismiss gap-fill flashes (or restores a baked-in plate).
  UImenu->SetVisible(false);
  UImenu->SetMustReDraw(false);
  if(GetLayout() && GetLayout()->GetSkin())
    {
      GetLayout()->GetSkin()->CalculeBoundaryLine_AllElements(UImenu, true);
    }

  // Re-lock popup size to the edit field after layout (AUTO text must not widen the menu).
  if(GetBoundaryLine() && UImenu->GetBoundaryLine())
    {
      const double menu_w = GetBoundaryLine()->width;
      const double inner_w = menu_w - (UI_ELEMENT_COMBO_OPTION_MARGIN_W * 2.0);
      UImenu->GetBoundaryLine()->width = menu_w;
      UImenu->SetIntrinsicWidth(menu_w);
      if(UImenu->GetComposeElements())
        {
          for(XDWORD i=0; i<UImenu->GetComposeElements()->GetSize(); i++)
            {
              UI_ELEMENT* child = UImenu->GetComposeElements()->Get(i);
              if(!child || child->GetType() != UI_ELEMENT_TYPE_OPTION || !child->GetBoundaryLine()) continue;
              child->GetBoundaryLine()->width = inner_w;
              child->SetIntrinsicWidth(inner_w);
            }
        }
      LayoutMenuUnderCombo();
      ApplyOptionColumns();   // tab stops survive CalculeBoundaryLine recentering
    }

  // Every open: no overlay scrollbars (menu height == content). Also sync VisibleRect so
  // Draw_Form paints the plate+border (programmatic menus never get XML visiblerect).
  UImenu->Scroll_SetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, UI_OVERFLOW_HIDDEN);
  UImenu->Scroll_SetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL  , UI_OVERFLOW_HIDDEN);
  UImenu->Scroll_SetBarShown(UI_PROPERTY_SCROLLEABLE_TYPE_ALL, false);
  UImenu->Scroll_SetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_ALL, 0.0);
  if(UImenu->GetVisibleRect() && UImenu->GetBoundaryLine())
    {
      UI_BOUNDARYLINE* bl = UImenu->GetBoundaryLine();
      UI_BOUNDARYLINE* vr = UImenu->GetVisibleRect();
      vr->x      = UImenu->GetXPosition();
      vr->y      = UImenu->GetYPosition();
      vr->width  = bl->width;
      vr->height = bl->height;
    }

  SnapshotRestoreHostFormSize(true);    // form AABB must stay fixed

  // Sticky popup AABB + pre-menu backdrop BEFORE the menu becomes visible/drawn.
  if(GEN_USERINTERFACE.Element_GetModal() && UImenu->GetBoundaryLine())
    {
      const double mw = UImenu->GetBoundaryLine()->width;
      const double mh = UImenu->GetBoundaryLine()->height;
      if(mw > 0.5 && mh > 0.5)
        {
          const double ml = UImenu->GetXPosition();
          const double mb = UImenu->GetYPosition();
          GEN_USERINTERFACE.ModalLayer_SetComboMenuRect(ml, mb - mh, mw, mh);
        }
    }

  UImenu->SetVisible(true);
  UImenu->SetMustReDraw(true);

  if(host) GEN_USERINTERFACE.Elements_SetToRedraw(host);
      else GEN_USERINTERFACE.Elements_SetToRedraw(UImenu);
  // Rebuild modal layer AABB so the mini-modal menu zone (may hang past the form) is wiped/drawn.
  if(GEN_USERINTERFACE.Element_GetModal()) GEN_USERINTERFACE.ModalLayer_Invalidate();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::CloseMenu()
* @brief      Hides the dropdown popup and rebuilds its overlay zone.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::CloseMenu()
{
  if(!UImenu) return false;
  if(!UImenu->IsVisible()) return true;

  // Capture popup AABB into the modal layer BEFORE hiding/clearing sticky so the next
  // DrawModalOnTop sees prev ⊃ form and runs the shrink restore.
  if(GEN_USERINTERFACE.Element_GetModal() && UImenu->GetBoundaryLine())
    {
      const double mw = UImenu->GetBoundaryLine()->width;
      const double mh = UImenu->GetBoundaryLine()->height;
      if(mw > 0.5 && mh > 0.5)
        {
          const double ml = UImenu->GetXPosition();
          const double mb = UImenu->GetYPosition();
          GEN_USERINTERFACE.ModalLayer_UnionRect(ml, mb - mh, mw, mh);
        }
    }
  GEN_USERINTERFACE.ModalLayer_ClearComboMenuRect();

  UImenu->SetVisible(false);
  // SetVisible dirties the menu + option children. Clear the whole subtree so a later
  // Draw of the host does not RestoreOnHide a stale plate over the form.
  UImenu->SetMustReDraw(false);
  if(UImenu->GetComposeElements())
    {
      for(XDWORD i=0; i<UImenu->GetComposeElements()->GetSize(); i++)
        {
          UI_ELEMENT* child = UImenu->GetComposeElements()->Get(i);
          if(child) child->SetMustReDraw(false);
        }
    }

  SnapshotRestoreHostFormSize(true);    // keep form size locked even after close
  host_size_locked = false;

  if(GetFather()) GEN_USERINTERFACE.Elements_SetToRedraw(GetFather());
  // Dirty ALL layouts so content under the overhanging menu plate is restored.
  // ModalLayer_Invalidate keeps the previous expanded AABB; DrawModalOnTop shrink
  // re-paints content immediately when the overlay contracts.
  GEN_USERINTERFACE.Elements_SetToRedraw();
  // Elements_SetToRedraw re-dirties this hidden MENU. Clear again so layout Update
  // does not RestoreOnHide a stale formbackdrop plate before DrawModalOnTop runs
  // (ModalLayer_IsComboMenuClearPending also blocks MENU RestoreOnHide).
  UImenu->SetMustReDraw(false);
  if(UImenu->GetComposeElements())
    {
      for(XDWORD i=0; i<UImenu->GetComposeElements()->GetSize(); i++)
        {
          UI_ELEMENT* child = UImenu->GetComposeElements()->Get(i);
          if(child) child->SetMustReDraw(false);
        }
    }
  if(GEN_USERINTERFACE.Element_GetModal()) GEN_USERINTERFACE.ModalLayer_Invalidate();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::ToggleMenu()
* @brief      Opens or closes the dropdown.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::ToggleMenu()
{
  if(IsMenuVisible()) return CloseMenu();
  return OpenMenu();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::IsMenuVisible()
* @brief      True when the dropdown menu is visible.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::IsMenuVisible()
{
  return (UImenu && UImenu->IsVisible());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::ApplyMenuOption(UI_ELEMENT* option_element)
* @brief      Applies the value of a selected menu option into the edit text.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::ApplyMenuOption(UI_ELEMENT* option_element)
{
  if(!option_element) return false;

  UI_ELEMENT* walk = option_element;
  // Option may be the selectable itself, or a child text — climb to OPTION then MENU.
  while(walk && walk->GetType() != UI_ELEMENT_TYPE_OPTION)
    {
      walk = walk->GetFather();
    }
  if(!walk) return false;

  UI_ELEMENT* menufather = walk->GetFather();
  if(!menufather || menufather != (UI_ELEMENT*)UImenu) return false;

  XSTRING* name = walk->GetName();
  if(!name) return false;

  // name = combo_item_NN
  int index = -1;
  {
    XSTRING prefix = NL("combo_item_");
    if(name->Find(prefix.Get(), false) != 0) return false;
    XSTRING num;
    name->Copy(prefix.GetSize(), name->GetSize(), num);
    index = num.ConvertToInt(0, NULL, false);
  }
  if(index < 0) return false;

  UI_ELEMENT_COMBO_ITEM* item = GetItem(index);
  if(!item) return false;

  GetText()->Set(item->value);
  SanitizeTextToMode();
  SetMustReDraw(true);

  CloseMenu();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_COMBO::Clean()
* @brief      Default-initialize members.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::IsPointInMenu(int design_x, int design_y)
{
  if(!UImenu || !UImenu->IsVisible() || !UImenu->GetBoundaryLine()) return false;

  UI_BOUNDARYLINE bline;
  bline.x      = UImenu->GetXPosition();
  bline.y      = UImenu->GetYPosition();
  bline.width  = UImenu->GetBoundaryLine()->width;
  bline.height = UImenu->GetBoundaryLine()->height;
  return bline.IsWithin(design_x, design_y);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::CloseAllVisibleMenus(UI_ELEMENT* root, UI_ELEMENT_COMBO* keep_open)
* @brief      Closes every open combo popup under root (mini-modal dismiss).
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::CloseAllVisibleMenus(UI_ELEMENT* root, UI_ELEMENT_COMBO* keep_open)
{
  if(!root || !root->GetComposeElements()) return false;

  bool closed = false;
  for(XDWORD i=0; i<root->GetComposeElements()->GetSize(); i++)
    {
      UI_ELEMENT* child = root->GetComposeElements()->Get(i);
      if(!child) continue;

      if(child->GetType() == UI_ELEMENT_TYPE_COMBO)
        {
          UI_ELEMENT_COMBO* combo = (UI_ELEMENT_COMBO*)child;
          if((combo != keep_open) && combo->IsMenuVisible())
            {
              combo->CloseMenu();
              closed = true;
            }
        }

      if(CloseAllVisibleMenus(child, keep_open)) closed = true;
    }
  return closed;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_COMBO* UI_ELEMENT_COMBO::FindOwnerOfMenu(UI_ELEMENT* menu_or_child)
* @brief      Owning combo for a menu (or a descendant option/text), or NULL.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_COMBO* UI_ELEMENT_COMBO::FindOwnerOfMenu(UI_ELEMENT* menu_or_child)
{
  if(!menu_or_child) return NULL;

  UI_ELEMENT* menu = menu_or_child;
  while(menu && (menu->GetType() != UI_ELEMENT_TYPE_MENU))
    {
      menu = menu->GetFather();
    }
  if(!menu) return NULL;

  UI_ELEMENT* host = menu->GetFather();
  if(!host || !host->GetComposeElements()) return NULL;

  for(XDWORD i=0; i<host->GetComposeElements()->GetSize(); i++)
    {
      UI_ELEMENT* sib = host->GetComposeElements()->Get(i);
      if(!sib || (sib->GetType() != UI_ELEMENT_TYPE_COMBO)) continue;
      UI_ELEMENT_COMBO* combo = (UI_ELEMENT_COMBO*)sib;
      if(combo->Get_UIMenu() == (UI_ELEMENT_MENU*)menu) return combo;
    }
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::ElementIsInsideOpenComboMenu(UI_ELEMENT* element)
* @brief      True when element lives under a currently visible combo dropdown menu.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::ElementIsInsideOpenComboMenu(UI_ELEMENT* element)
{
  UI_ELEMENT_COMBO* owner = FindOwnerOfMenu(element);
  return (owner && owner->IsMenuVisible());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_MENU* UI_ELEMENT_COMBO::FindOpenMenuUnderPoint(UI_ELEMENT* root, int design_x, int design_y)
* @brief      Finds a visible combo dropdown under root whose AABB contains the design point.
* @note       PreSelectElement only recurses into compose children when the point is inside the father
*             box. Modal combo menus hang past the form; the manager uses this to PreSelect the menu
*             even when the pointer is outside the host AABB.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_MENU* UI_ELEMENT_COMBO::FindOpenMenuUnderPoint(UI_ELEMENT* root, int design_x, int design_y)
{
  if(!root || !root->GetComposeElements()) return NULL;

  UI_ELEMENT_MENU* hit = NULL;

  for(XDWORD i = 0; i < root->GetComposeElements()->GetSize(); i++)
    {
      UI_ELEMENT* child = root->GetComposeElements()->Get(i);
      if(!child || !child->IsVisible()) continue;

      if(child->GetType() == UI_ELEMENT_TYPE_COMBO)
        {
          UI_ELEMENT_COMBO* combo = (UI_ELEMENT_COMBO*)child;
          if(combo->IsMenuVisible() && combo->IsPointInMenu(design_x, design_y) && combo->Get_UIMenu())
            {
              hit = combo->Get_UIMenu();
            }
        }

      UI_ELEMENT_MENU* nested = FindOpenMenuUnderPoint(child, design_x, design_y);
      if(nested) hit = nested;
    }

  return hit;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool UI_ELEMENT_COMBO::SnapshotRestoreHostFormSize(bool restore)
* @brief      Locks / restores the host form AABB so the popup never resizes the form.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
bool UI_ELEMENT_COMBO::SnapshotRestoreHostFormSize(bool restore)
{
  UI_ELEMENT* host = GetFather();
  if(!host || !host->GetBoundaryLine()) return false;
  if(host->GetType() != UI_ELEMENT_TYPE_FORM && host->GetType() != UI_ELEMENT_TYPE_MENU) return false;

  UI_ELEMENT_FORM* form = (UI_ELEMENT_FORM*)host;

  if(!restore)
    {
      host_lock_w = host->GetBoundaryLine()->width;
      host_lock_h = host->GetBoundaryLine()->height;
      host_lock_vis_w = 0.0;
      host_lock_vis_h = 0.0;
      if(form->GetVisibleRect())
        {
          host_lock_vis_w = form->GetVisibleRect()->width;
          host_lock_vis_h = form->GetVisibleRect()->height;
        }
      host_size_locked = true;
      return true;
    }

  if(!host_size_locked) return false;

  host->GetBoundaryLine()->width  = host_lock_w;
  host->GetBoundaryLine()->height = host_lock_h;
  host->SetIntrinsicWidth(host_lock_w);
  host->SetIntrinsicHeight(host_lock_h);
  if(form->GetVisibleRect() && (host_lock_vis_w > 0.0) && (host_lock_vis_h > 0.0))
    {
      form->GetVisibleRect()->width  = host_lock_vis_w;
      form->GetVisibleRect()->height = host_lock_vis_h;
    }
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void UI_ELEMENT_COMBO::Clean()
* @brief      Default-initialize members.
* @ingroup    USERINTERFACE
*
*---------------------------------------------------------------------------------------------------------------------*/
void UI_ELEMENT_COMBO::Clean()
{
  UImenu           = NULL;
  menu_owned       = false;
  menu_idle_ink_r  = 200;
  menu_idle_ink_g  = 208;
  menu_idle_ink_b  = 220;
  host_lock_w      = 0.0;
  host_lock_h      = 0.0;
  host_lock_vis_w  = 0.0;
  host_lock_vis_h  = 0.0;
  host_size_locked = false;
}
