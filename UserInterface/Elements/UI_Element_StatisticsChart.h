/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       UI_Element_StatisticsChart.h
* 
* @class      UI_ELEMENT_STATISTICSCHART
* @brief      User Interface Element Statistics Chart class (StatisticsChars → cached bitmap)
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

#include "XString.h"
#include "XVector.h"

#include "UI_Element.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum UI_ELEMENT_STATISTICSCHART_TYPE
{
  UI_ELEMENT_STATISTICSCHART_TYPE_UNKNOWN            = 0 ,
  UI_ELEMENT_STATISTICSCHART_TYPE_LINES                  ,
  UI_ELEMENT_STATISTICSCHART_TYPE_COLUMNS                ,
  UI_ELEMENT_STATISTICSCHART_TYPE_AREA                   ,
  UI_ELEMENT_STATISTICSCHART_TYPE_BARS                   ,
  UI_ELEMENT_STATISTICSCHART_TYPE_PIE                    ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPBITMAP;
class GRP2DCANVAS;

class UI_ELEMENT_STATISTICSCHART : public UI_ELEMENT
{
  public:
                                        UI_ELEMENT_STATISTICSCHART        ();
    virtual                            ~UI_ELEMENT_STATISTICSCHART        ();

    UI_ELEMENT_STATISTICSCHART_TYPE     GetChartType                      ();
    void                                SetChartType                      (UI_ELEMENT_STATISTICSCHART_TYPE charttype);

    XSTRING*                            GetTitle                          ();
    void                                SetTitle                          (XCHAR* title);

    bool                                ClearData                         ();
    bool                                AddCategory                       (XCHAR* label);
    bool                                AddSerie                          (XCHAR* name, XBYTE r, XBYTE g, XBYTE b);
    bool                                AddValue                          (XDWORD serieindex, double value);

    bool                                SetSerieFromSamples               (XCHAR* seriename, const float* samples, int count, XBYTE r = 56, XBYTE g = 139, XBYTE b = 253);
    bool                                SetColumnsFromValues              (XCHAR* seriename, XCHAR** labels, const float* values, int count, XBYTE r = 210, XBYTE g = 153, XBYTE b = 34);

    bool                                HasData                           ();

    bool                                GetNeedsRebuild                   ();
    void                                SetNeedsRebuild                   (bool needsrebuild);

    GRPBITMAP*                          GetBitmap                         ();
    bool                                RebuildBitmap                     (GRP2DCANVAS* referencecanvas);

    GRPBITMAP*                          GetSharpBitmap                    ();
    bool                                EnsureSharpBitmap                 (GRP2DCANVAS* referencecanvas, double density);
    void                                InvalidateSharpBitmap             ();

    XBYTE                               GetAlpha                          ();
    void                                SetAlpha                          (XBYTE alpha);

    virtual void                        ReapplyStyleVisual                ();

  private:

    class SERIE
    {
      public:
                                        SERIE                             ()                                              { r = 56; g = 139; b = 253;                             }
                                       ~SERIE                             ()                                              { values.DeleteAll();                                   }

        XSTRING                         name;
        XBYTE                           r;
        XBYTE                           g;
        XBYTE                           b;
        XVECTOR<double>                 values;
    };

    void                                Clean                             ();
    bool                                DeleteBitmap                      ();
    bool                                DeleteSharpBitmap                 ();
    bool                                RasterizeChartBitmap              (GRP2DCANVAS* referencecanvas, double density, GRPBITMAP*& out_bitmap);

    UI_ELEMENT_STATISTICSCHART_TYPE     charttype;
    XSTRING                             title;

    XVECTOR<XSTRING*>                   categories;
    XVECTOR<SERIE*>                     series;

    bool                                needsrebuild;
    GRPBITMAP*                          bitmap;
    GRPBITMAP*                          sharpbitmap;
    double                              sharp_density;
    XBYTE                               alpha;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


