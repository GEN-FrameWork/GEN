/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartLines.cpp
* 
* @class      GRPSTATISTICSCHARTLINES
* @brief      Graphic Chart : line chart implementation
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

/*---- PRECOMPILATION CONTROL ----------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "GRPStatisticsChartLines.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTLINES::GRPSTATISTICSCHARTLINES()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTLINES::GRPSTATISTICSCHARTLINES()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTLINES::~GRPSTATISTICSCHARTLINES()
* @brief      Destructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTLINES::~GRPSTATISTICSCHARTLINES()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTLINES::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
* @brief      Draw plot : grid, axes, one polyline (with point markers) per series and labels
* @ingroup    GRAPHIC
* 
* @param[in]  builder : output builder
* @param[in]  x : plot area left
* @param[in]  y : plot area top
* @param[in]  width : plot area width
* @param[in]  height : plot area height
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRPSTATISTICSCHARTLINES::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
{
  XDWORD ncats   = data.GetNCategories();
  XDWORD nseries = data.GetNSeries();

  if(!ncats || !nseries) return false;

  double rawmax  = data.GetMaxValue();
  double nicemax = 1.0;
  int    ndiv    = 5;

  ComputeNiceMax(rawmax, nicemax, ndiv);

  if(nicemax <= 0.0) return false;

  double axisfontsize = config.GetAxisFontSize();

  // value grid + value labels (Y axis)

  GRPSTATISTICSCHARTSTYLE gridstyle;
  gridstyle.SetStroke(config.GetGridColor(), 1.0);

  GRPSTATISTICSCHARTTEXTSTYLE valuestyle;
  valuestyle.color  = config.GetTextColor();
  valuestyle.size   = axisfontsize;
  valuestyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_END;

  for(int d=0; d<=ndiv; d++)
    {
      double value = (nicemax * (double)d) / (double)ndiv;
      double yy    = y + height - ((value / nicemax) * height);

      if(config.GetShowGrid())
        {
          builder.DrawLine(x, yy, x + width, yy, gridstyle);
        }

      if(config.GetShowAxisLabels())
        {
          XSTRING label;
          label.Format(__L("%g"), value);

          builder.DrawText(x - (axisfontsize * 0.5), yy + (axisfontsize * 0.35), label.Get(), valuestyle);
        }
    }

  // axes

  GRPSTATISTICSCHARTSTYLE axisstyle;
  axisstyle.SetStroke(config.GetAxisColor(), 1.5);

  builder.DrawLine(x, y + height, x + width, y + height, axisstyle);            // X axis
  builder.DrawLine(x, y, x, y + height, axisstyle);            // Y axis

  // series polylines + point markers

  double slotwidth = width / (double)ncats;
  double marker    = axisfontsize * 0.5;

  for(XDWORD s=0; s<nseries; s++)
    {
      GRPSTATISTICSCHARTSERIE* serie = data.GetSerie(s);
      if(!serie) continue;

      GRPSTATISTICSCHARTCOLOR color = serie->HasColor() ? serie->GetColor() : config.GetPaletteColor(s);

      GRPSTATISTICSCHARTSTYLE linestyle;
      linestyle.SetStroke(color, 2.0);

      GRPSTATISTICSCHARTSTYLE markstyle;
      markstyle.SetFill(color);

      double prevx = 0.0;
      double prevy = 0.0;

      for(XDWORD c=0; c<ncats; c++)
        {
          double value = serie->GetValue(c);
          double px    = x + (slotwidth * ((double)c + 0.5));
          double py    = y + height - ((value / nicemax) * height);

          if(c > 0)
            {
              builder.DrawLine(prevx, prevy, px, py, linestyle);
            }

          builder.DrawRect(px - (marker * 0.5), py - (marker * 0.5), marker, marker, markstyle);

          if(config.GetShowValues())
            {
              XSTRING vlabel;
              vlabel.Format(__L("%g"), value);

              GRPSTATISTICSCHARTTEXTSTYLE vstyle;
              vstyle.color  = config.GetTextColor();
              vstyle.size   = config.GetFontSize() * 0.9;
              vstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE;

              builder.DrawText(px, py - (marker * 0.5) - 3.0, vlabel.Get(), vstyle);
            }

          prevx = px;
          prevy = py;
        }
    }

  // category labels (X axis)

  if(config.GetShowAxisLabels())
    {
      GRPSTATISTICSCHARTTEXTSTYLE catstyle;
      catstyle.color  = config.GetTextColor();
      catstyle.size   = axisfontsize;
      catstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE;

      for(XDWORD c=0; c<ncats; c++)
        {
          XSTRING* category = data.GetCategory(c);
          if(!category) continue;

          double px = x + (slotwidth * ((double)c + 0.5));

          builder.DrawText(px, y + height + (axisfontsize * 1.3), category->Get(), catstyle);
        }
    }

  return true;
}


