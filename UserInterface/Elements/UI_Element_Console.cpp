/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_Element_Console.cpp
*
* @class      UI_ELEMENT_CONSOLE
* @brief      Graphical output console (line buffer with per-line color)
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

#include <stdarg.h>

#ifdef WINDOWS
#include <windows.h>
#endif

#include "UI_Element_Console.h"
#include "UI_Layout.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


UI_ELEMENT_CONSOLE_LINE::UI_ELEMENT_CONSOLE_LINE()
{
  Clean();
}


UI_ELEMENT_CONSOLE_LINE::~UI_ELEMENT_CONSOLE_LINE()
{
  Clean();
}


void UI_ELEMENT_CONSOLE_LINE::Clean()
{
  text.Empty();
  colorindex = UI_ELEMENT_CONSOLE_COLOR_BLACK;
  rgb.SetInvalid();
  visualrows = 0;
  id         = 0;
}


UI_ELEMENT_CONSOLE::UI_ELEMENT_CONSOLE()
{
  Clean();

  SetType(UI_ELEMENT_TYPE_CONSOLE);
  GetTypeString()->Set(NL("console"));
}


UI_ELEMENT_CONSOLE::~UI_ELEMENT_CONSOLE()
{
  Clear();
  Clean();
}


XDWORD UI_ELEMENT_CONSOLE::GetSizeFont()
{
  return sizefont;
}


void UI_ELEMENT_CONSOLE::SetSizeFont(XDWORD sizefont)
{
  if(this->sizefont != sizefont) InvalidateLayoutCache();
  this->sizefont = sizefont;
}


XDWORD UI_ELEMENT_CONSOLE::GetLineSpacing()
{
  return linespacing;
}


void UI_ELEMENT_CONSOLE::SetLineSpacing(XDWORD linespacing)
{
  this->linespacing = linespacing;
}


bool UI_ELEMENT_CONSOLE::IsWordWrap()
{
  return wordwrap;
}


void UI_ELEMENT_CONSOLE::SetWordWrap(bool wordwrap)
{
  if(this->wordwrap != wordwrap) InvalidateLayoutCache();
  this->wordwrap = wordwrap;
}


XDWORD UI_ELEMENT_CONSOLE::GetMaxLines()
{
  return maxlines;
}


void UI_ELEMENT_CONSOLE::SetMaxLines(XDWORD maxlines)
{
  if(maxlines < 1) maxlines = 1;
  this->maxlines = maxlines;
  if(maxlines_off < this->maxlines) maxlines_off = this->maxlines;
  TrimToMaxLines();
}


XDWORD UI_ELEMENT_CONSOLE::GetMaxLinesOff()
{
  return maxlines_off;
}


void UI_ELEMENT_CONSOLE::SetMaxLinesOff(XDWORD maxlinesoff)
{
  if(maxlinesoff < 1) maxlinesoff = 1;
  if(maxlinesoff < maxlines) maxlinesoff = maxlines;
  maxlines_off = maxlinesoff;
  TrimToMaxLines();
}


bool UI_ELEMENT_CONSOLE::GetAutoscroll()
{
  return autoscroll;
}


void UI_ELEMENT_CONSOLE::SetAutoscroll(bool autoscroll)
{
  this->autoscroll = autoscroll;
  if(!autoscroll)
    {
      autoscrollpending = false;
    }
   else
    {
      TrimToMaxLines();
    }
}


double UI_ELEMENT_CONSOLE::GetContentHeight()
{
  return contentheight;
}


void UI_ELEMENT_CONSOLE::SetContentHeight(double contentheight)
{
  this->contentheight = contentheight;
}


double UI_ELEMENT_CONSOLE::GetContentWidth()
{
  return contentwidth;
}


void UI_ELEMENT_CONSOLE::SetContentWidth(double contentwidth)
{
  this->contentwidth = contentwidth;
}


