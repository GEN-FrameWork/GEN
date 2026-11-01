/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_Element_Console.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"
#include "XString.h"

#include "UI_Color.h"
#include "UI_Property_Scrolleable.h"
#include "UI_Element.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

// Color indices align with XTRACE_COLOR_* (XTrace.h). 0xFF = use rgb override.
#define UI_ELEMENT_CONSOLE_COLOR_BLACK          0
#define UI_ELEMENT_CONSOLE_COLOR_BLUE           1
#define UI_ELEMENT_CONSOLE_COLOR_GREEN          2
#define UI_ELEMENT_CONSOLE_COLOR_PURPLE         3
#define UI_ELEMENT_CONSOLE_COLOR_RED            4
#define UI_ELEMENT_CONSOLE_COLOR_GRAY           5
#define UI_ELEMENT_CONSOLE_COLOR_RGB            0xFF

#define UI_ELEMENT_CONSOLE_DEFAULT_MAXLINES     2000
#define UI_ELEMENT_CONSOLE_DEFAULT_MAXLINES_OFF 8000
#define UI_ELEMENT_CONSOLE_DEFAULT_LINESPACING  2

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

struct UI_ELEMENT_CONSOLE_LINE
{
                                  UI_ELEMENT_CONSOLE_LINE     ();
                                 ~UI_ELEMENT_CONSOLE_LINE     ();

  XSTRING                         text;
  XBYTE                           colorindex;                 // 0..5 or UI_ELEMENT_CONSOLE_COLOR_RGB
  UI_COLOR                        rgb;
  XDWORD                          visualrows;                 // cached wrap rows (0 = needs measure)
  XDWORD                          id;                         // monotonic id assigned on AddLine (never 0)

  private:

  void                            Clean                       ();
};


class UI_ELEMENT_CONSOLE : public UI_ELEMENT, public UI_PROPERTY_SCROLLEABLE
{
  public:
                                  UI_ELEMENT_CONSOLE          ();
    virtual                      ~UI_ELEMENT_CONSOLE          ();

    XDWORD                        GetSizeFont                 ();
    void                          SetSizeFont                 (XDWORD sizefont);

    XDWORD                        GetLineSpacing              ();
    void                          SetLineSpacing              (XDWORD linespacing = UI_ELEMENT_CONSOLE_DEFAULT_LINESPACING);

    bool                          IsWordWrap                  ();
    void                          SetWordWrap                 (bool wordwrap);

    XDWORD                        GetMaxLines                 ();
    void                          SetMaxLines                 (XDWORD maxlines);

    XDWORD                        GetMaxLinesOff              ();
    void                          SetMaxLinesOff              (XDWORD maxlinesoff);

    bool                          GetAutoscroll               ();
    void                          SetAutoscroll               (bool autoscroll);

    double                        GetContentHeight            ();
    void                          SetContentHeight            (double contentheight);

    double                        GetContentWidth             ();
    void                          SetContentWidth             (double contentwidth);

    double                        GetLayoutMaxTextWidth       ();
    void                          SetLayoutMaxTextWidth       (double maxtextwidth);

    XDWORD                        GetLayoutSizeFont           ();
    void                          SetLayoutSizeFont           (XDWORD sizefont);

    void                          SetLayoutMetrics            (double pad_x, double pad_y, double lineheight, double fontheight);
    double                        GetLayoutPadX               ();
    double                        GetLayoutPadY               ();
    double                        GetLayoutLineHeight         ();
    double                        GetLayoutFontHeight         ();

    void                          InvalidateLayoutCache       ();

    XDWORD                        ConsumeTrimmedRows          ();

    XDWORD                        GetLineCount                ();
    UI_ELEMENT_CONSOLE_LINE*      GetLine                     (XDWORD index);

    bool                          GetText                     (XSTRING& text);
    bool                          GetSelectedText             (XSTRING& text);

    int                           FindLineByID                (XDWORD id);
    int                           FindLine                    (XCHAR* substring, bool ignorecase = true, XDWORD startindex = 0);

    bool                          HitTestLine                 (int screen_x, int screen_y, XDWORD& lineindex);

    void                          Selection_Clear             ();
    bool                          Selection_Has               ();
    bool                          Selection_Get               (XDWORD& first, XDWORD& last);
    void                          Selection_Set               (XDWORD first, XDWORD last);
    void                          Selection_SelectAll         ();
    bool                          Selection_IsLineSelected    (XDWORD index);
    bool                          Selection_CopyToClipboard   ();
    bool                          CopyTextToClipboard         (XSTRING& text);

    bool                          Print                       (XCHAR* string);
    bool                          Printf                      (XCHAR* mask, ...);
    bool                          PrintColor                  (XBYTE colorindex, XCHAR* string);
    bool                          PrintColorf                 (XBYTE colorindex, XCHAR* mask, ...);
    bool                          PrintRGB                    (UI_COLOR& color, XCHAR* string);
    bool                          PrintRGBf                   (UI_COLOR& color, XCHAR* mask, ...);

    bool                          Clear                       ();

    bool                          ConsumeAutoscrollPending    ();

  private:

    bool                          AddLine                     (XCHAR* string, XBYTE colorindex, UI_COLOR* rgb);
    void                          TrimToMaxLines              ();
    void                          Selection_AdjustFrontTrim   (XDWORD trimmed_lines);
    void                          Clean                       ();

    XDWORD                        sizefont;
    XDWORD                        linespacing;
    bool                          wordwrap;
    XDWORD                        maxlines;
    XDWORD                        maxlines_off;               // hard cap while autoscroll is OFF (>= maxlines)
    bool                          autoscroll;
    bool                          autoscrollpending;
    double                        contentheight;
    double                        contentwidth;
    double                        layout_maxtextwidth;        // width used for cached visualrows
    XDWORD                        layout_sizefont;            // font size used for cached visualrows
    double                        layout_pad_x;
    double                        layout_pad_y;
    double                        layout_lineheight;
    double                        layout_fontheight;
    XDWORD                        trimmedrows_pending;        // front-trim rows to compensate when OFF
    XDWORD                        next_line_id;               // next id to assign on AddLine
    bool                          selection_active;
    XDWORD                        selection_first;
    XDWORD                        selection_last;
    XVECTOR<UI_ELEMENT_CONSOLE_LINE*> lines;
};

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
