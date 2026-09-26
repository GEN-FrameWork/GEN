/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartPie3D.h
* 
* @class      GRPSTATISTICSCHARTPIE3D
* @brief      Graphic Statistics Chart : 3D pie chart (tilted ellipse with thickness). Concrete chart type over the base.
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

#include "GRPStatisticsChart.h"

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPSTATISTICSCHARTPIE3D : public GRPSTATISTICSCHART
{
  public:
                                    GRPSTATISTICSCHARTPIE3D               ();
    virtual                        ~GRPSTATISTICSCHARTPIE3D               ();

    void                            SetTiltRatio                          (double ratio)                                  { tiltratio = ratio;                                    }
    double                          GetTiltRatio                          ()                                              { return tiltratio;                                     }
    void                            SetThicknessRatio                     (double ratio)                                  { thicknessratio = ratio;                               }
    double                          GetThicknessRatio                     ()                                              { return thicknessratio;                                }

  protected:

    bool                            DrawPlot                              (GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height);

    XDWORD                          GetLegendCount                        ();
    bool                            GetLegendEntry                        (XDWORD index, XSTRING** name, GRPSTATISTICSCHARTCOLOR& color);

  private:

    void                            DrawSectorTop                         (GRPSTATISTICSCHARTBUILDER& builder, double cx, double cy, double rx, double ry, double startdeg, double enddeg, GRPSTATISTICSCHARTCOLOR& color);
    void                            DrawSectorWall                        (GRPSTATISTICSCHARTBUILDER& builder, double cx, double cy, double rx, double ry, double thick, double startdeg, double enddeg, GRPSTATISTICSCHARTCOLOR& color);
    GRPSTATISTICSCHARTCOLOR         MakeShade                             (GRPSTATISTICSCHARTCOLOR& color, double factor);

    double                          tiltratio;
    double                          thicknessratio;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/