double UI_ELEMENT_CONSOLE::GetLayoutMaxTextWidth()
{
  return layout_maxtextwidth;
}


void UI_ELEMENT_CONSOLE::SetLayoutMaxTextWidth(double maxtextwidth)
{
  layout_maxtextwidth = maxtextwidth;
}


XDWORD UI_ELEMENT_CONSOLE::GetLayoutSizeFont()
{
  return layout_sizefont;
}


void UI_ELEMENT_CONSOLE::SetLayoutSizeFont(XDWORD sizefont)
{
  layout_sizefont = sizefont;
}


void UI_ELEMENT_CONSOLE::SetLayoutMetrics(double pad_x, double pad_y, double lineheight, double fontheight)
{
  layout_pad_x       = pad_x;
  layout_pad_y       = pad_y;
  layout_lineheight  = lineheight;
  layout_fontheight  = fontheight;
}


double UI_ELEMENT_CONSOLE::GetLayoutPadX()
{
  return layout_pad_x;
}


double UI_ELEMENT_CONSOLE::GetLayoutPadY()
{
  return layout_pad_y;
}


double UI_ELEMENT_CONSOLE::GetLayoutLineHeight()
{
  return layout_lineheight;
}


double UI_ELEMENT_CONSOLE::GetLayoutFontHeight()
{
  return layout_fontheight;
}


void UI_ELEMENT_CONSOLE::InvalidateLayoutCache()
{
  layout_maxtextwidth = -1.0f;
  layout_sizefont     = 0;

  for(XDWORD i=0; i<lines.GetSize(); i++)
    {
      UI_ELEMENT_CONSOLE_LINE* line = lines.Get(i);
      if(line) line->visualrows = 0;
    }
}


XDWORD UI_ELEMENT_CONSOLE::ConsumeTrimmedRows()
{
  XDWORD n = trimmedrows_pending;
  trimmedrows_pending = 0;
  return n;
}


XDWORD UI_ELEMENT_CONSOLE::GetLineCount()
{
  return lines.GetSize();
}


UI_ELEMENT_CONSOLE_LINE* UI_ELEMENT_CONSOLE::GetLine(XDWORD index)
{
  return lines.Get(index);
}


bool UI_ELEMENT_CONSOLE::GetText(XSTRING& text)
{
  text.Empty();

  XDWORD count = lines.GetSize();
  for(XDWORD i=0; i<count; i++)
    {
      UI_ELEMENT_CONSOLE_LINE* line = lines.Get(i);
      if(!line) continue;

      if(i > 0) text.Add(_C('\n'));
      text.Add(line->text);
    }

  return true;
}


bool UI_ELEMENT_CONSOLE::GetSelectedText(XSTRING& text)
{
  text.Empty();
  if(!selection_active) return false;

  XDWORD first = selection_first;
  XDWORD last  = selection_last;
  if(first > last) { XDWORD t = first; first = last; last = t; }

  XDWORD count = lines.GetSize();
  if(first >= count) return false;
  if(last  >= count) last = count - 1;

  bool any = false;
  for(XDWORD i=first; i<=last; i++)
    {
      UI_ELEMENT_CONSOLE_LINE* line = lines.Get(i);
      if(!line) continue;
      if(any) text.Add(_C('\n'));
      text.Add(line->text);
      any = true;
    }

  return any;
}


int UI_ELEMENT_CONSOLE::FindLineByID(XDWORD id)
{
  if(id == 0) return NOTFOUND;

  for(XDWORD i=0; i<lines.GetSize(); i++)
    {
      UI_ELEMENT_CONSOLE_LINE* line = lines.Get(i);
      if(line && line->id == id) return (int)i;
    }

  return NOTFOUND;
}


