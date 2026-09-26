/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartBars.cpp
* 
* @class      GRPSTATISTICSCHARTBARS
* @brief      Graphic Chart : bars chart (horizontal grouped bars) implementation
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

#include "GRPStatisticsChartBars.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTBARS::GRPSTATISTICSCHARTBARS()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTBARS::GRPSTATISTICSCHARTBARS()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRPSTATISTICSCHARTBARS::~GRPSTATISTICSCHARTBARS()
* @brief      Destructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRPSTATISTICSCHARTBARS::~GRPSTATISTICSCHARTBARS()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRPSTATISTICSCHARTBARS::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
* @brief      Draw plot : value grid, axes, grouped horizontal bars and labels (categories on Y, values on X)
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
bool GRPSTATISTICSCHARTBARS::DrawPlot(GRPSTATISTICSCHARTBUILDER& builder, double x, double y, double width, double height)
{
  XDWORD ncats   = data.GetNCategories();
  XDWORD nseries = data.GetNSeries();

  if(!ncats || !nseries) return false;

  double rawmax  = data.GetMaxValue();
  double nicemax = 1.0;
  int    ndiv    = 5;

  ComputeNiceMax(rawmax, nicemax, ndiv);

  if(nicemax <= 0.0) return false;

  double fontsize     = config.GetFontSize();
  double axisfontsize = config.GetAxisFontSize();

  // value grid (vertical lines) + value labels (X axis)

  GRPSTATISTICSCHARTSTYLE gridstyle;
  gridstyle.SetStroke(config.GetGridColor(), 1.0);

  GRPSTATISTICSCHARTTEXTSTYLE valuestyle;
  valuestyle.color  = config.GetTextColor();
  valuestyle.size   = axisfontsize;
  valuestyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_MIDDLE;

  for(int d=0; d<=ndiv; d++)
    {
      double value = (nicemax * (double)d) / (double)ndiv;
      double xx    = x + ((value / nicemax) * width);

      if(config.GetShowGrid())
        {
          builder.DrawLine(xx, y, xx, y + height, gridstyle);
        }

      if(config.GetShowAxisLabels())
        {
          XSTRING label;
          label.Format(__L("%g"), value);

          builder.DrawText(xx, y + height + (axisfontsize * 1.3), label.Get(), valuestyle);
        }
    }

  // axes

  GRPSTATISTICSCHARTSTYLE axisstyle;
  axisstyle.SetStroke(config.GetAxisColor(), 1.5);

  builder.DrawLine(x, y + height, x + width, y + height, axisstyle);            // X axis
  builder.DrawLine(x, y, x, y + height, axisstyle);            // Y axis

  // grouped horizontal bars

  double slotheight  = height / (double)ncats;
  double grouppad    = slotheight * 0.15;
  double groupheight = slotheight - (grouppad * 2.0);
  double barheight   = groupheight / (double)nseries;

  GRPSTATISTICSCHARTTEXTSTYLE catstyle;
  catstyle.color  = config.GetTextColor();
  catstyle.size   = axisfontsize;
  catstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_END;

  for(XDWORD c=0; c<ncats; c++)
    {
      double sloty = y + (slotheight * (double)c);

      for(XDWORD s=0; s<nseries; s++)
        {
          GRPSTATISTICSCHARTSERIE* serie = data.GetSerie(s);
          if(!serie) continue;

          double value = serie->GetValue(c);
          double barw  = (value / nicemax) * width;
          double barx  = x;
          double bary  = sloty + grouppad + (barheight * (double)s);

          GRPSTATISTICSCHARTCOLOR color = serie->HasColor() ? serie->GetColor() : config.GetPaletteColor(s);

          GRPSTATISTICSCHARTSTYLE barstyle;
          barstyle.SetFill(color);

          builder.DrawRect(barx, bary, barw, barheight, barstyle);

          if(config.GetShowValues())
            {
              XSTRING vlabel;
              vlabel.Format(__L("%g"), value);

              GRPSTATISTICSCHARTTEXTSTYLE vstyle;
              vstyle.color  = config.GetTextColor();
              vstyle.size   = fontsize * 0.9;
              vstyle.anchor = GRPSTATISTICSCHARTTEXTANCHOR_START;

              builder.DrawText(barx + barw + 3.0, bary + (barheight * 0.5) + (fontsize * 0.3), vlabel.Get(), vstyle);
            }
        }

      if(config.GetShowAxisLabels())
        {
          XSTRING* category = data.GetCategory(c);
          if(category)
            {
              builder.DrawText(x - (axisfontsize * 0.5), sloty + (slotheight * 0.5) + (axisfontsize * 0.35), category->Get(), catstyle);
            }
        }
    }

  return true;
}


