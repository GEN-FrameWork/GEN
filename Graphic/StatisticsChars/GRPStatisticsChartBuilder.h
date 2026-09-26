/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartBuilder.h
* 
* @class      GRPSTATISTICSCHARTBUILDER
* @brief      Graphic Statistics Chart : abstract output builder. Charts draw against this vocabulary of primitives, a
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

#include "XBase.h"
#include "XString.h"

#include "GRPVectorFile.h"

#include "GRPStatisticsChartStyle.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum GRPSTATISTICSCHARTOUTPUTFORMAT
{
  GRPSTATISTICSCHARTOUTPUTFORMAT_UNKNOWN           = 0 ,
  GRPSTATISTICSCHARTOUTPUTFORMAT_SVG               ,
};



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPSTATISTICSCHARTBUILDER
{
  public:
                                            GRPSTATISTICSCHARTBUILDER     ()                                              {                                                       }
    virtual                                 ~GRPSTATISTICSCHARTBUILDER    ()                                              {                                                       }

    virtual GRPSTATISTICSCHARTOUTPUTFORMAT  GetFormat                     () = 0;
    virtual GRPVECTORFILETYPE               GetVectorFileType             () = 0;

    virtual bool                            BeginDocument                 (double width, double height) = 0;
    virtual bool                            EndDocument                   () = 0;

    virtual bool                            DrawRect                      (double x, double y, double width, double height, GRPSTATISTICSCHARTSTYLE& style) = 0;
    virtual bool                            DrawLine                      (double x1, double y1, double x2, double y2, GRPSTATISTICSCHARTSTYLE& style) = 0;
    virtual bool                            DrawText                      (double x, double y, XCHAR* text, GRPSTATISTICSCHARTTEXTSTYLE& style) = 0;
    virtual bool                            DrawPolygon                   (const double* xy, XDWORD npoints, GRPSTATISTICSCHARTSTYLE& style) = 0;

    virtual bool                            GetResult                     (XSTRING& result) = 0;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