int UI_ELEMENT_CONSOLE::FindLine(XCHAR* substring, bool ignorecase, XDWORD startindex)
{
  if(!substring) return NOTFOUND;

  XDWORD count = lines.GetSize();
  if(startindex >= count) return NOTFOUND;

  for(XDWORD i=startindex; i<count; i++)
    {
      UI_ELEMENT_CONSOLE_LINE* line = lines.Get(i);
      if(!line) continue;
      if(line->text.Find(substring, ignorecase, 0) != XSTRING_NOTFOUND) return (int)i;
    }

  return NOTFOUND;
}


bool UI_ELEMENT_CONSOLE::HitTestLine(int screen_x, int screen_y, XDWORD& lineindex)
{
  if(!GetBoundaryLine()) return false;
  if(layout_lineheight < 1.0f) return false;

  double rect_x = GetXPosition();
  double rect_y = GetYPosition() - GetBoundaryLine()->height;
  double rect_w = GetBoundaryLine()->width;
  double rect_h = GetBoundaryLine()->height;
  double scale  = 1.0;

  UI_LAYOUT* layout = GetLayout();
  if(layout && layout->IsUIScaleActive())
    {
      scale = layout->GetUIScale();
      if(scale < UI_LAYOUT_UISCALE_MIN) scale = UI_LAYOUT_UISCALE_MIN;
      rect_x = rect_x * scale + layout->GetUIScaleOffsetX();
      rect_y = rect_y * scale + layout->GetUIScaleOffsetY();
      rect_w *= scale;
      rect_h *= scale;
    }

  if(screen_x < (int)rect_x || screen_x >= (int)(rect_x + rect_w)) return false;
  if(screen_y < (int)rect_y || screen_y >= (int)(rect_y + rect_h)) return false;

  double pad_y = layout_pad_y * scale;
  double lh    = layout_lineheight * scale;
  double vdisp = Scroll_GetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL) * scale;

  double rel = ((double)screen_y - rect_y - pad_y) - vdisp;
  if(rel < 0.0f) return false;

  XDWORD visual_row = (XDWORD)(rel / lh);

  if(!wordwrap)
    {
      if(visual_row >= lines.GetSize()) return false;
      lineindex = visual_row;
      return true;
    }

  XDWORD row = 0;
  for(XDWORD i=0; i<lines.GetSize(); i++)
    {
      UI_ELEMENT_CONSOLE_LINE* line = lines.Get(i);
      XDWORD rows_here = 1;
      if(line && line->visualrows > 0) rows_here = line->visualrows;
      if(visual_row >= row && visual_row < row + rows_here)
        {
          lineindex = i;
          return true;
        }
      row += rows_here;
    }

  return false;
}


void UI_ELEMENT_CONSOLE::Selection_Clear()
{
  if(!selection_active) return;
  selection_active = false;
  selection_first  = 0;
  selection_last   = 0;
  SetMustReDraw(true);
}


bool UI_ELEMENT_CONSOLE::Selection_Has()
{
  return selection_active;
}


bool UI_ELEMENT_CONSOLE::Selection_Get(XDWORD& first, XDWORD& last)
{
  if(!selection_active) return false;
  first = selection_first;
  last  = selection_last;
  if(first > last) { XDWORD t = first; first = last; last = t; }
  return true;
}


void UI_ELEMENT_CONSOLE::Selection_Set(XDWORD first, XDWORD last)
{
  XDWORD count = lines.GetSize();
  if(count == 0)
    {
      Selection_Clear();
      return;
    }

  if(first >= count) first = count - 1;
  if(last  >= count) last  = count - 1;

  selection_active = true;
  selection_first  = first;
  selection_last   = last;
  SetMustReDraw(true);
}


void UI_ELEMENT_CONSOLE::Selection_SelectAll()
{
  if(lines.GetSize() == 0)
    {
      Selection_Clear();
      return;
    }
  Selection_Set(0, lines.GetSize() - 1);
}


bool UI_ELEMENT_CONSOLE::Selection_IsLineSelected(XDWORD index)
{
  if(!selection_active) return false;
  XDWORD first = selection_first;
  XDWORD last  = selection_last;
  if(first > last) { XDWORD t = first; first = last; last = t; }
  return (index >= first && index <= last);
}


