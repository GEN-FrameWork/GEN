/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartBuilderSVG.h
* 
* @class      GRPSTATISTICSCHARTBUILDERSVG
* @brief      Graphic Statistics Chart : SVG output builder. Translates the abstract chart primitives into SVG XML (a string).
* @ingroup    GRAPHIC
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

#include "GRPStatisticsChartBuilder.h"

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPSTATISTICSCHARTBUILDERSVG : public GRPSTATISTICSCHARTBUILDER
{
  public:
                                    GRPSTATISTICSCHARTBUILDERSVG          ();
    virtual                         ~GRPSTATISTICSCHARTBUILDERSVG         ();

    GRPSTATISTICSCHARTOUTPUTFORMAT  GetFormat                             ();
    GRPVECTORFILETYPE               GetVectorFileType                     ();

    bool                            BeginDocument                         (double width, double height);
    bool                            EndDocument                           ();

    bool                            DrawRect                              (double x, double y, double width, double height, GRPSTATISTICSCHARTSTYLE& style);
    bool                            DrawLine                              (double x1, double y1, double x2, double y2, GRPSTATISTICSCHARTSTYLE& style);
    bool                            DrawText                              (double x, double y, XCHAR* text, GRPSTATISTICSCHARTTEXTSTYLE& style);
    bool                            DrawPolygon                           (const double* xy, XDWORD npoints, GRPSTATISTICSCHARTSTYLE& style);

    bool                            GetResult                             (XSTRING& result);

  private:

    void                            Clean                                 ();

    void                            AppendNumber                          (XSTRING& out, double value);
    void                            AppendFillAndStroke                   (XSTRING& out, GRPSTATISTICSCHARTSTYLE& style);
    void                            AppendColor                           (XSTRING& out, GRPSTATISTICSCHARTCOLOR& color);
    void                            AppendOpacity                         (XSTRING& out, XCHAR* attribute, GRPSTATISTICSCHARTCOLOR& color);
    XCHAR*                          AnchorToString                        (GRPSTATISTICSCHARTTEXTANCHOR anchor);
    void                            EscapeXML                             (XCHAR* text, XSTRING& out);

    XSTRING                         content;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




