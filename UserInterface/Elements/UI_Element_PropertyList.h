/**-------------------------------------------------------------------------------------------------------------------
*
* @file       UI_Element_PropertyList.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"
#include "XString.h"

#include "UI_Property_Scrolleable.h"
#include "UI_Element.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define UI_ELEMENT_PROPERTYLIST_DEFAULT_NAMECOLUMNWIDTH     220.0f
#define UI_ELEMENT_PROPERTYLIST_MIN_COLUMNWIDTH             80.0f
#define UI_ELEMENT_PROPERTYLIST_DEFAULT_ROWHEIGHT           22.0f
#define UI_ELEMENT_PROPERTYLIST_DEFAULT_HEADERHEIGHT        26.0f
#define UI_ELEMENT_PROPERTYLIST_SPLITTER_HITWIDTH           6.0f

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

struct UI_ELEMENT_PROPERTYLIST_ROW
{
                                      UI_ELEMENT_PROPERTYLIST_ROW   ();
                                     ~UI_ELEMENT_PROPERTYLIST_ROW   ();

  XSTRING                             name;
  XSTRING                             value;

  private:

  void                                Clean                         ();
};


class UI_ELEMENT_PROPERTYLIST : public UI_ELEMENT, public UI_PROPERTY_SCROLLEABLE
{
  public:
                                      UI_ELEMENT_PROPERTYLIST       ();
    virtual                          ~UI_ELEMENT_PROPERTYLIST       ();

    XDWORD                            GetSizeFont                   ();
    void                              SetSizeFont                   (XDWORD sizefont);

    double                            GetRowHeight                  ();
    void                              SetRowHeight                  (double rowheight);

    double                            GetHeaderHeight               ();
    void                              SetHeaderHeight               (double headerheight);

    double                            GetContentHeight              ();
    void                              SetContentHeight              (double contentheight);

    XSTRING*                          GetHeaderName                 ();
    XSTRING*                          GetHeaderValue                ();
    void                              SetHeaderCaptions             (XCHAR* headername, XCHAR* headervalue);

    bool                              Clear                         ();
    bool                              Add                           (XCHAR* name, XCHAR* value);
    bool                              SetValue                      (XCHAR* name, XCHAR* value);

    XDWORD                            GetRowCount                   ();
    UI_ELEMENT_PROPERTYLIST_ROW*      GetRow                        (XDWORD index);

    double                            GetNameColumnWidth            ();
    void                              SetNameColumnWidth            (double namecolumnwidth);

    // Splitter (all coordinates local to the element box: x from the left edge, y from the top edge, in design px).
    bool                              HitTestSplitter               (double local_x, double local_y, double box_w, double box_h);
    bool                              ScreenToLocal                 (int screen_x, int screen_y, double& local_x, double& local_y);

    bool                              IsSplitDragging               ();
    bool                              Split_BeginDrag               ();
    bool                              Split_DragTo                  (double abs_x);                  // abs_x relative to element left
    bool                              Split_EndDrag                 ();

  private:

    void                              Clean                         ();

    XDWORD                            sizefont;
    double                            rowheight;
    double                            headerheight;
    double                            contentheight;
    double                            name_column_width;
    bool                              split_dragging;
    XSTRING                           headername;
    XSTRING                           headervalue;
    XVECTOR<UI_ELEMENT_PROPERTYLIST_ROW*> rows;
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