bool UI_ELEMENT_CONSOLE::Selection_CopyToClipboard()
{
  XSTRING text;
  if(selection_active)
    {
      if(!GetSelectedText(text)) return false;
    }
   else
    {
      if(!GetText(text)) return false;
      if(text.IsEmpty()) return false;
    }

  return CopyTextToClipboard(text);
}


bool UI_ELEMENT_CONSOLE::CopyTextToClipboard(XSTRING& text)
{
#ifdef WINDOWS
  if(!OpenClipboard(NULL)) return false;
  EmptyClipboard();

  XDWORD chars = text.GetSize() + 1;
  SIZE_T bytes = (SIZE_T)chars * sizeof(wchar_t);
  HGLOBAL mem  = GlobalAlloc(GMEM_MOVEABLE, bytes);
  if(!mem)
    {
      CloseClipboard();
      return false;
    }

  wchar_t* dst = (wchar_t*)GlobalLock(mem);
  if(!dst)
    {
      GlobalFree(mem);
      CloseClipboard();
      return false;
    }

  if(text.GetSize() > 0 && text.Get())
    {
      memcpy(dst, text.Get(), text.GetSize() * sizeof(wchar_t));
    }
  dst[text.GetSize()] = 0;
  GlobalUnlock(mem);

  if(!SetClipboardData(CF_UNICODETEXT, mem))
    {
      GlobalFree(mem);
      CloseClipboard();
      return false;
    }

  CloseClipboard();
  return true;
#else
  (void)text;
  return false;
#endif
}


bool UI_ELEMENT_CONSOLE::Print(XCHAR* string)
{
  return PrintColor(UI_ELEMENT_CONSOLE_COLOR_BLACK, string);
}


bool UI_ELEMENT_CONSOLE::Printf(XCHAR* mask, ...)
{
  if(!mask) return false;

  XSTRING outstring;
  va_list arg;

  va_start(arg, mask);
  outstring.FormatArg(mask, &arg);
  va_end(arg);

  return Print(outstring.Get());
}


bool UI_ELEMENT_CONSOLE::PrintColor(XBYTE colorindex, XCHAR* string)
{
  return AddLine(string, colorindex, NULL);
}


bool UI_ELEMENT_CONSOLE::PrintColorf(XBYTE colorindex, XCHAR* mask, ...)
{
  if(!mask) return false;

  XSTRING outstring;
  va_list arg;

  va_start(arg, mask);
  outstring.FormatArg(mask, &arg);
  va_end(arg);

  return PrintColor(colorindex, outstring.Get());
}


bool UI_ELEMENT_CONSOLE::PrintRGB(UI_COLOR& color, XCHAR* string)
{
  return AddLine(string, UI_ELEMENT_CONSOLE_COLOR_RGB, &color);
}


bool UI_ELEMENT_CONSOLE::PrintRGBf(UI_COLOR& color, XCHAR* mask, ...)
{
  if(!mask) return false;

  XSTRING outstring;
  va_list arg;

  va_start(arg, mask);
  outstring.FormatArg(mask, &arg);
  va_end(arg);

  return PrintRGB(color, outstring.Get());
}


bool UI_ELEMENT_CONSOLE::Clear()
{
  lines.DeleteContents();
  lines.DeleteAll();
  contentheight       = 0.0f;
  contentwidth        = 0.0f;
  trimmedrows_pending = 0;
  autoscrollpending   = false;
  Selection_Clear();
  Scroll_SetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, 0.0f);
  Scroll_SetDisplacement(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, 0.0f);
  InvalidateLayoutCache();
  SetMustReDraw(true);
  return true;
}


bool UI_ELEMENT_CONSOLE::ConsumeAutoscrollPending()
{
  if(!autoscrollpending) return false;
  autoscrollpending = false;
  return true;
}


