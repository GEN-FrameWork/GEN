/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartArea.cpp
* 
* @class      GRPSTATISTICSCHARTAREA
* @brief      Graphic Chart : area chart implementation
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

#include "GRPStatisticsChartArea.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTAREA::GRPSTATISTICSCHARTAREA()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTAREA::GRPSTATISTICSCHARTAREA()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTAREA::~GRPSTATISTICSCHARTAREA()
* @brief      Destructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTAREA::~GRPSTATISTICSCHARTAREA()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTAREA::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
* @brief      Draw plot : grid, axes, one translucent filled area (with top line) per series and labels
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
bool GRPSTATISTICSCHARTAREA::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
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
          label.Format(_L("%g"), value);

          builder.DrawText(x - (axisfontsize * 0.5), yy + (axisfontsize * 0.35), label.Get(), valuestyle);
        }
    }

  // axes

  GRPSTATISTICSCHARTSTYLE axisstyle;
  axisstyle.SetStroke(config.GetAxisColor(), 1.5);

  builder.DrawLine(x, y + height, x + width, y + height, axisstyle);            // X axis
  builder.DrawLine(x, y, x, y + height, axisstyle);            // Y axis

  // per series : translucent filled area (piecewise trapezoids) + opaque top line

  double slotwidth = width / (double)ncats;
  double baseline  = y + height;                                               // value 0 line

  for(XDWORD s=0; s<nseries; s++)
    {
      GRPSTATISTICSCHARTSERIE* serie = data.GetSerie(s);
      if(!serie) continue;

      GRPSTATISTICSCHARTCOLOR color = serie->HasColor() ? serie->GetColor() : config.GetPaletteColor(s);

      GRPSTATISTICSCHARTCOLOR fillcolor = color;
      fillcolor.a = 100;                                                        // translucent so overlapping areas show

      GRPSTATISTICSCHARTSTYLE areastyle;
      areastyle.SetFill(fillcolor);

      GRPSTATISTICSCHARTSTYLE linestyle;
      linestyle.SetStroke(color, 2.0);

      //  one filled polygon per series (top points left to right, then down to the baseline) : no internal seams

      XVECTOR<double> points;

      for(XDWORD c=0; c<ncats; c++)
        {
          double value = serie->GetValue(c);
          double px    = x + (slotwidth * ((double)c + 0.5));
          double py    = baseline - ((value / nicemax) * height);

          points.Add(px);
          points.Add(py);
        }

      double firstx = x + (slotwidth * 0.5);
      double lastx  = x + (slotwidth * ((double)(ncats - 1) + 0.5));

      points.Add(lastx);   points.Add(baseline);                               // close down at the right
      points.Add(firstx);  points.Add(baseline);                               // and back along the baseline

      builder.DrawPolygon(points.GetPointer(0), points.GetSize() / 2, areastyle);

      points.DeleteAll();

      //  opaque top line over the fill

      double prevx = 0.0;
      double prevy = 0.0;

      for(XDWORD c=0; c<ncats; c++)
        {
          double value = serie->GetValue(c);
          double px    = x + (slotwidth * ((double)c + 0.5));
          double py    = baseline - ((value / nicemax) * height);

          if(c > 0)
            {
              builder.DrawLine(prevx, prevy, px, py, linestyle);
            }

          prevx = px;
          prevy = py;
        }

      if(config.GetShowValues())
        {
          GRPSTATISTICSCHARTTEXTSTYLE vstyle;
          vstyle.color  = config.GetTextColor();
          vstyle.size   = config.GetFontSize() * 0.9;
          vstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE;

          for(XDWORD c=0; c<ncats; c++)
            {
              double value = serie->GetValue(c);
              double px    = x + (slotwidth * ((double)c + 0.5));
              double py    = baseline - ((value / nicemax) * height);

              XSTRING vlabel;
              vlabel.Format(_L("%g"), value);

              builder.DrawText(px, py - 4.0, vlabel.Get(), vstyle);
            }
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