bool UI_ELEMENT_CONSOLE::AddLine(XCHAR* string, XBYTE colorindex, UI_COLOR* rgb)
{
  if(!string) return false;

  XSTRING source(string);
  XDWORD  start = 0;

  if(source.IsEmpty())
    {
      UI_ELEMENT_CONSOLE_LINE* line = GEN_NEW UI_ELEMENT_CONSOLE_LINE();
      if(!line) return false;

      line->id         = ++next_line_id;
      line->colorindex = colorindex;
      if(rgb)
        {
          line->rgb.CopyFrom(rgb);
          line->colorindex = UI_ELEMENT_CONSOLE_COLOR_RGB;
        }

      lines.Add(line);
    }
   else
    {
      while(start < source.GetSize())
        {
          XDWORD end = start;
          while(end < source.GetSize() && source.Get()[end] != _C('\n') && source.Get()[end] != _C('\r'))
            {
              end++;
            }

          UI_ELEMENT_CONSOLE_LINE* line = GEN_NEW UI_ELEMENT_CONSOLE_LINE();
          if(!line) return false;

          if(end > start)
            {
              line->text.Set(&source.Get()[start], end - start);
            }

          line->id         = ++next_line_id;
          line->colorindex = colorindex;
          if(rgb)
            {
              line->rgb.CopyFrom(rgb);
              line->colorindex = UI_ELEMENT_CONSOLE_COLOR_RGB;
            }

          lines.Add(line);

          if(end >= source.GetSize()) break;
          if(source.Get()[end] == _C('\r') && (end + 1) < source.GetSize() && source.Get()[end + 1] == _C('\n')) end++;
          start = end + 1;
        }
    }

  TrimToMaxLines();

  if(autoscroll) autoscrollpending = true;
  SetMustReDraw(true);

  return true;
}


void UI_ELEMENT_CONSOLE::Selection_AdjustFrontTrim(XDWORD trimmed_lines)
{
  if(!selection_active || trimmed_lines == 0) return;

  if(selection_last < trimmed_lines)
    {
      Selection_Clear();
      return;
    }

  if(selection_first >= trimmed_lines) selection_first -= trimmed_lines;
   else                                selection_first  = 0;

  selection_last -= trimmed_lines;
}


void UI_ELEMENT_CONSOLE::TrimToMaxLines()
{
  XDWORD limit = autoscroll ? maxlines : maxlines_off;
  XDWORD trimmed_lines = 0;

  while(lines.GetSize() > limit)
    {
      UI_ELEMENT_CONSOLE_LINE* oldest = lines.Get(0);
      XDWORD rows = 1;
      if(oldest && oldest->visualrows > 0) rows = oldest->visualrows;

      lines.DeleteIndex(0);
      GEN_DELETE oldest;

      if(!autoscroll) trimmedrows_pending += rows;
      trimmed_lines++;
    }

  if(trimmed_lines > 0) Selection_AdjustFrontTrim(trimmed_lines);
}


void UI_ELEMENT_CONSOLE::Clean()
{
  sizefont            = 0;
  linespacing         = UI_ELEMENT_CONSOLE_DEFAULT_LINESPACING;
  wordwrap            = true;
  maxlines            = UI_ELEMENT_CONSOLE_DEFAULT_MAXLINES;
  maxlines_off        = UI_ELEMENT_CONSOLE_DEFAULT_MAXLINES_OFF;
  autoscroll          = true;
  autoscrollpending   = false;
  contentheight       = 0.0f;
  contentwidth        = 0.0f;
  layout_maxtextwidth = -1.0f;
  layout_sizefont     = 0;
  layout_pad_x        = 8.0f;
  layout_pad_y        = 6.0f;
  layout_lineheight   = 0.0f;
  layout_fontheight   = 0.0f;
  trimmedrows_pending = 0;
  next_line_id        = 0;
  selection_active    = false;
  selection_first     = 0;
  selection_last      = 0;
}
